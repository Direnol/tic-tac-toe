#include "Gui.h"

Gui::Gui() {
    InitAllWin();
    status = 0;
    cur = 0;
    initMainWindow();
}

void Gui::initMainWindow() {
    initGameWindow();
    initChatOutWindow();
    initChatInWindow();
}

void Gui::initGameWindow() {
    game = newwin(1 + my - (gy / 2), gx, 1, 1);
    box(game, 0, 0);
    area = derwin(game, getmaxy(game) - 2, getmaxx(game) - 2, 1, 1);
    wrefresh(game);
}

void Gui::initChatInWindow() {
    int input_area = my - (gy * 2 - gy * 1 / 2);
    chat_in_box = newwin((input_area) < 7 ? input_area - 1 : input_area, mx - gx - 2, my - (gy - 3), gx + 1);
    box(chat_in_box, 0, 0);
    wrefresh(chat_in_box);
    input_chat = derwin(chat_in_box, getmaxy(chat_in_box) - 2, getmaxx(chat_in_box) - 2, 1, 1);

}

void Gui::initChatOutWindow() {
    chat_out_box = newwin(my - (gy - 2), mx - gx - 2, 1, gx + 1);
    box(chat_out_box, 0, 0);
    wrefresh(chat_out_box);
    output_chat = derwin(chat_out_box, getmaxy(chat_out_box) - 2, getmaxx(chat_out_box) - 2, 1, 1);
    scrollok(output_chat, true);
}

Gui::~Gui() {
    DelWin();
    if (sock != -1) close(sock);
    sock = -1;
    tid.join();
}

void Gui::repaint() {
    clear();
    DelWin();
    InitAllWin();
    initMainWindow();
}

void Gui::DelWin() {
    clear();
    delwin(input_chat);
    delwin(area);
    delwin(game);
    delwin(chat_out_box);
    delwin(chat_in_box);
    endwin();
}

void Gui::InitAllWin() {
    initscr();
    getmaxyx(stdscr, this->my, this->mx);
    noecho();
    keypad(stdscr, true);
    halfdelay(1);
    box(stdscr, 0, 0);
    this->gy = this->my / 2;
    this->gx = this->mx / 2;
    mvwprintw(stdscr, my - 3, 3, "[%d|%d] [%d|%d] String status", my, mx, gy, gx);
    refresh();
}

void Gui::menu() {
    wclear(area);
    mvwaddstr(area, 0, 0, "F2 - new game\n"
            "F3 - connect to game\n"
            "F4 - print list of games\n"
        "F5 - exit from game(autolose)\n"
        "Your name is "
    );
    waddstr(area, player_name.data());
    wrefresh(area);
}

// Отрисовка Х либо О на поле
void Gui::tic_tac_toe() {
    wclear(area);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (play.area[i][j] == 0) {
                if (play.i == i && play.j == j)
                    paint(j + 2, i, 0, 1);
                else
                    paint(j + 2, i, 0);
            } else if (play.area[i][j] == 1) {
                if (play.i == i && play.j == j)
                    paint(j + 2, i, 1, 1);
                else
                    paint(j + 2, i, 1);
            } else {
                if (play.i == i && play.j == j)
                    paint(j + 2, i, 2, 1);
                else
                    paint(j + 2, i, 2);
            }
            wrefresh(area);
        }
    }
}


void Gui::printMessage(char *s, int _my)
{
    int y = 0, x = 0;
    waddstr(output_chat, s);
    getyx(output_chat, y, x);
    if (y == _my - 1) {
        wscrl(output_chat, 3);
        wmove(output_chat, y - 2, 0);
    } else {
        wmove(output_chat, y + 1, 0);
    }
    wrefresh(output_chat);
}


void Gui::paint(int x, int y, int sybmol, int type_char) {
    int size = 5;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            mvwaddch(area, // для какого окна
                     y * size + i, x * size + j, // координаты вывода
                     FIGURE[sybmol][i][j] | ((type_char == 1) ? A_STANDOUT : 0) // что выводим
            );
        }
    }
}
