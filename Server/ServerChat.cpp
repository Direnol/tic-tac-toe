#include <sstream>
#include "ServerChat.h"

using namespace std;
ServerChat::ServerChat(const char *ip, uint16_t port)
{
    if (ip == nullptr) throw ERROR_IP;
    if (port < 0) throw ERROR_PORT;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) throw ERROR_SOCK;
    int on = 1;
    setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    sockaddr_in sock_in = {
            .sin_family = AF_INET,
            .sin_port = htons(port),
    };
    sock_in.sin_addr.s_addr = inet_addr(ip);

    if (bind(sock, (sockaddr *) (&sock_in), sizeof(sock_in)) < 0) throw ERROR_BIND;
    listen(sock, 2);
    char filelog[26];
    sprintf(filelog, "%s:%d.log", ip, port);
    log.open(filelog);
    if (!log.good()) throw ERROR_LOG;
    printf("Chat: ip:port=[%s:%d]\nlog=[%s]\n", ip, port, filelog);
    work = true;
}

ServerChat::~ServerChat()
{
    log.close();
    if (sock != -1) close(sock);
    sock = -1;
}

int ServerChat::listen_connect(int player)
{
    void *ptr = new char[BUFFER_SIZE];
    auto *pch = static_cast<char *>(ptr);
    auto *pmsg = static_cast<msg *>(ptr);
    ssize_t res;
    do {
        res = static_cast<int>(recv(player, ptr, BUFFER_SIZE, 0));
        if (res <= 0) {
            close(player);
            return 0;
        }
        if (players.find(string(pch)) == players.end()) {
            players.insert(make_pair(string(pch), make_pair(player, ALL)));
            this->sockets.insert(make_pair(player, string(pch)));
            send(player, "OK", strlen("OK"), 0);
            break;
        }
        send(player, "NO", strlen("NO"), 0);
    } while (true);

    string name(pch);
    dlock();
    logging(name + " has been conected");
    dunlock();
    while (work) {
        res = recv(player, ptr, BUFFER_SIZE, 0);
        if (res <= 0) {
            goto end;
        }
        switch (pmsg->code) {
            case (ALL): {
                string tmp(name);
                tmp += ":" + string(pmsg->message);
                strcpy(pmsg->message, tmp.c_str());
                dlock();
                logging(tmp);
                dunlock();
                for (auto &it : players) {
                    if (it.second.first == player) continue;
                    send(it.second.first, pmsg, sizeof(msg), 0);
                }
                break;
            }
            case (GAME) : {
                /*TODO handler for game:
                 * create, list, connect*/
                break;
            }
            case (OPTIONS): {
                if (strcmp("out", pmsg->message) == 0) {
                    goto end;
                } else if (strcmp("kill", pmsg->message) == 0) {
                    work = false;
                    if (sock != -1) close(sock);
                    sock = -1;
                    dlock();
                    logging("Server off");
                    dunlock();
                    // TODO: off server
                }
                break;
            }
            default: {
                cout << "Default switch find " << pmsg->code << endl;
                break;
            }
        }
    }
    end:
    delete (char *)ptr;
    players.erase(name);
    sockets.erase(player);
    close(player);
    dlock();
    logging(name + " has been disconect");
    dunlock();
    return 0;
}

int ServerChat::server()
{
    int new_fd = -1;
    sockaddr_in new_connect{};
    socklen_t socklen = sizeof(new_connect);
    string nameconnect;

    while (work) {
        // check connect;
        new_fd = accept(sock, (sockaddr *) (&new_connect), &socklen);
        if (new_fd <= 0) continue;
        nameconnect = string(inet_ntoa(new_connect.sin_addr)) + ":" +
                      to_string(new_connect.sin_port);
        this->dlock();
        log << "New connect [" << nameconnect << "] ";
        log << get_time() << __DATE__ << "\n";
        this->dunlock();
        threads.emplace_back(thread(&ServerChat::listen_connect, this, new_fd));
    }
    return 0;
}
int ServerChat::connect2(int player1, int player2)
{
    return 0;
}

void ServerChat::dlock()
{
    mut.lock();
}

void ServerChat::dunlock()
{
    mut.unlock();
}
void ServerChat::logging(string s)
{
    log << get_time() << ":" << s << endl;
}

string ServerChat::get_time() {
    time_t cur_sec = time(nullptr);

    stringstream time;

    tm *tmp_time = localtime(&cur_sec);

    asctime(tmp_time);
    time << tmp_time->tm_hour << ':' << tmp_time->tm_min << ':' << tmp_time->tm_sec;
    time << " ";
    return time.str();
}

bool ServerChat::getWinner(tic_tac *game, int &who)
{
    return false;
}
