#include "boardgraphics.h"
#include "studio.h"
#include "link.h"
#include <iostream>
#include <string>
#include <sstream>
#include <ostream>
#include <string>


const int a = 100;


// Constructor
BoardGraphics::BoardGraphics(Studio* s, int left)
    : Observer{s}, left{left}, w{(left + 2) * a + 20, left * a} {
    // Use a shared_ptr to manage this object
    s->attach(std::shared_ptr<Observer>(this, [](Observer*) {
        // Custom deleter: Do nothing because lifetime is managed externally
    }));
}

// Destructor
BoardGraphics::~BoardGraphics() {
    s->detach(std::shared_ptr<Observer>(this, [](Observer*) {
        // Custom deleter: Do nothing
    }));
}

void BoardGraphics::notify() { 

    ostringstream out;
    s->aa()->displayBoard(out);
    string line;
    int i = 0;
    int x = 20;
    istringstream in  {out.str()};
    w.clearRectangle(800, 0, 220, 400, 2);
    w.clearRectangle(800, 400, 220, 400, 4);
    while (std::getline(in, line)) {
        if (!(5 <= i && i <= 14))  w.drawString(840, 30 + i * 30 + x, line);
        if (i == 14) x = 155;
        i++;
        
    }

    for (int j = 0; j < 8; ++j){
        for (int i = 0; i < 8; ++i){
            char c = s->getState(j, i);
            char reveal = s->getDataorVirus(j, i);
            string p = std::to_string(c) + std::to_string(reveal);
            if (memo[j*8 + i] == p) continue;
            memo[j*8 + i] = p;
            std::string str(1, c);
            if ((i + j) % 2 == 0) {
                if (str != "+") w.fillRectangle(i*a, j*a, a, a, 3); 
                if (str != ".") {
                    if (str != "+") { 
                        w.fillRectangle(i*a + 23, j*a + 23, a/2 + 4, a/2 + 4, 1); 
                        if ("A" <= str && str <= "H") {
                            if(reveal == 'D')
                            {
                               w.fillRectangle(i*a + 26, j*a + 26, a/2 - 2, a/2 - 2, 5);
                            }
                            else if (reveal =='V')
                            {
                                w.fillRectangle(i*a + 26, j*a + 26, a/2 - 2, a/2 - 2, 6);
                            }
                            else
                            {
                                w.fillRectangle(i*a + 26, j*a + 26, a/2 - 2, a/2 - 2, 4);
                            }
                        } else if ("a" <= str && str <= "h") {
                            if(reveal == 'D')
                            {
                                w.fillRectangle(i*a + 26, j*a + 26, a/2 - 2, a/2 - 2, 5);
                            }
                            else if(reveal == 'V')
                            {
                               w.fillRectangle(i*a + 26, j*a + 26, a/2 - 2, a/2 - 2, 6);
                            }
                            else
                            {
                                w.fillRectangle(i*a + 26, j*a + 26, a/2 - 2, a/2 - 2, 2);
                            }
                        } else if (str == "S") {
                            if (j == 0) w.fillRectangle(i*a + 37, j*a + 37, a/4, a/4, 2); 
                            if (j == 7) w.fillRectangle(i*a + 37, j*a + 37, a/4, a/4, 4); 
                        }
                        else if(str == "m")
                        {
                           w.fillRectangle(i*a + 37, j*a + 37, a/4, a/4, 2); 
                        }
                        else if(str == "w")
                        {
                            w.fillRectangle(i*a + 37, j*a + 37, a/4, a/4, 4);
                        }
                        w.drawString(i*a + 47, j*a + 54, str);
                    } else {
                        w.fillRectangle(i*a, j*a, a, a, 7); 
                    }
                } 
            } else {
                if (str != "+") w.fillRectangle(i*a, j*a, a, a, 0); 
                if (str != ".") {
                    if (str != "+") { 
                        w.fillRectangle(i*a + 23, j*a + 23, a/2 + 4, a/2 + 4, 1); 
                        if ("A" <= str && str <= "H") {
                            if(reveal == 'D')
                            {
                               w.fillRectangle(i*a + 26, j*a + 26, a/2 - 2, a/2 - 2, 5);
                            }
                            else if (reveal =='V')
                            {
                                w.fillRectangle(i*a + 26, j*a + 26, a/2 - 2, a/2 - 2, 6);
                            }
                            else
                            {
                               w.fillRectangle(i*a + 26, j*a + 26, a/2 - 2, a/2 - 2, 4);
                            }
                        } else if ("a" <= str && str <= "h") {
                            if(reveal == 'D')
                            {
                               w.fillRectangle(i*a + 26, j*a + 26, a/2 - 2, a/2 - 2, 5);
                            }
                            else if (reveal =='V')
                            {
                                w.fillRectangle(i*a + 26, j*a + 26, a/2 - 2, a/2 - 2, 6);
                            }
                            else
                            {
                              w.fillRectangle(i*a + 26, j*a + 26, a/2 - 2, a/2 - 2, 2);
                            }
                        } else if (str == "S") {
                            if (j == 0) w.fillRectangle(i*a + 37, j*a + 37, a/4, a/4, 2); 
                            if (j == 7) w.fillRectangle(i*a + 37, j*a + 37, a/4, a/4, 4); 
                        }
                        else if(str == "m")
                        {
                            w.fillRectangle(i*a + 37, j*a + 37, a/4, a/4, 2); 
                        }
                        else if(str == "w")
                        {
                            w.fillRectangle(i*a + 37, j*a + 37, a/4, a/4, 4);
                        }
                        w.drawString(i*a + 47, j*a + 54, str);
                    } else {
                        w.fillRectangle(i*a, j*a, a, a, 7); 
                    }
                } 
            }
        }
    }
}


