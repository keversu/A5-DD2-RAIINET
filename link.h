// Links.h
#ifndef LINKS_H
#define LINKS_H

#include <string>

class Link {
    friend class Game;
private:
    bool isData;    // True if it's a data link, false if it's a virus
    int strength;   // Strength of the link (1-4)
    char identifier; // A unique character to identify this link (e.g., 'a', 'b')
    bool revealed;  // True if the link is revealed to the opponent
    int jumps;      // The number of jumps a link would take for each move
    bool knockedOut; // true if the link has the knock out ability applied

public:
    // Constructor
    Link(bool isData, int strength, char identifier, bool revealed, int jumps);

    // Copy ctor
    Link(const Link &other);

    // Copy asst. operator
    Link &operator=(const Link &other);

    // Accessors
    bool isDataLink() const;
    int getStrength() const;
    char getIdentifier() const;
    bool isRevealed() const;
    int getJumps() const;
    bool getKnockedOut() const;

    // Mutators
    void reveal(const bool b);                  // Reveal the link to the opponent
    void setStrength(const int num);            // Sets the strength of the link
    void setLinkAsData(const bool linkType);    // Change from data to virus or vice versa (used for Polarize)
    void setJumps(const int num);               // Sets the number of squares the link jumps
    void setKnockedOut(const bool b);           // Sets the knockedOut bool
    void setIdentifier(const char id);          // Sets the identifier
    
    // Display
    std::string display(bool reveal) const; // Return the string representation of the link for display
};

#endif // LINKS_H
