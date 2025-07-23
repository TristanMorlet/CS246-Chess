#ifndef WINDOW_H
#define WINDOW_H

#include <X11/Xlib.h>
#include <iostream>
#include <string>

class Xwindow {
    Display *d;
    Window w;
    int s;
    GC gc;
    unsigned long colours[5];
    int width, height;

public:
    Xwindow(int width = 500, int height = 500);  // Constructor
    ~Xwindow();                              // Destructor

    enum {White = 0, Black, Red, Green, Blue}; // Available colours

    // Draws a rectangle
    void fillRectangle(int x, int y, int width, int height, int colour = Black);

    // Draws a string
    void drawString(int x, int y, std::string msg);

    // Getters for window dimensions
    int getWidth() const;
    int getHeight() const;
};

#endif
