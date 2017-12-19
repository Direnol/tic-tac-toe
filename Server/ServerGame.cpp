#include "ServerChat.h"

bool ServerChat::getWinner(tic_tac *game, int &who)
{
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

void ServerChat::switchGame(int &status, msg *pmsg, int player, string &name)
{
    status ^= 1;
    string buffer = "Nobody created game";
    if (strcmp("create", pmsg->message) == 0) {
    create:
        qgame.emplace_back(player);
        pmsg->code = ALL;
        buffer = name + " has created game!";
        logging(buffer);
        strcpy(pmsg->message, buffer.data());
        for (auto &it : players) {
            send(it.second.first, pmsg, sizeof(msg), 0);
        }
        strcpy(pmsg->message, "Your figure is (X)");
        send(player, pmsg, sizeof(msg), 0);
        logging(name + " created game");
    } else if (strcmp("connect", pmsg->message) == 0) {
        pmsg->code = ALL;
        if (qgame.empty()) {
            strcpy(pmsg->message, buffer.data());
            send(player, pmsg, sizeof(msg), 0);
            goto create;
        }
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
        pmsg->code = ALL;
        strcpy(pmsg->message, buf.data());
        send(player, pmsg, sizeof(msg), 0);
        send(alien_fd, pmsg, sizeof(msg), 0);
    }
}

void ServerChat::switchFinishGame(int &status, msg *pmsg, int player, string &name, tic_tac *game_struct)
{
    status ^= 1;
    players[name].second = -1;
    pmsg->code = ALL;
    string logger;
    if (game_struct->winner == 2) {
        strcpy(pmsg->message, "Nobody win");
        send(player, pmsg, sizeof(msg), 0);
        logger = "Nobody win from " + name;
    } else if (game_struct->winner == game_struct->figure) {
        strcpy(pmsg->message, "You are win");
        send(player, pmsg, sizeof(msg), 0);
        logger = name + " is win";
    } else {
        strcpy(pmsg->message, "You are lose");
        send(player, pmsg, sizeof(msg), 0);
        logger = name + " is lose";
    }
    logging(logger);
}

void ServerChat::switchProcessGame(int &status, msg *pmsg, int player, string &name, tic_tac *game_struct)
{
    int winner;
    game_struct->cur_player ^= 1;
    if (getWinner(game_struct, winner)) {
        pmsg->code = FINISH_GAME;
        game_struct->winner = winner;
        game_struct->cur_player = -1;
        send(players[name].second, pmsg, sizeof(msg), 0);
        send(player, pmsg, sizeof(msg), 0);
        return;
    }
    string logger = name + "put " + (game_struct->figure == 0 ? "[X] to (" : "[0] to (") +
        to_string(game_struct->i) + ',' + to_string(game_struct->j) + ')';
    logging(logger);
    send(players[name].second, pmsg, sizeof(msg), 0);
    send(player, pmsg, sizeof(msg), 0);
    pmsg->code = ALL;
    strcpy(pmsg->message, "Your turn");
    send(players[name].second, pmsg, sizeof(msg), 0);
}