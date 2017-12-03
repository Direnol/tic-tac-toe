#include "ClientChat.h"
#include "../Interface/Gui.h"

Gui *gui;

void resize_term(int sig)
{
    signal(SIGWINCH, SIG_IGN);
    gui->repaint();
    signal(SIGWINCH, resize_term);
}

int main()
{
    gui = new Gui;
    signal(SIGWINCH, resize_term);
    gui->loop();

    delete(gui);
    return 0;
}