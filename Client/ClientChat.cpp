#include <cstring>
#include "ClientChat.h"

ClientChat::ClientChat(const char *ip, int port)
{
    this->ip = ip;
    this->port = port;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in sock_in{};
    sock_in.sin_family = AF_INET;
    sock_in.sin_port = htons(static_cast<uint16_t>(port));
    inet_aton(ip, &sock_in.sin_addr);
    if (connect(sock, (sockaddr *) (&sock_in), sizeof(sock_in)) < 0)
        throw ERROR_CONNECT;

}

ClientChat::~ClientChat()
{
    if (sock != -1) close(sock);
    sock = -1;
}

int ClientChat::nameSend()
{
    char buf[BUFFER_SIZE];

    do {
        cout << "Enter name : ";
        player_name = "";
        nameSet();
        send(sock, player_name.c_str(),
             player_name.size(), 0
        );
        if (player_name.size() > PLAYER_NAME_SIZE) continue;
        recv(sock, buf, BUFFER_SIZE, 0);
    } while (strncmp(buf, "OK", 2) != 0);
    return 0;
}

void ClientChat::nameSet()
{
    cin >> player_name;
}

int ClientChat::messageSend(char *message, size_t n)
{
    msg pmsg = {};
    string _msg(message);
    pmsg.code = get_command(_msg, pmsg);

    if (n > 50) return 1;

    strcpy(pmsg.message, _msg.data());

    if (send(sock, &pmsg, sizeof(msg), 0) <= 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

void ClientChat::messageRecv(msg *pmsg)
{
    if (recv(sock, pmsg, sizeof(msg), 0) <= 0) {
        pmsg->code = -1;
    }
}

COMMANDS ClientChat::get_command(string &message, msg pmsg)
{
    if (message.size() > 2) {
        if (message.find("/sg", 0) != string::npos) {
            message.erase(0, 3);
            return OPTIONS;
        }
        if (message.find("/g", 0) != string::npos) {
            message.erase(0, 2);
            return GAME;
        }
    }
    return ALL;
}

ClientChat::ClientChat() = default;



