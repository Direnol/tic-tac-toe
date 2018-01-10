#ifndef TIC_TAC_TOE_GUI_H
#define TIC_TAC_TOE_GUI_H

#include "../Client/ClientChat.h"
#include <ncurses.h>
#include <form.h>
#include <sys/ioctl.h>
#include <csignal>
#include <string>

void resize_term(int sig);

123
class Gui : public ClientChat {
private:
    int mx, // MainWindow x
        my; // MainWidnwo y
    int gx, // GameWindow x
        gy; // GameWindow y
    WINDOW *game;
    WINDOW *chat_in_box; // box for input_chat
    WINDOW *chat_out_box;
    // 0 - menu | 1 - game
    int status;
    WINDOW *area;
    tic_tac play;
    Chat chat;
    WINDOW *input_chat;
    WINDOW *output_chat;
    thread tid;
    bool work;
    int cur;

    void DelWin();

    void InitAllWin();

    void initMainWindow();

    void initGameWindow();

    void initChatInWindow();

    void initChatOutWindow();

    void menu();

    void tic_tac_toe();

    void paint(int x, int y, int sybmol, int type_char = 0);

    int pressedKey(unsigned int key);

    void keymap_chat(chtype c);

    void SendMessage();

    size_t rtrim(char *s);

    void RecvMessage();

    void createGame();

    void connectToGame();

    void serverList();

    void sendGameProcess();

    void start_chat();

    void autoLose();

    void printMessage(char *s, int _my);

public:

    Gui();

    Gui(char *ip, uint16_t port);

    ~Gui() override;

    void repaint();

    void loop();
};

#endif //TIC_TAC_TOE_GUI_H
