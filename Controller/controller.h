#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../Model/game.h"
#include "../View/textview.h"
#include "../View/graphicalview.h" 
#include <iostream>
#include <string>
#include <memory>

class Controller {
    private:
        Game game;
        Coordinate parseCoordinate(const std::string& s);
        void enterSetupMode(bool& gameInProgress, std::unique_ptr<TextView>& tv, std::unique_ptr<GraphicalView>& gv);
        void handleAiTurn(bool& gameInProgress);
        void checkGameState(bool& gameInProgress);
    public:
        void run();
};

#endif
