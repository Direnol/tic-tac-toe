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
#include <ncurses.h>

#define BUFFER_SIZE 128
#define PLAYER_NAME_SIZE 10

using namespace std;

const chtype FIGURE[3][5][5] = {
        {{'-', '-', '-', '-', '-'},
                {'|', '\\', ' ', '/', '|'},
                {'|', ' ', 'x', ' ', '|'},
                {'|', '/', ' ', '\\', '|'},
                {'-', '-', '-', '-', '-'}

        },
        {{'-', '-', '-', '-', '-'},
                {'|', ' ',  '-', ' ', '|'},
                {'|', '(', 'o', ')', '|'},
                {'|', ' ', '-', ' ',  '|'},
                {'-', '-', '-', '-', '-'}
        },
        {{'-', '-', '-', '-', '-'},
                {'|', ' ',  ' ', ' ', '|'},
                {'|', ' ', ' ', ' ', '|'},
                {'|', ' ', ' ', ' ',  '|'},
                {'-', '-', '-', '-', '-'}
        }
};

struct tic_tac {
    char area[3][3]{};
    int i; // coords game area
    int j; //
    int winner;
    int cur_player;
    int figure;

    tic_tac()
    {
        i = j = figure = 0;
        winner = cur_player = -1;
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
    OPTIONS,
    INIT,
    PROCESS_GAME,
    FINISH_GAME
};

#endif //TIC_TAC_TOE_COMMON_H
