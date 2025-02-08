#ifndef PLAYER_H
#define PLAYER_H

#include <ostream>
#include <vector>
#include <map>
#include <string>
#include "link.h"

class Player {
private:
    int downloadedData;            // Number of data downloads
    int downloadedViruses;         // Number of virus downloads
    bool abilityAvailable;

public:
    // Constructor
    Player();
    std::vector<Link> links;        // Links owned by the player
    std::map<char, int> abilities;

    // Accessors
    int getDownloadedData() const;
    int getDownloadedViruses() const;
    const std::vector<Link>& getLinks() const;
    int getAbilityUsage(const char& ability) const;
    bool getAbilityAvailable() const;

    // Mutators
    void addDownloadedData();
    void addDownloadedVirus();
    void decrementAbilityUsage(const char& ability);
    void incrementAbilityUsage(const char& ability);
    void addLink(const Link& link);
    void setAbilityAvailable(const bool b);

    // Check if the player is out of abilities or conditions
    bool hasAbility(const char& ability) const;\

    int abilitycount() const;

    // Utilities
    void displayLinks(std::ostream &out, bool isPlayer1Turn, int player) const;
    
    void downloadLink(Link &l);
};

#endif // PLAYER_H
