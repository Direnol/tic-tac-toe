#include "ServerChat.h"

bool ServerChat::getWinner(tic_tac *game, int &who)
{
    // TODO need correct check: who win or nobody win
    bool win = false;
    int c1 = 1, c2 = 1;
    for (auto &i : game->area) {
        for (int j = 1; j < 3; ++j) {
            if (i[j] == 0 && i[j - 1] == i[j]) ++c1;
            if (i[j] == 1 && i[j - 1] == i[j]) ++c2;
        }
        if (c1 == 3) {
            who = 0;
            win = true;
            break;
        }
        if (c2 == 3) {
            who = 1;
            win = true;
            break;
        }
        c1 = c2 = 1;
    }
    if (win) return win;

    for (int j = 0; j < 3; ++j) {
        for (int i = 1; i < 3; ++i) {
            if (game->area[i][j] == 0 && game->area[i - 1][j] == game->area[i][j]) ++c1;
            if (game->area[i][j] == 1 && game->area[i - 1][j] == game->area[i][j]) ++c2;
        }
        if (c1 == 3) {
            who = 0;
            win = true;
            break;
        }
        if (c2 == 3) {
            who = 1;
            win = true;
            break;
        }
        c1 = c2 = 1;
    }
    if (win) return win;
    for (int i = 1, j = 1; i < 3; ++j, ++i) {
        if (game->area[i][j] == 0 && game->area[i - 1][j - 1] == game->area[i][j]) ++c1;
        if (game->area[i][j] == 1 && game->area[i - 1][j - 1] == game->area[i][j]) ++c2;

    }
    if (c1 == 3) {
        who = 0;
        win = true;
    }
    if (c2 == 3) {
        who = 1;
        win = true;
    }
    if (win) return win;
    c1 = c2 = 1;
    for (int i = 2, j = 2; i > 0; --j, --i) {
        if (game->area[i][j] == 0 && game->area[i - 1][j - 1] == game->area[i][j]) ++c1;
        if (game->area[i][j] == 1 && game->area[i - 1][j - 1] == game->area[i][j]) ++c2;
    }
    if (c1 == 3) {
        who = 0;
        win = true;
    }
    if (c2 == 3) {
        who = 1;
        win = true;
    }
    if (win) return win;
    c1 = 0;
    for (auto &i : game->area) {
        for (char j : i) {
            if (j == 2) ++c1;
        }
    }
    if (c1 == 0) {
        win = true;
        who = 2;
    }
    return win;
}

int ServerChat::listen_connect(int player)
{
    void *ptr = new char[BUFFER_SIZE];
    auto *pch = static_cast<char *>(ptr);
    auto *pmsg = static_cast<msg *>(ptr);
    int status = 0;
    ssize_t res;
    do {
        res = static_cast<int>(recv(player, ptr, BUFFER_SIZE, 0));
        if (res <= 0) {
            close(player);
            return 0;
        }
        pch[res] = '\0';
        if (players.find(string(pch)) == players.end()) {
            players.insert(make_pair(string(pch), make_pair(player, -1)));
            this->sockets.insert(make_pair(player, string(pch)));
            send(player, "OK", strlen("OK"), 0);
            break;
        }
        send(player, "NO", strlen("NO"), 0);
    } while (true);

    string name(pch);
    logging(name + " has been conected");
    auto *game_struct = reinterpret_cast<tic_tac *>(pmsg->message);
    while (work) {
        res = recv(player, ptr, BUFFER_SIZE, 0);
        if (res <= 0) {
            goto end;
        }
        switch (pmsg->code) {
            case (ALL): {
                string tmp(name);
                tmp += ":" + string(pmsg->message);
                strcpy(pmsg->message, tmp.c_str());
                logging(tmp);
                for (auto &it : players) {
                    if (it.second.first == player) continue;
                    send(it.second.first, pmsg, sizeof(msg), 0);
                }
                break;
            }
            case (GAME) : {
                status ^= 1;
                if (strcmp("create", pmsg->message) == 0) {
                    string buffer = name + " has created game!";
                    qgame.emplace_back(player);
                    pmsg->code = ALL;
                    logging(buffer);
                    strcpy(pmsg->message, buffer.data());

                    for (auto &it : players) {
                        // if (it.second.first == player) continue;
                        send(it.second.first, pmsg, sizeof(msg), 0);
                    }
                    strcpy(pmsg->message, "Your figure is (X)");
                    send(player, pmsg, sizeof(msg), 0);

                    logging(name + " created game");
                } else if (strcmp("connect", pmsg->message) == 0) {
                    pmsg->code = ALL;
                    strcpy(pmsg->message, "Your figure is (0)");
                    send(player, pmsg, sizeof(msg), 0);
                    strcpy(pmsg->message, "Your turn");
                    int alien_fd = qgame.front(); // get alien fd
                    send(alien_fd, pmsg, sizeof(msg), 0);
                    qgame.pop_front(); // delete from queue game
                    string alien_name = sockets[alien_fd]; // get alien's name
                    players[name].second = alien_fd; // say, that i have alien alien_fd
                    players[alien_name].second = player; // say, that alien has alient me
                    pmsg->code = INIT;
                    auto *init = reinterpret_cast<int *>(pmsg->message);
                    init[0] = 1; // figure
                    init[1] = 0; // first step
                    send(player, pmsg, sizeof(msg), 0);
                    init[0] = 0;
                    send(alien_fd, pmsg, sizeof(msg), 0);
                    string buf = name + string("(0) vs ") + alien_name + "(X)";
                    logging(buf);


                    //start game
                    // send to me
                    // send to alien

                }

                break;
            }
            case (OPTIONS): {
                if (strcmp("out", pmsg->message) == 0) {
                    goto end;
                } else if (strcmp("kill", pmsg->message) == 0) {
                    work = false;
                    if (sock != -1) close(sock);
                    sock = -1;
                    logging("Signal SERVER_OFF");
                    // TODO: off server
                }
                break;
            }
            case (PROCESS_GAME) : {
                int winner;
                game_struct->cur_player ^= 1;
                if (getWinner(game_struct, winner)) {
                    pmsg->code = FINISH_GAME;
                    game_struct->winner = winner;
                    game_struct->cur_player = -1;
                    send(players[name].second, pmsg, sizeof(msg), 0);
                    send(player, pmsg, sizeof(msg), 0);
                    break;
                }
                send(players[name].second, pmsg, sizeof(msg), 0);
                send(player, pmsg, sizeof(msg), 0);
                pmsg->code = ALL;
                strcpy(pmsg->message, "Your turn");
                send(players[name].second, pmsg, sizeof(msg), 0);
                break;
            }
            case FINISH_GAME: {
                status ^= 1;
                players[name].second = -1;
                pmsg->code = ALL;
                if (game_struct->winner == 2) {
                    strcpy(pmsg->message, "Nobody win");
                    send(player, pmsg, sizeof(msg), 0);
                } else if (game_struct->winner == game_struct->figure) {
                    strcpy(pmsg->message, "You are win");
                    send(player, pmsg, sizeof(msg), 0);
                } else {
                    strcpy(pmsg->message, "You are lose");
                    send(player, pmsg, sizeof(msg), 0);
                }

                break;
            }
            default: {
                cout << "Default switch find " << pmsg->code << endl;
                break;
            }
        }
    }
    end:
    delete (char *) ptr;
    players.erase(name);
    if (sockets.find(player) != sockets.end()) {
        close(player);
        sockets.erase(player);
    }
    logging(name + " has been disconect");
    return 0;
}
