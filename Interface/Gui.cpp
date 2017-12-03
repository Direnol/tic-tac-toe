#include "Gui.h"

Gui::Gui()
{
    InitAllWin();
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
    wrefresh(game);
}

void Gui::initChatInWindow()
{
    int input_area = my - (gy * 2 - gy * 1 / 2);
    chat_in = newwin((input_area) < 7 ? input_area - 1 : input_area, mx - gx - 2, my - (gy - 3), gx + 1);
    box(chat_in, 0, 0);
    wrefresh(chat_in);

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
    DelWin();
    InitAllWin();
    initMainWindow();
}

void Gui::DelWin()
{
    delwin(game);
    delwin(chat_out);
    delwin(chat_in);
    endwin();

}
void Gui::InitAllWin()
{
    initscr();
    getmaxyx(stdscr, this->my, this->mx);
    box(stdscr, 0, 0);
    this->gy = this->my / 2;
    this->gx = this->mx / 2;
    mvwprintw(stdscr, my - 3, 3, "[%d|%d] [%d|%d] String status", my, mx, gy, gx);
    refresh();

}
void Gui::loop()
{
    while (getch() != '\n') {}
//    while (1) {}
}

