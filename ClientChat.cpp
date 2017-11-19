#include <cstring>
#include "ClientChat.h"

ClientChat::ClientChat(const char *ip, int port) {
    this->ip = ip;
    this->port = port;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in sock_in;
    sock_in.sin_family = AF_INET;
    sock_in.sin_port = htons(static_cast<uint16_t>(port));
    inet_aton(ip, &sock_in.sin_addr);
    if (connect(sock, (sockaddr *) (&sock_in), sizeof(sock_in)) < 0)
        throw ERROR_CONNECT;

}

ClientChat::~ClientChat() {}

int ClientChat::nameSend() {
    char buf[BUFFER_SIZE];

    do {
        cout << "Enter name : ";
        nameSet();
        send(sock, player_name.c_str(),
             player_name.size(), 0
        );
        recv(sock, buf, BUFFER_SIZE, 0);
    } while (strcmp(buf, "OK") != 0);

    return 0;
}

void ClientChat::nameSet() {
    cin >> player_name;
}

int ClientChat::messageSend() {
    msg pmsg;

    pmsg.code = ALL;
    string tmp;
    while (true) {
        getline(cin, tmp);

        if (tmp.size() > 50) return 1;

        strcpy(pmsg.message, tmp.c_str());

        if (send(sock, &pmsg, sizeof(msg), 0) <= 0) break;

    }
    return 0;
}

int ClientChat::messageRecv() {
    msg pmsg;
    while (true) {
        if (recv(sock, &pmsg, sizeof(msg), 0) <= 0) break;

        cout << pmsg.code << ' ' << pmsg.message << endl;
    }
    return 0;
}

void ClientChat::start() {

    thread th[2];

    th[0] = thread(&ClientChat::messageSend, this);
    th[1] = thread(&ClientChat::messageRecv, this);

    for (int i = 0; i < 2; ++i) th[i].join();
}



