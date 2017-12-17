#ifndef TIC_TAC_TOE_CLIENTCHAT_H
#define TIC_TAC_TOE_CLIENTCHAT_H

#include "../Headers/Common.h"

class ClientChat {
protected:
    int sock;
    string ip;
    int port;
    string player_name;
public:
    ClientChat();

    ClientChat(const char *ip, int port);

    virtual ~ClientChat();

    void nameSet();

    int nameSend();

    int messageSend(char *message, size_t n);

    void messageRecv(msg *pmsg);

    COMMANDS get_command(string &message, msg pmsg);

};

#endif //TIC_TAC_TOE_CLIENTCHAT_H
