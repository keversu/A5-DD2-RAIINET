#include "player.h"
#include <iostream>
#include <ostream>
#include <iomanip>

// Constructor
Player::Player()
    : downloadedData(0), downloadedViruses(0), abilityAvailable{true} {
}

// Accessors
int Player::getDownloadedData() const {
    return downloadedData;
}

int Player::getDownloadedViruses() const {
    return downloadedViruses;
}

const std::vector<Link>& Player::getLinks() const {
    return links;
}

int Player::getAbilityUsage(const char& ability) const {
    if (abilities.find(ability) != abilities.end())
        return abilities.at(ability);
    return 0;
}

bool Player::getAbilityAvailable() const {
    return abilityAvailable;
}

// Mutators
void Player::addDownloadedData() {
    ++downloadedData;
}

void Player::addDownloadedVirus() {
    ++downloadedViruses;
}

void Player::decrementAbilityUsage(const char& ability) {
    if (abilities.find(ability) != abilities.end() && abilities[ability] > 0) {
        --abilities[ability];
    }
}

void Player::incrementAbilityUsage(const char& ability) {
    if (abilities.find(ability) != abilities.end()) {
        ++abilities[ability];
    }
}

void Player::addLink(const Link& link) {
    links.push_back(link);
}

void Player::setAbilityAvailable(const bool b) {
    abilityAvailable = b;
}

bool Player::hasAbility(const char& ability) const {
    return abilities.find(ability) != abilities.end() && abilities.at(ability) > 0;
}

void Player::displayLinks(std::ostream &out, bool isPlayer1Turn, int player) const {
    int count = 0;
   if(isPlayer1Turn && player==2)
   {
     for (const Link& link : links) {
        if(link.isRevealed())
        {
            out << link.display(true) << " ";
        }
        else
        {
            out << link.display(false) << " ";
        }
        count++;
        if (count == 4) out << std::endl;
     }
     out << std::endl;
   }
   else if (!isPlayer1Turn && player==1)
   {
     for (const Link& link : links) {
        if(link.isRevealed())
        {
            out << link.display(true) << " ";
        }
        else
        {
            out << link.display(false) << " ";
        }     
        count++;
        if (count == 4) out << std::endl;
    }
        out << std::endl;
     }
   else
   {
     for (const Link& link : links) {
      out << link.display(true) << " ";
      count++;
      if (count == 4) out << std::endl;
     }
      out << std::endl;
      
   }
}

void Player::downloadLink(Link &l) {
    if (l.isDataLink()) {
        addDownloadedData();
    } else {
        addDownloadedVirus();
    }
}

int Player::abilitycount() const{
     int sum = 0;
    for (const auto& pair : abilities) {
        sum += pair.second; // Add the count (value) to the sum
    }
    return sum;
}
