// THIS IS A TEMPORARY IMPLEMENTATION TO TEST OUTPUT, LATER WE WILL NEED TO CHANGE THIS TO ACCOUNT FOR BOARD BEING OWNED BY GAME CLASS SO CONTROLLER WONT OWN A BOARD, RATHER A GAME


#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../Model/game.h"
#include <iostream>
#include <string>

class Controller {
    // temp owns board
    private:
        Game game;
        Coordinate parseCoordinate(const std::string& s);

    // later code
    // Game game;
    public:
        void run();

        // later
        // void game();
};

#endif
