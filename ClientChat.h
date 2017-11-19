#ifndef TIC_TAC_TOE_CLIENTCHAT_H
#define TIC_TAC_TOE_CLIENTCHAT_H


#include "Common.h"

class ClientChat {
private:
    int sock;
public:
    ClientChat(const char *ip, int port);

    ~ClientChat();

};


#endif //TIC_TAC_TOE_CLIENTCHAT_H
