#include "ClientChat.h"

ClientChat::ClientChat(const char *ip, int port) {
    sockaddr_in sock_in;
    sock_in.sin_family = AF_INET;
    sock_in.sin_port = htons(static_cast<uint16_t>(port));
    inet_aton(ip, &sock_in.sin_addr);
    if (connect(sock, (sockaddr *) (&sock_in), sizeof(sock_in)))
        throw ERROR_CONNECT;
}

ClientChat::~ClientChat() {

}
