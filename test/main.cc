#include "../Model/board.h"
#include "../View/textview.h"

int main() {
    // 1. Create the Board. The constructor will set up the initial pieces.
    Board b;

    // 2. Create the TextView, passing it a reference to the board.
    TextView tv{b};

    // 3. Attach the view as an observer to the board.
    b.attach(&tv);

    // 4. Manually trigger a notification to display the initial state.
    b.notifyObservers();

    return 0;
}