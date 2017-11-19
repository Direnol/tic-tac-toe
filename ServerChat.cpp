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
    close(sock);
}
int ServerChat::listen_connect(int player)
{
    void *ptr = new char[BUFFER_SIZE];
    char *pch = nullptr;
    int res;
    do {
        res = static_cast<int>(recv(player, ptr, BUFFER_SIZE, 0));
        if (res <= 0) {
            close(res);
            return 0;
        }
        pch = static_cast<char *>(ptr);
        if (players.find(string(pch)) == players.end()) {
            players.insert(make_pair(string(pch), player));
            send(player, "OK", strlen("OK"), 0);
            break;
        }
        send(player, "NO", strlen("NO"), 0);
    } while (true);
    dlock();
    log << pch << " has been connected" << endl;
    dunlock();

    while ((res = static_cast<int>(recv(sock, ptr, BUFFER_SIZE, 0)))) {


    }

    delete (char *)ptr;
    close(player);
    return 0;
}

int ServerChat::server()
{
    int new_fd = -1;
    sockaddr_in new_connect;
    socklen_t socklen = sizeof(new_connect);
    string nameconnect;
    while (work) {
        // check connect;
        new_fd = accept(sock, (sockaddr *) (&new_connect), &socklen);
        if (new_fd <= 0) continue;
        nameconnect = string(inet_ntoa(new_connect.sin_addr)) + ":" +
            to_string(new_connect.sin_port);
        this->dlock();
        log << "New connect [" << nameconnect << "]\n";
        this->sockets.insert(make_pair(new_fd, new_connect));
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
    mutex.lock();
}

void ServerChat::dunlock()
{
    mutex.unlock();
}
void ServerChat::logging(string s)
{
    log << s << endl;
}
