#ifndef BOARDGRAPHICS_H
#define BOARDGRAPHICS_H
#include "observer.h"
#include "game.h"
#include "window.h"
#include "subject.h"
#include "studio.h"
#include <iostream>
#include <string>

class BoardGraphics: public Observer {
 
 public:

    BoardGraphics(Studio* s, int left);
    ~BoardGraphics();
    void notify();
private:
    int left;
    string memo[64];
    Xwindow w;
    
};

#endif
