#include "Gui.h"

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

void Gui::loop() {
    chtype c = 0;
    work = true;
    start_chat();
    menu();
    bool game_flag;

    while (c != KEY_F(10) && work) {
        game_flag = false;
        c = static_cast<chtype>(getch());
        if (c == ERR) continue;

        if (c == '\t') {
            if (status) cur ^= 1;
            continue;
        }
        (cur ? touchwin(area) : touchwin(input_chat));
        if (status) {
            tic_tac_toe();
        } else {
            menu();
        }

        // TODO key handler for F4, F5

        switch (c) {
            case KEY_F(2): {
                createGame();
                game_flag = true;
                break;
            }
            case KEY_F(3): {
                connectToGame();
                game_flag = true;
                break;
            }
            case KEY_F(4): {
                serverList();
                game_flag = true;
                break;
            }
            case KEY_F(5): {
                autoLose();
                game_flag = true;
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

int Gui::pressedKey(unsigned int key) {
    switch (key) {
        case '\n': {
            if (play.area[play.i][play.j] == 2 && play.figure == play.cur_player) {
                play.area[play.i][play.j] = static_cast<char>(play.figure);
                sendGameProcess();
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

void Gui::SendMessage() {
    char message[BUFFER_SIZE];
    mvwinstr(input_chat, 0, 0, message);
    size_t n = rtrim(message);
    int _mx, _my;
    getmaxyx(output_chat, _my, _mx);
    char buf[player_name.size() + strlen(message) + 2];
    sprintf(buf, "%s:%s", player_name.data(), message);
    printMessage(buf, _my);
    messageSend(message, n);
    wclear(input_chat);
    chat.pos = chat.max = 0;

}

void Gui::RecvMessage() {
    msg pmsg{};
    int mx, my;
    auto *game_struct = reinterpret_cast<tic_tac *>(pmsg.message);
    while (work) {
        messageRecv(&pmsg);
        getmaxyx(output_chat, my, mx);
        if (pmsg.code < 0) {
            work = false;
            waddstr(output_chat, "!!! ");
            break;
        }
        switch (pmsg.code) {
            case INIT: {
                auto *init = reinterpret_cast<int *>(pmsg.message);
                this->play.figure = init[0]; // figure
                this->play.cur_player = init[1]; // cur player
                status = 1;
                cur ^= 1;
                tic_tac_toe();
                break;
            }
            case GAME: {
                // TODO FAILED END
                break;
            }
            case PROCESS_GAME: {
                for (int i = 0; i < 3; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        play.area[i][j] = game_struct->area[i][j];
                    }
                }
                play.cur_player = game_struct->cur_player;
                tic_tac_toe();
                break;
            }
            case FINISH_GAME: {
                status = 0;
                menu();
                cur = 0;
                game_struct->figure = play.figure;
                send(sock, &pmsg, sizeof(msg), 0);
                play = tic_tac();
                break;
            }
            default:
                printMessage(pmsg.message, my);
        }
    }
    waddstr(output_chat, "Disconnect from server");
    wrefresh(output_chat);
}

size_t Gui::rtrim(char *s) {
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

void Gui::sendGameProcess()
{
    msg pmsg{};
    pmsg.code = PROCESS_GAME;
    auto *game_struct = reinterpret_cast<tic_tac *>(pmsg.message);
    memcpy(game_struct, &play, sizeof(msg));
    send(sock, &pmsg, sizeof(msg), 0);
    play.cur_player ^= 1;
    sleep(1);
}

void Gui::autoLose()
{

}
