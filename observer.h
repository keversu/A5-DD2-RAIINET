#ifndef _OBSERVER_H_
#define _OBSERVER_H_
#include "game.h"
#include "studio.h"
#include <iostream>

class Observer {
 protected: 
      Studio* s;
 public:
  Observer(Studio* s) : s{s} {}
  virtual void notify() = 0;
  virtual ~Observer() = default;
};

#endif
