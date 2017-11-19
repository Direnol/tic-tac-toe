#ifndef TIC_TAC_TOE_ServerChat_H
#define TIC_TAC_TOE_ServerChat_H

#include "Common.h"

class ServerChat {
private:
    int sock;
    queue<int, pair<string, string>> qmsg;
    // <sock, addr>
    map<int, sockaddr_in> sockets;
    // <name, sock>
    map<string, int> players;
    ofstream log;
    bool work;
    vector<thread> threads;
    mutex mutex;
public:
    ServerChat(const char *ip, uint16_t port);
    ~ServerChat();
    int listen_connect(int player);
    int server();
    void dlock();
    void dunlock();
    int connect2(int player1, int player2);
    void logging(string s);
};

#endif //TIC_TAC_TOE_ServerChat_H
