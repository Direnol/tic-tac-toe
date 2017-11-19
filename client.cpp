#include "ClientChat.h"

int main() {
    try {
        ClientChat("127.0.0.1", 6969);
    } catch (ERROR_CHAT e) {
        printf("Error: %d", e);
    }

    return 0;
}