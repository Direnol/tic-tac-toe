#ifndef TIC_TAC_TOE_ServerChat_H
#define TIC_TAC_TOE_ServerChat_H

#include "Common.h"

class ServerChat {
private:
    int sock;
    // <sock, name>
    map<int, string> sockets;
    // <name, <sock, status> >
    map<string, pair<int, int> > players;

    ofstream log;
    bool work;
    vector<thread> threads;
    mutex mut;
public:
    ServerChat(const char *ip, uint16_t port);
    ~ServerChat();
    int listen_connect(int player);
    int server();
    void dlock();
    void dunlock();
    int connect2(int player1, int player2);
    void logging(string s);

    string get_time();

    string get_date();

};

#endif //TIC_TAC_TOE_ServerChat_H
