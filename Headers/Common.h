#ifndef TIC_TAC_TOE_COMMON_H
#define TIC_TAC_TOE_COMMON_H

#include <queue>
#include <vector>
#include <string>
#include <netinet/in.h>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <map>
#include <cstring>
#include <mutex>
#include <sys/socket.h>
#include <iostream>
#include <ctime>


#define BUFFER_SIZE 128
#define PLAYER_NAME_SIZE 10

using namespace std;

struct msg {
    int code;
    char message[60];
};

enum ERROR_CHAT {
    ERROR_IP = 1,
    ERROR_PORT,
    ERROR_SOCK,
    ERROR_BIND,
    ERROR_LOG,
    ERROR_CONNECT
};

enum COMMANDS{
    ALL,
    GAME,
    OPTIONS
};

#endif //TIC_TAC_TOE_COMMON_H
