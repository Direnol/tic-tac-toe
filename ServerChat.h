#ifndef TIC_TAC_TOE_ServerChat_H
#define TIC_TAC_TOE_ServerChat_H

#include <cstdint>
#include <queue>
#include <vector>
#include <string>
#include <netinet/in.h>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <map>
#include <mutex>
#include <thread>
#define BUFFER_SIZE 128

enum ERROR_CHAT{
    ERROR_IP,
    ERROR_PORT,
    ERROR_SOCK,
    ERROR_BIND,
    ERROR_LOG
};

class ServerChat {
private:
    int sock;
    std::queue<std::pair<int, int>> q;
    std::ofstream log;
    std::map<std::string, std::string> players; // <ip:port, name>
    std::map<int, std::string> sockets; // <fd, ip:port>
    std::vector<std::thread> threads;
    std::map<int, int> games; // <player1, player2>
    std::mutex mutex;
    bool work;
public:
    void dlock();
    void dunlock();
    ServerChat(const char *ip, uint16_t port);
    ~ServerChat();
    int listen_connect(int player);
    int server();
    int connect2(int player1, int player2);
    void logging(std::string);
};

#endif //TIC_TAC_TOE_ServerChat_H
