#include "link.h"

Link::Link(bool isData, int strength, char identifier, bool revealed, int jumps)
    : isData(isData), strength(strength), identifier(identifier), revealed(false), jumps{jumps}, knockedOut{false} {}

// Copy ctor
Link::Link(const Link &other) 
    : isData(other.isData), strength(other.strength), identifier(other.identifier), revealed(other.revealed), jumps{other.jumps}, knockedOut{other.knockedOut} {}

// Copy asst. operator
Link &Link::operator=(const Link &other) {
    if (this == &other) {
        return *this;
    }

    isData = other.isData;
    strength = other.strength;
    identifier = other.identifier;
    revealed = other.revealed;
    jumps = other.jumps;
    knockedOut = other.knockedOut;

    return *this;
}

bool Link::isDataLink() const {
    return isData;
}

int Link::getStrength() const {
    return strength;
}

char Link::getIdentifier() const {
    return identifier;
}

bool Link::isRevealed() const {
    return revealed;
}

int Link::getJumps() const {
    return jumps;
}

bool Link::getKnockedOut() const {
    return knockedOut;
}

void Link::reveal(const bool b) {
    revealed = b;
}

void Link::setStrength(const int num) {
    strength = num;
}

void Link::setLinkAsData(const bool linkType) {
    isData = linkType;
}

void Link::setJumps(const int num) {
    jumps = num;
}

void Link::setKnockedOut(const bool b) {
    knockedOut = b;
}

void Link::setIdentifier(const char id) {
    identifier = id;
}

std::string Link::display(bool reveal) const {  
    if (reveal)
    { 
        return std::string(1, identifier) + ": " + (isData ? "D" : "V") + std::to_string(strength); // Display type and strength
    }
    else
    {
        return std::string(1, identifier) + ": ?"; // Display type and strength
    }
}
