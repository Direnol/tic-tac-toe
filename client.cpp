#include "ClientChat.h"

int main() {
    try {
        ClientChat("192.168.1.7", 6969);
    } catch (ERROR_CHAT e) {
        printf("Error: %d", e);
    }

    return 0;
}