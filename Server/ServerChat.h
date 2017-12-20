#ifndef TIC_TAC_TOE_ServerChat_H
#define TIC_TAC_TOE_ServerChat_H

#include "../Headers/Common.h"

class ServerChat {
private:
    int sock;
    // <sock, name>
    map<int, string> sockets;
    // <name, <my_sock, alien> >
    map<string, pair<int, int> > players;
    //queue for game
    deque<int> qgame;
    ofstream log;
    bool work;
    vector<thread> threads;
    mutex mut;

    int listen_connect(int player);

    void dlock();

    void dunlock();

    void logging(string s);

    string get_time();

    bool getWinner(tic_tac *game, int &who);

    void switchGame(int &status, msg *pmsg, int player, string &name);

    void switchFinishGame(int &status, msg *pmsg, int player, string &name, tic_tac *game_struct);

    void switchProcessGame(int &status, msg *pmsg, int player, string &name, tic_tac *game_struct);

    int Login(ssize_t &res, int player, char *pch, void *ptr);

public:

    ServerChat(const char *ip, uint16_t port);

    int server();

    ~ServerChat();
};

#endif //TIC_TAC_TOE_ServerChat_H
