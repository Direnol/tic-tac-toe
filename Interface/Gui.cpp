#include "Gui.h"

Gui::Gui()
{
    InitAllWin();
    status = 1;
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
    chat_in_box = newwin((input_area) < 7 ? input_area - 1 : input_area, mx - gx - 2, my - (gy - 3), gx + 1);
    box(chat_in_box, 0, 0);
    wrefresh(chat_in_box);
    input_chat = derwin(chat_in_box, getmaxy(chat_in_box) - 2, getmaxx(chat_in_box) - 2, 1, 1);

}

void Gui::initChatOutWindow()
{
    chat_out_box = newwin(my - (gy - 2), mx - gx - 2, 1, gx + 1);
    box(chat_out_box, 0, 0);
    wrefresh(chat_out_box);
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
    delwin(input_chat);
    delwin(area);
    delwin(game);
    delwin(chat_out_box);
    delwin(chat_in_box);
    endwin();

}
void Gui::InitAllWin()
{
    initscr();
    getmaxyx(stdscr, this->my, this->mx);
    noecho();
    keypad(stdscr, TRUE);
    box(stdscr, 0, 0);
    this->gy = this->my / 2;
    this->gx = this->mx / 2;
    mvwprintw(stdscr, my - 3, 3, "[%d|%d] [%d|%d] String status", my, mx, gy, gx);
    refresh();

}
void Gui::loop()
{
    int cur = 0;
    chtype c;
    while ((c = static_cast<chtype>(getch())) != KEY_F(10)) {
        if (status)
            tic_tac_toe();
        else
            menu();
        if (c == '\t') {
            cur ^= 1;
            continue;
        }
        if (cur == 0) {
            keymap_chat(c);
        } else {
            //
        }
    }
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
void Gui::keymap_chat(chtype c)
{
    if (c == '\n') {
        // TODO: send

        return;
    }
    int mx, my, x, y;
    getmaxyx(input_chat, my, mx);
    getyx(input_chat, y, x);
    switch (c) {
        case (KEY_LEFT): {
            if (chat.pos == 0) break;
            --chat.pos;
            if (x == 0) {
                wmove(input_chat, y - 1, mx);
            } else {
                wmove(input_chat, y, x - 1);
            }
            break;
        }
        case (KEY_RIGHT): {
            if (chat.pos == chat.max) break;
            ++chat.pos;
            if (x == mx) {
                wmove(input_chat, y + 1, 0);
            } else {
                wmove(input_chat, y, x + 1);
            }
            break;
        }
        case 127: {
            if (chat.pos == 0) break;
            --chat.pos;
            if (x == 0) {
                wmove(input_chat, y - 1, mx);
            } else {
                wmove(input_chat, y, x - 1);
            }
            wdelch(input_chat);
            break;
        }
        default: {
            if (x == mx && y == mx) break;
            if (chat.pos == chat.max) ++chat.max;
            ++chat.pos;
            waddch(input_chat, c);
        }
    }
}

