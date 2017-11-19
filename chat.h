#ifndef TIC_TAC_TOE_CHAT_H
#define TIC_TAC_TOE_CHAT_H

class Chat {
private:
    int sock;
    std::queue<pair<int, int>> q;
public:
    Chat(char *ip, uint16_t port);
    ~Chat();
    add_players(int p1, int p2);
    server();
    connect2(int player1, int player2);

};

#endif //TIC_TAC_TOE_CHAT_H
