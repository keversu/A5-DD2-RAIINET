#include "ability.h"
#include "link.h"
#include "game.h"

#include<iostream>

using namespace std;

void Ability::linkBoost(Link &l) {
  const int BOOST_STRENGTH = 1;
  l.setJumps(l.getJumps() + BOOST_STRENGTH);
}

void Ability::firewall(Player &p, Link &l) {
  if (l.isDataLink()) {
    l.reveal(true);
  } else {
    p.downloadLink(l);
  }
}

void Ability::download(Player &p, Link &l) {
  p.downloadLink(l);
}

void Ability::polarize(Link &l) {
  if (l.isDataLink()) {
    l.setLinkAsData(false);
  } else {
    l.setLinkAsData(true);
  }
}

void Ability::scan(Link &l) {
  l.reveal(true);
}

void Ability::exchange(Link &l1, Link &l2) {
  Link temp {l1};

  l1.setStrength(l2.getStrength());
  l1.setLinkAsData(l2.isDataLink());

  l2.setStrength(temp.getStrength());
  l2.setLinkAsData(temp.isDataLink());
}

void Ability::craze(Link &l) {
  const int BOOST_STRENGTH = 1;
  l.setStrength(l.getStrength() + BOOST_STRENGTH); 
}

void Ability::knockOut(Link &l) {
  l.setKnockedOut(true);
}

bool Ability::walls(const char c, Game &g){

  pair<int, int> pos = g.findLinkPosition(c);

  bool result = g.setWalls(c, pos.first, pos.second - 1, pos.second + 1);
  if(result)
  {
     linkWithWalls.push_back(c);
     return true;
  }

  return false;
}
