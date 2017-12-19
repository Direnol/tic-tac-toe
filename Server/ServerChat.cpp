#include <sstream>
#include "ServerChat.h"

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
    if (sock != -1) close(sock);
    sock = -1;
    for (auto &it : sockets) {
        sockets.erase(it.first);
        close(it.first);
    }
    for (auto &it : threads) it.join();
    logging("Server is closed");
    log.close();
}


int ServerChat::server()
{
    int new_fd;
    sockaddr_in new_connect{};
    socklen_t socklen = sizeof(new_connect);
    string nameconnect;

    while (work) {
        // check connect;
        new_fd = accept(sock, (sockaddr *) (&new_connect), &socklen);
        if (new_fd <= 0) continue;
        nameconnect = string(inet_ntoa(new_connect.sin_addr)) + ":" +
            to_string(new_connect.sin_port);
        logging("New connect [" + nameconnect + "] " + get_time() + __DATE__);
        threads.emplace_back(thread(&ServerChat::listen_connect, this, new_fd));
    }
    logging("Server close connect");
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
    dlock();
    log << get_time() << ": " << s << endl;
    cout << get_time() << ": " << s << endl;
    dunlock();
}

string ServerChat::get_time()
{
    time_t cur_sec = time(nullptr);
    stringstream time;
    tm *tmp_time = localtime(&cur_sec);
    asctime(tmp_time);
    time << tmp_time->tm_hour << ':' << tmp_time->tm_min << ':' << tmp_time->tm_sec;
    return time.str();
}


int ServerChat::listen_connect(int player)
{
    void *ptr = new char[BUFFER_SIZE];
    auto *pch = static_cast<char *>(ptr);
    auto *pmsg = static_cast<msg *>(ptr);
    int status = 0;
    ssize_t res;
    if (Login(res, player, pch, ptr)) {
        delete pch;
        return EXIT_FAILURE;
    }
    string name(pch);
    logging(name + " has been conected");
    auto *game_struct = reinterpret_cast<tic_tac *>(pmsg->message);
    while (work) {
        res = recv(player, ptr, BUFFER_SIZE, 0);
        if (res <= 0) {
            break;
        }
        switch (pmsg->code) {
            case (ALL): {
                string tmp(name);
                tmp += ":" + string(pmsg->message);
                strcpy(pmsg->message, tmp.c_str());
                logging(tmp);
                for (auto &it : players) {
                    if (it.second.first == player) continue;
                    send(it.second.first, pmsg, sizeof(msg), 0);
                }
                break;
            }
            case (GAME) : {
                if (status) {
                    pmsg->code = ALL;
                    strcpy(pmsg->message, "You are already in game or queue for game");
                    send(player, pmsg, sizeof(msg), 0);
                    break;
                }
                switchGame(status, pmsg, player, name);
                break;
            }
            case (OPTIONS): {
                if (strcmp("out", pmsg->message) == 0) {
                    goto end;
                } else if (strcmp("kill", pmsg->message) == 0) {
                    work = false;
                    if (sock != -1) close(sock);
                    sock = -1;
                    logging("Signal SERVER_OFF");
                }
                break;
            }
            case (PROCESS_GAME) : {
                switchProcessGame(status, pmsg, player, name, game_struct);
                break;
            }
            case FINISH_GAME: {
                switchFinishGame(status, pmsg, player, name, game_struct);
                break;
            }
            default: {
                cout << "Default switch find " << pmsg->code << endl;
                break;
            }
        }
    }
    end:
    if (status) {
        // TODO FAILED END or lose
    }
    delete (char *) ptr;
    players.erase(name);
    if (sockets.find(player) != sockets.end()) {
        close(player);
        sockets.erase(player);
    }
    logging(name + " has been disconect");
    return EXIT_SUCCESS;
}

int ServerChat::Login(ssize_t &res, int player, char *pch, void *ptr)
{
    do {
        res = static_cast<int>(recv(player, ptr, BUFFER_SIZE, 0));
        if (res <= 0) {
            close(player);
            return EXIT_FAILURE;
        }
        pch[res] = '\0';
        if (players.find(string(pch)) == players.end()) {
            players.insert(make_pair(string(pch), make_pair(player, -1)));
            this->sockets.insert(make_pair(player, string(pch)));
            send(player, "OK", strlen("OK"), 0);
            break;
        }
        send(player, "NO", strlen("NO"), 0);
    } while (true);
    return EXIT_SUCCESS;
}




