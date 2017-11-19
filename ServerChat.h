#ifndef TIC_TAC_TOE_ServerChat_H
#define TIC_TAC_TOE_ServerChat_H

#include <cstdint>
#include <queue>
#include <vector>
#include <string>
#include <netinet/in.h>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>


enum ERROR_CHAT{
    ERROR_IP,
    ERROR_PORT,
    ERROR_SOCK,
    ERROR_BIND,
    ERROR_LOG
};

class ServerChat {
private:
    int sock;
    std::queue<std::pair<int, int>> q;
    std::ofstream log;
public:
    ServerChat(const char *ip, uint16_t port);
    ~ServerChat();
    int add_players(int p1, int p2);
    int server();
    int connect2(int player1, int player2);
};

#endif //TIC_TAC_TOE_ServerChat_H
