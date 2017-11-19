#include <cstring>
#include "ClientChat.h"

ClientChat::ClientChat(const char *ip, int port) {
    this->ip = ip;
    this->port = port;

    sockaddr_in sock_in;
    sock_in.sin_family = AF_INET;
    sock_in.sin_port = htons(static_cast<uint16_t>(port));
    inet_aton(ip, &sock_in.sin_addr);
    if (connect(sock, (sockaddr *) (&sock_in), sizeof(sock_in)))
        throw ERROR_CONNECT;
    string player_name;
    cin >> player_name;


    char buf[BUFFER_SIZE];

    do {
        send(sock, player_name.c_str(),
             player_name.size(), 0
        );
        recv(sock, buf, BUFFER_SIZE, 0);
    } while (strcmp(buf, "OK") != 0);

}

ClientChat::~ClientChat() {

}
