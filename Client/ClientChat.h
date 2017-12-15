#ifndef TIC_TAC_TOE_CLIENTCHAT_H
#define TIC_TAC_TOE_CLIENTCHAT_H

#include "../Interface/Gui.h"
#include "../Headers/Common.h"

class ClientChat {
private:
    int sock;
    string ip;
    int port;
    Gui *gui;
    string player_name;
public:
    ClientChat();

    ClientChat(const char *ip, int port);

    virtual ~ClientChat();

    void nameSet();

    int nameSend();

    int messageSend();

    int messageRecv();

    void start();

    COMMANDS get_command(string message, msg pmsg);
};


#endif //TIC_TAC_TOE_CLIENTCHAT_H
