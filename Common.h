#ifndef TIC_TAC_TOE_COMMON_H
#define TIC_TAC_TOE_COMMON_H

#include <cstdint>
#include <queue>
#include <vector>
#include <string>
#include <netinet/in.h>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

enum ERROR_CHAT {
    ERROR_IP = 1,
    ERROR_PORT,
    ERROR_SOCK,
    ERROR_BIND,
    ERROR_LOG,
    ERROR_CONNECT
};

#endif //TIC_TAC_TOE_COMMON_H
