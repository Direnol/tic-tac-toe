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
}

ServerChat::~ServerChat()
{
    log.close();
    close(sock);
}
int ServerChat::add_players(int p1, int p2)
{
    return 0;
}
int ServerChat::server()
{
    return 0;
}
int ServerChat::connect2(int player1, int player2)
{
    return 0;
}
