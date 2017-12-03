#include "Gui.h"

Gui::Gui() {
    initscr();
    getmaxyx(stdscr, this->my, this->mx);
    box(stdscr, 0, 0);
    this->gy = this->my / 2;
    this->gx = this->mx / 2;
    mvwprintw(stdscr, my - 3, 3, "%d|%d String status", my, mx);
    refresh();

}

void Gui::initMainWindow() {
    initGameWindow();
    initChatOutWindow();
    initChatInWindow();

    getch();

}

void Gui::initGameWindow() {
    game = newwin(1 + my - (gy / 2) , gx, 1, 1);
    box(game, 0, 0);
    wrefresh(game);

}

void Gui::initChatInWindow() {
    chat_in = newwin(5, mx - gx - 2, my - (gy - 3), gx + 1);
    box(chat_in, 0, 0);
    wrefresh(chat_in);

}

void Gui::initChatOutWindow() {
    chat_out = newwin(my - (gy - 2) , mx - gx - 2, 1, gx + 1);
    box(chat_out, 0, 0);
    wrefresh(chat_out);

}

Gui::~Gui() {
    delwin(game);
    delwin(chat_out);
    delwin(chat_in);
    endwin();

}
