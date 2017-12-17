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

