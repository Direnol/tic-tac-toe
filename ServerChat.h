#ifndef TIC_TAC_TOE_ServerChat_H
#define TIC_TAC_TOE_ServerChat_H

#include "Common.h"

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
