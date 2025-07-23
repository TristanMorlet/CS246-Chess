// THIS IS A TEMPORARY IMPLEMENTATION TO TEST OUTPUT, LATER WE WILL NEED TO CHANGE THIS TO ACCOUNT FOR BOARD BEING OWNED BY GAME CLASS SO CONTROLLER WONT OWN A BOARD, RATHER A GAME


#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../Model/game.h"
#include "../View/textview.h"
#include "../View/graphicalview.h" 
#include <iostream>
#include <string>
#include <memory>

class Controller {
    // temp owns board
    private:
        Game game;
        Coordinate parseCoordinate(const std::string& s);
        void enterSetupMode(bool& gameInProgress, std::unique_ptr<TextView>& tv, std::unique_ptr<GraphicalView>& gv);
        void handleAiTurn(bool& gameInProgress);

    // later code
    // Game game;
    public:
        void run();

        // later
        // void game();
};

#endif
