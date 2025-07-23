#include "../Controller/controller.h"
/*
FOR TREEE: g++20 main.cc ../Controller/controller.cc ../Model/game.cc ../Model/board.cc ../Model/strategy.cc ../Model/piece.cc ../Model/player.cc ../View/textview.cc 
X11: g++20 main.cc ../Controller/controller.cc ../Model/game.cc ../Model/board.cc ../Model/strategy.cc ../Model/piece.cc ../Model/player.cc ../View/textview.cc ../View/graphicalview.cc ../View/window.cc -o chess_game -lX11 -L/usr/X11/lib -I/usr/X11/include
*/
int main() {
    // 1. Create the Board. The constructor will set up the initial pieces.
    // Board b;

    // 2. Create the TextView, passing it a reference to the board.
    // TextView tv{b};

    // 3. Attach the view as an observer to the board.
    // b.attach(&tv);

    // 4. Manually trigger a notification to display the initial state.
    // b.notifyObservers();

    // swapping out above for the controller base,
    // we are simply using c.run, however later we will use c.play to play instead
    Controller c;
    c.run();

    return 0;
}
