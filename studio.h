#ifndef STUDIO_H
#define STUDIO_H
#include <iostream>
#include "subject.h"

class Game;

class Studio : public Subject{
  Game *a;

 public:
  explicit Studio( Game* aa ): a{aa} {}

  Game*& aa() { return a; }

  void render();

  char getState(int row, int col) const;

  char getDataorVirus(int row, int col) const;

  

  virtual ~Studio() = default;
};

#endif
