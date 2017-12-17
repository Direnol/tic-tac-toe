#include <ncurses.h>
#include "ServerChat.h"

int main()
{
    ServerChat *chat = nullptr;
    try {
        chat = new ServerChat("127.0.0.1", 6969);
    } catch (ERROR_CHAT e) {
        cout << e << endl;
    }
    chat->server();
    delete chat;
    return 0;
}