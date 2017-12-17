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
#include <string.h>

#define BUFFER_SIZE 128
#define PLAYER_NAME_SIZE 10

using namespace std;

struct tic_tac {
    char area[3][3];
    int i; // coords game area
    int j; //
    int figure;
    int winner;

    tic_tac()
    {
        i = j = 0;
        figure = 2;
        winner = -1;
        for (auto &i : area)
            memset(i, 2, 3);
    }
};

struct Chat {
    int pos;
    int max;
};

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

enum COMMANDS {
    ALL,
    GAME,
    OPTIONS
};

#endif //TIC_TAC_TOE_COMMON_H
