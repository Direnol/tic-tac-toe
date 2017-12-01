#ifndef TIC_TAC_TOE_CLIENTCHAT_H
#define TIC_TAC_TOE_CLIENTCHAT_H


#include "Common.h"

class ClientChat {
private:
    int sock;
    string ip;
    int port;
    string player_name;
public:
    ClientChat(const char *ip, int port);

    ~ClientChat();

    void nameSet();

    int nameSend();

    int messageSend();

    int messageRecv();

    void start();

    COMMANDS get_command(string message, msg pmsg);
};


#endif //TIC_TAC_TOE_CLIENTCHAT_H
