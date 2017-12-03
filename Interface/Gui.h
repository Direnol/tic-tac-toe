#ifndef TIC_TAC_TOE_GUI_H
#define TIC_TAC_TOE_GUI_H

#include "../Client/ClientChat.h"
#include <ncurses.h>
#include <form.h>
#include <sys/ioctl.h>
#include <csignal>

/* TODO: Description ncurses text-style
    A_NORMAL             //- обычный режим
    A_STANDOUT           //- самое яркое выделение из всех возможных
    A_UNDERLINE          //- подчёркивание
    A_BLINK              //- мигание
    A_DIM                //- тусклый символ
    A_BOLD               //- выделение жирным шрифтом
*/


void resize_term(int sig);

// TODO: Description GUI for client
class Gui : public ClientChat {
private:
    int mx, // MainWindow x
        my; // MainWidnwo y
    int gx, // GameWindow x
        gy; // GameWindow y

    WINDOW *game;
    WINDOW *chat_in;
    WINDOW *chat_out;

    void DelWin();

    void InitAllWin();
public:
    Gui();

    ~Gui() override;

    void initMainWindow();

    void initGameWindow();

    void initChatInWindow();

    void initChatOutWindow();

    void repaint();

    void loop();
};


#endif //TIC_TAC_TOE_GUI_H
