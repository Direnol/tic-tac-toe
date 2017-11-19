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
#include <string.h>
#include <mutex>
#include <sys/socket.h>

#define BUFFER_SIZE 128

using namespace std;

enum ERROR_CHAT {
    ERROR_IP = 1,
    ERROR_PORT,
    ERROR_SOCK,
    ERROR_BIND,
    ERROR_LOG,
    ERROR_CONNECT
};

#endif //TIC_TAC_TOE_COMMON_H
