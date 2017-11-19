#include <ncurses.h>
#include "ServerChat.h"

int main()
{
    try {
        ServerChat foo("192.168.1.7", 6969);
        foo.server()
    } catch (ERROR_CHAT e) {
        printf("%d\n", e);
    }

    return 0;
}