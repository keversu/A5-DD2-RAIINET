#ifndef ABILITY_H
#define ABILITY_H

#include <vector>
#include "link.h"
#include "player.h"

class Game;

using namespace std;

class Ability {
  public:
    void linkBoost(Link &l);
    void polarize(Link &l);
    void firewall(Player &p, Link &l);
    void download(Player &p, Link &l);
    void scan(Link &l);
    void exchange(Link &l1, Link &l2);
    void craze(Link &l);
    bool walls(const char c, Game &g);
    vector <char> linkWithWalls;
    void knockOut(Link &l);
};

#endif
