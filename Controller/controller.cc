#include "controller.h"
#include "../Model/board.h"
#include "../View/textview.h"
#include "../types/types.h" // Assuming you have a types.h for Colour, Coordinate etc.
#include <sstream>

using namespace std;

void Controller::run() {
    Board b;
    TextView tv{b};
    b.attach(&tv);
    b.notifyObservers();
}