#include "Gui.h"

Gui::Gui()
{
    InitAllWin();
    status = 0;
    for (auto &i : play.area)
        memset(i, 2, 3);

    initMainWindow();
}

void Gui::initMainWindow()
{
    initGameWindow();
    initChatOutWindow();
    initChatInWindow();
}

void Gui::initGameWindow()
{
    game = newwin(1 + my - (gy / 2), gx, 1, 1);
    box(game, 0, 0);
    area = derwin(game, getmaxy(game) - 2, getmaxx(game) - 2, 1, 1);
    wrefresh(game);
}

void Gui::initChatInWindow()
{
    int input_area = my - (gy * 2 - gy * 1 / 2);
    chat_in = newwin((input_area) < 7 ? input_area - 1 : input_area, mx - gx - 2, my - (gy - 3), gx + 1);
    box(chat_in, 0, 0);
    wrefresh(chat_in);
    input = derwin(chat_in, getmaxy(chat_in) - 2, getmaxx(chat_in) - 2, 1, 1);

}

void Gui::initChatOutWindow()
{
    chat_out = newwin(my - (gy - 2), mx - gx - 2, 1, gx + 1);
    box(chat_out, 0, 0);
    wrefresh(chat_out);

}

Gui::~Gui()
{
    DelWin();
}

void Gui::repaint()
{
    clear();
    DelWin();
    InitAllWin();
    initMainWindow();
}

void Gui::DelWin()
{
    delwin(input);
    delwin(area);
    delwin(game);
    delwin(chat_out);
    delwin(chat_in);
    endwin();

}
void Gui::InitAllWin()
{
    initscr();
    getmaxyx(stdscr, this->my, this->mx);
    curs_set(0);
    box(stdscr, 0, 0);
    this->gy = this->my / 2;
    this->gx = this->mx / 2;
    mvwprintw(stdscr, my - 3, 3, "[%d|%d] [%d|%d] String status", my, mx, gy, gx);
    refresh();

}
void Gui::loop()
{
    status = 1;
    while (wgetch(input) != '\n') {
        if (status)
            tic_tac_toe();
        else
            menu();
    }
//    while (1) {}
}
void Gui::menu()
{

}

void Gui::tic_tac_toe()
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (play.area[i][j] == 0) {
                paint(j + 2, i, 0);
            }
            else if (play.area[i][j] == 1) {
                paint(j + 2, i, 1);
            }
            else {
                paint(j + 2, i, 2);
            }
            wrefresh(area);
        }
    }
}
void Gui::paint(int x, int y, int c)
{
    int size = 5;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            mvwaddch(area, y * size + i, x * size + j, FIGURE[c][i][j]);
        }
    }
}

