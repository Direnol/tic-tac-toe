#include "ClientChat.h"
#include "../Interface/Gui.h"

Gui *gui;

void resize_term(int sig)
{
    winsize w = {};
    w.ws_col = 80;
    w.ws_row = 25;
    ioctl(0, TIOCSWINSZ, &w);
    signal(SIGWINCH, SIG_IGN);
    gui->repaint();
    signal(SIGWINCH, resize_term);
}

int main()
{
    gui = new Gui(const_cast<char *>("192.168.1.6"), 6969);
    signal(SIGWINCH, resize_term);
    gui->loop();

    delete (gui);
    return 0;
}