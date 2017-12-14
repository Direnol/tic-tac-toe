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

struct tic_tac {
    char area[3][3];
};

struct Chat {
    int pos;
    int max;
};


// TODO: Description GUI for client
class Gui : public ClientChat {
private:
    int mx, // MainWindow x
        my; // MainWidnwo y
    int gx, // GameWindow x
        gy; // GameWindow y


    const chtype FIGURE[3][5][5] = {
        {{'-', '-', '-', '-', '-'},
         {'|', '\\', ' ', '/', '|'},
         {'|', ' ', 'x', ' ', '|'},
         {'|', '/', ' ', '\\', '|'},
         {'-', '-', '-', '-', '-'}

        },
        {{'-', '-', '-', '-', '-'},
         {'|', ' ', '-', ' ', '|'},
         {'|', '(', 'o', ')', '|'},
         {'|', ' ', '-', ' ', '|'},
         {'-', '-', '-', '-', '-'}
        },
        {{'-', '-', '-', '-', '-'},
         {'|', ' ', ' ', ' ', '|'},
         {'|', ' ', ' ', ' ', '|'},
         {'|', ' ', ' ', ' ', '|'},
         {'-', '-', '-', '-', '-'}
        }
    };

    WINDOW *game;
    WINDOW *chat_in_box; // box for input_chat
    WINDOW *chat_out_box;

    // 0 - menu | 1 - game
    int status;

    WINDOW *area;
    tic_tac play;

    Chat chat;

    WINDOW *input_chat;
    void DelWin();

    void InitAllWin();

    void initMainWindow();

    void initGameWindow();

    void initChatInWindow();

    void initChatOutWindow();

    void menu();

    void tic_tac_toe();

    void keymap_chat(chtype c);

    void paint(int x, int y, int c);

    ClientChat *tcp_chat;

public:
    Gui(char *ip, int port);

    ~Gui() override;

    void repaint();

    void loop();
};

#endif //TIC_TAC_TOE_GUI_H
