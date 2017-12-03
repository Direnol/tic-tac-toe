#ifndef TIC_TAC_TOE_GUI_H
#define TIC_TAC_TOE_GUI_H

#include "../Client/ClientChat.h"
#include <ncurses.h>
#include <form.h>

/* TODO: Description ncurses text-style
    A_NORMAL             //- обычный режим
    A_STANDOUT           //- самое яркое выделение из всех возможных
    A_UNDERLINE          //- подчёркивание
    A_BLINK              //- мигание
    A_DIM                //- тусклый символ
    A_BOLD               //- выделение жирным шрифтом
*/

// TODO: Description GUI for client
class Gui : public ClientChat {
private:
    enum TextFormat {

    };
public:
    Gui();

    ~Gui() override;

    void Init();

    void textPrint();
};


#endif //TIC_TAC_TOE_GUI_H
