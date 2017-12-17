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

void Gui::loop() {
    chtype c = 0;
    work = true;
    start_chat();

    if (status) {
        tic_tac_toe();
    } else {
        menu();
    }

    bool game_flag = 0;

    while (c != KEY_F(10) && work) {
        game_flag = 0;
        (cur ? touchwin(area) : touchwin(input_chat));
        c = static_cast<chtype>(getch());
        if (c == ERR) continue;
        if (c == '\t') {
            if (status) cur ^= 1;
            continue;
        }

        // TODO key handler for F1, F2, F3

        switch (c) {
            case KEY_F(2): {
                createGame();
                game_flag = 1;
                break;
            }
            case KEY_F(3): {
                connectToGame();
                game_flag = 1;
                break;
            }
            case KEY_F(4): {
                serverList();
                game_flag = 1;
                break;
            }
            default:
                break;
        }

        if (game_flag) continue;

        if (cur == 0) {
            keymap_chat(c);
            wrefresh(input_chat);
        } else {
            pressedKey(c);
            tic_tac_toe();
            wrefresh(area);
        }
    }
    work = false;
    messageSend(const_cast<char *>("/sgout"), strlen("/sgout"));
    sleep(1);
}

void Gui::menu() {
    wclear(area);

    mvwaddstr(area, 0, 0, "F2 - new game\n"
            "F3 - connect to game\n"
            "F4 - print list of games\n"
    );

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

int Gui::pressedKey(unsigned int key) {
    switch (key) {
        case '\n': {
            if (play.area[play.i][play.j] == 2) {
                play.area[play.i][play.j] = static_cast<char>(play.figure);
                // send change
            }
            break;
        }
        case KEY_UP: {
            if (play.i <= 0) break;
            play.i--;
            break;
        }
        case KEY_LEFT: {
            if (play.j <= 0) break;
            play.j--;
            break;
        }
        case KEY_RIGHT: {
            if (play.j >= 2) break;
            play.j++;
            break;
        }
        case KEY_DOWN: {
            if (play.i >= 2) break;
            play.i++;
            break;
        }
        default:
            break;
    }

    return 0;
}

void Gui::keymap_chat(chtype c) {
    if (c == '\n') {
        this->SendMessage();
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
                wmove(input_chat, y - 1, mx - 1);
            } else {
                wmove(input_chat, y, x - 1);
            }
            break;
        }
        case (KEY_RIGHT): {
            if (chat.pos == chat.max) break;
            ++chat.pos;
            if (x == mx - 1) {
                wmove(input_chat, y + 1, 0);
            } else {
                wmove(input_chat, y, x + 1);
            }
            break;
        }
        case KEY_UP:
        case KEY_DOWN:
            break;
        case KEY_BACKSPACE:
        case 127: {
            if (chat.pos == 0) break;
            --chat.pos;
            --chat.max;
            if (x == 0) {
                wmove(input_chat, y - 1, mx - 1);
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

Gui::Gui(char *ip, uint16_t port) : ClientChat(ip, port) {
    nameSend();
    InitAllWin();
    status = 0;
    cur = 0;
    initMainWindow();
}

void Gui::start_chat() {
    tid = thread(&Gui::RecvMessage, this);
}

void Gui::SendMessage() {
    char message[BUFFER_SIZE];
    mvwinstr(input_chat, 0, 0, message);
    size_t n = rtimr(message);
    messageSend(message, n);
    wclear(input_chat);
    chat.pos = chat.max = 0;
}

void Gui::RecvMessage() {
    msg pmsg{};
    int mx, my, x, y;
    getmaxyx(output_chat, my, mx);
    while (work) {
        messageRecv(&pmsg);
        if (pmsg.code < 0) {
            work = false;
            waddstr(output_chat, "!!! ");
            break;
        }
        switch (pmsg.code) {
            case INIT: {
                int *init = reinterpret_cast<int *>(pmsg.message);
                this->play.figure = init[0]; // figure
                this->play.cur_player = init[1]; // cur player
                status = 1;
                break;
            }
            case GAME: {
                break;
            }
            case PROCESS_GAME: {
                break;
            }
            case FINISH_GAME: {
                break;
            }
            default:
                waddstr(output_chat, pmsg.message);
                getyx(output_chat, y, x);
                if (y == my - 1) {
                    wscrl(output_chat, 3);
                    wmove(output_chat, y - 2, 0);
                } else {
                    wmove(output_chat, y + 1, 0);
                }
                wrefresh(output_chat);
        }
    }
    waddstr(output_chat, "Disconnect from server");
    wrefresh(output_chat);
}

size_t Gui::rtimr(char *s) {
    size_t n = strlen(s);
    for (size_t i = n - 1; i >= 0 && s[i] == ' '; --i, --n) s[i] = '\0';
    return n;
}

void Gui::createGame() {
    messageSend(const_cast<char *>("/gcreate"), strlen("/gcreate"));
}

void Gui::connectToGame() {
    messageSend(const_cast<char *>("/gconnect"), strlen("/gconnect"));
}

void Gui::serverList() {
    // TODO: server's list
}
