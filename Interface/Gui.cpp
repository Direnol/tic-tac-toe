#include "Gui.h"

Gui::Gui() {
    initscr();
    cout << "Gui has been init" << endl;
    refresh();
}

void Gui::Init() {

}

Gui::~Gui() {
    endwin();
}

void Gui::textPrint() {

}
