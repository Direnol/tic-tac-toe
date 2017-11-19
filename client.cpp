#include "ClientChat.h"

int main() {

    ClientChat client("127.0.0.1", 6969);
    client.nameSend();
    client.start();


    return 0;
}