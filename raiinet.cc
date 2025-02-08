#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <memory>

#include "link.h"
#include "player.h"
#include "game.h"
#include "ability.h"
#include "boardgraphics.h"
#include "window.h"

#include "studio.h"
#include "observer.h"
#include "subject.h"
#include "boardgraphics.h"

using namespace std;

// Default abilities and links
const string DEFAULT_ABILITIES = "LFDSP"; // Link_Boost, Firewall, Download, Scan, Polarize
const vector<string> DEFAULT_LINKS = {"V1", "D4", "V3", "V2", "D3", "V4", "D2", "D1"};
const map<char, string> abilityNames = {
        {'L', "Link Boost"},
        {'F', "Firewall"},
        {'S', "Scan"},
        {'D', "Download"},
        {'P', "Polarize"},
        {'C', "Craze"},
        {'E', "Exchange"},
        {'K', "Knock Out"},
        {'W', "Walls"}
};

map<char, int> abilityID_1 = {
        {'L', 0},
        {'F', 0},
        {'S', 0},
        {'D', 0},
        {'P', 0},
        {'C', 0},
        {'E', 0},
        {'K', 0},
        {'W', 0}
};

map<char, int> abilityID_2 = {
        {'L', 0},
        {'F', 0},
        {'S', 0},
        {'D', 0},
        {'P', 0},
        {'C', 0},
        {'E', 0},
        {'K', 0},
        {'W', 0}
};

// Function to randomize links if no file is provided
vector<string> randomizeLinks() {
    vector<string> links = DEFAULT_LINKS;
    random_device rd;
    shuffle(links.begin(), links.end(), default_random_engine(rd()));
    return links;
}

// Function to read links from a file
vector<string> readLinksFromFile(const string& filename) {
    vector<string> links;
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Unable to open file " << filename << endl;
        return DEFAULT_LINKS; // Fallback to default
    }
    string link;
    while (file >> link) {
        links.push_back(link);
    }
    file.close();
    return links;
}

Game g;

Studio s{&g};
vector<shared_ptr<Observer>> observers;

void abilityNotUsed(char ability)
{
    if(g.isPlayer1Turn)
    {
        g.player1.incrementAbilityUsage(ability);
        g.player1.setAbilityAvailable(true);
    }
    else
    {
        g.player2.incrementAbilityUsage(ability);
        g.player2.setAbilityAvailable(true);
    }
}

bool isValidLinkID(char id) {
    if (('a' <= id && id <= 'h') || ('A' <= id && id <= 'H')) {
        return true;
    }
    return false;
}


void processCommand(const string& command) {
    istringstream iss(command);
    string action;
    iss >> action;

// have to check if game ended
    if (action == "move") {
        char linkID;
        string dir;  //need to have error checks if wrong input is put in so sytem doesn't break
        iss >> linkID >> dir;

        if (!isValidLinkID(linkID)) {
            cerr << "Invalid link selection!" << endl;
            return;
        }

        if (g.getLinkFromID(linkID).getKnockedOut()) {
            cout << linkID << " is knocked out so it cannot move! A link on your team must be next to " << linkID << " to save it!" << endl;
        } else {
            bool prevplayer = g.isPlayer1Turn;
            g.processMove(linkID, dir);

            if(g.isPlayer1Turn != prevplayer)
            {
                processCommand("board");
            }
        }
        
    } 
    else if (action == "abilities") 
    {
        // Iterate over the abilities map
        int id = 1;  // ID starts from 1
        if(g.isPlayer1Turn)
        {
            for (const auto& pair : g.player1.abilities) {
                char ability = pair.first;
                int value = pair.second;

                // Print the ability name, ID, and value
                if (abilityNames.find(ability) != abilityNames.end()) {
                    cout << "Ability ID: " << id << ", Ability: " << abilityNames.at(ability)
                         << ", Available: " << value << endl;
                    id++;
                }
            }
        }
        else
        {
            for (const auto& pair : g.player2.abilities) {
                char ability = pair.first;
                int value = pair.second;

                // Print the ability name, ID, and value
                if (abilityNames.find(ability) != abilityNames.end()) {
                    cout << "Ability ID: " << id << ", Ability: " << abilityNames.at(ability)
                         << ", Available: " << value << endl;
                    id++;
                }
            }            
        }
    } 
    else if (action == "ability") 
    {
        int id;
        iss >> id;
        if (!iss) {
            cerr << "Error: Invalid ability command format." << endl;
            return;
        }
        
        char ability;

        if(g.isPlayer1Turn)
        {
           for (const auto& pair : abilityID_1) {
            if (pair.second == id) {
                ability = pair.first;
                break;  // Exit the loop after finding the first match
            }
          }
        }
        else
        {
           for (const auto& pair : abilityID_2) {
                if (pair.second == id) {
                    ability = pair.first;
                    break;  // Exit the loop after finding the first match
                }
          }
        }

        //check if they still are allowed to use that ability
        //subtract after use

        bool use_ability = true;

        if ((g.isPlayer1Turn && !g.player1.getAbilityAvailable()) || (!g.isPlayer1Turn && !g.player2.getAbilityAvailable())) 
        {   
            use_ability = false;
            cout << "You may only use an ability once per round!" << endl;
        }
        else if ((g.isPlayer1Turn && !g.player1.hasAbility(ability)) || (!g.isPlayer1Turn && !g.player2.hasAbility(ability)))
        {
            use_ability = false;
            cout << "You can no longer use this ability!" << endl;
        }
        else if (g.isPlayer1Turn && g.player1.hasAbility(ability))
        {
            g.player1.decrementAbilityUsage(ability);            
        }
        else if (!g.isPlayer1Turn && g.player2.hasAbility(ability))
        {
            g.player2.decrementAbilityUsage(ability);
        }
    

       if(use_ability)
       {
        if (ability=='L')
        {
            char link;
            if (iss >> link) {
                if (isValidLinkID(link)) {
                    g.ability.linkBoost(g.getLinkFromID(link));  // Apply Link Boost to the chosen link
                    cout << "Link boost applied on " << link << "!" << endl;

                    if (g.isPlayer1Turn) {
                        g.player1.setAbilityAvailable(false);
                    } else {
                        g.player2.setAbilityAvailable(false);
                    }
                } else {
                    cerr << "Error: Invalid link for Link Boost." << endl;
                    if(g.isPlayer1Turn)
                    {
                        g.player1.incrementAbilityUsage(ability);
                    }
                    else
                    {
                        g.player2.incrementAbilityUsage(ability);
                    }
                }
                
            } else {
                cerr << "Error: Invalid link for Link Boost." << endl;
                if(g.isPlayer1Turn)
                {
                    g.player1.incrementAbilityUsage(ability);
                }
                else
                {
                    g.player2.incrementAbilityUsage(ability);
                }
            }
        }
        else if (ability=='F') {
            int row, col;
            if (iss >> row >> col) {
                if (g.getState(row, col) == '.')
                {
                    if (g.isPlayer1Turn) g.firewallBoard[row][col] = 1;
                    else g.firewallBoard[row][col] = 2;
                    pair<int, int> pos = {row, col};
                    if (g.isPlayer1Turn) {
                        g.setCell(pos, 'm');
                    } else {
                        g.setCell(pos, 'w');
                    }
                    cout << "Firewall applied at [" << row << "][" << col << "]!" << endl;
                    processCommand("board");

                    if (g.isPlayer1Turn) {
                        g.player1.setAbilityAvailable(false);
                    } else {
                        g.player2.setAbilityAvailable(false);
                    }
                }
                else
                {
                    abilityNotUsed(ability);
                    cerr<<"Firewall should be set in an empty block!"<<endl;
                }
            } else {
                    abilityNotUsed(ability);
                cerr << "Error: Invalid coordinates for Firewall." << endl;
            }

        } else if (ability=='S') { // does not reveal own links
            char link;
            
            if (iss >> link) {
                if (isValidLinkID(link)) {
                    cout << "Scan applied on " << link << "!" << endl;
                    if ((g.isPlayer1Turn && 'a' <= link && link <= 'h') ||
                        (!g.isPlayer1Turn && 'A' <= link && link <= 'H')) {
                        abilityNotUsed(ability);
                        cout << "Scan applied on your own Link! Information unrevealed!" << endl;
                    } 
                    else if (link =='.')
                    {
                        abilityNotUsed(ability);
                        cout << "Scan applied on an empty space! Information unrevealed!" << endl;
                    }
                    else {
                        Link l = g.getLinkFromID(link);
                        g.ability.scan(g.getLinkFromID(link));
                        if (l.isDataLink()) {
                            cout << "Type: Data" << endl;
                        } else {
                            cout << "Type: Virus" << endl;
                        }
                        cout << "Strength: " << l.getStrength() << endl;

                        if (g.isPlayer1Turn) {
                            g.player1.setAbilityAvailable(false);
                        } else {
                            g.player2.setAbilityAvailable(false);
                        }
                        processCommand("board");
                    }
                } else {
                    abilityNotUsed(ability);
                    cerr<<"Invalid link!"<<endl;
                }
            }
            else
            {
                abilityNotUsed(ability);
                cerr<<"Invalid link!"<<endl;
            }
        } else if (ability=='D') {
            char link; 
            if (iss >> link) {
                if (isValidLinkID(link)) {
                    if ((g.isPlayer1Turn && 'a' <= link && link <= 'h') ||
                        (!g.isPlayer1Turn && 'A' <= link && link <= 'H')) {
                        cout << "Download cannot be applied on your own links!" << endl;
                        abilityNotUsed(ability);
                    } else {
                        g.setCell(g.findLinkPosition(link), '.');
                        if (g.isPlayer1Turn) {
                            g.ability.download(g.player1, g.getLinkFromID(link));
                            cout << "Player 1 downloads " << link << "!" << endl;
                        } else {
                            g.ability.download(g.player2, g.getLinkFromID(link));
                            cout << "Player 2 downloads " << link << "!" << endl;
                        }

                        if (g.isPlayer1Turn) {
                            g.player1.setAbilityAvailable(false);
                        } else {
                            g.player2.setAbilityAvailable(false);
                        }
                    }
                } else {
                    abilityNotUsed(ability);
                    cerr<<"Invalid link!"<<endl;
                }
            }
            else
            {
                abilityNotUsed(ability);
                cerr<<"Invalid link!"<<endl;
            }
        } else if (ability=='P') {
            char link;
            if (iss >> link) {
                if (isValidLinkID(link)) {
                    g.ability.polarize(g.getLinkFromID(link));
                    cout << "Polarize applied on " << link << "!" << endl;

                    if (g.isPlayer1Turn) {
                            g.player1.setAbilityAvailable(false);
                        } else {
                            g.player2.setAbilityAvailable(false);
                        }
                } else {
                    abilityNotUsed(ability);
                    cerr<<"Invalid link!"<<endl;
                }
            }
            else
            {
                abilityNotUsed(ability);
                cerr<<"Invalid link!"<<endl;
            }
        } else if (ability=='C') {
            char link;
            if (iss >> link) {
                if (isValidLinkID(link)) {
                    g.ability.craze(g.getLinkFromID(link));
                    cout << "Craze applied on " << link << "!" << endl;

                    if (g.isPlayer1Turn) {
                        g.player1.setAbilityAvailable(false);
                    } else {
                        g.player2.setAbilityAvailable(false);
                    }
                } else {
                    abilityNotUsed(ability);
                    cerr<<"Invalid link!"<<endl;
                }
            }
            else
            {
                abilityNotUsed(ability);
                cerr<<"Invalid link!"<<endl;
            }
        }
        else if (ability=='E')
        {
            char link1, link2;
            if (iss >> link1 >> link2) 
            {
                if (isValidLinkID(link1) && isValidLinkID(link2)) {
                    g.ability.exchange(g.getLinkFromID(link1), g.getLinkFromID(link2));
                    cout << "Exchange applied on " << link1 << " and " << link2 << "!" << endl;
                    if (g.isPlayer1Turn) {
                        g.player1.setAbilityAvailable(false);
                    } else {
                        g.player2.setAbilityAvailable(false);
                    }
                } else {
                    abilityNotUsed(ability);
                    cerr<<"Invalid link!"<<endl;
                }
            }
            else
            {
                abilityNotUsed(ability);
                cerr<<"Invalid link!"<<endl;
            }
        }
        else if (ability=='W')
        {
            char link;
            if (iss >> link)
            {
                if (isValidLinkID(link)) {
                    if(g.ability.walls(link, g))
                    {
                        cout << "Set up walls for: " << link <<endl;
                        processCommand("board");
                        
                        if (g.isPlayer1Turn) {
                            g.player1.setAbilityAvailable(false);
                        } else {
                            g.player2.setAbilityAvailable(false);
                        }
                    }
                    else
                    {
                        cout<< "Could not set up walls for: " << link <<endl;
                        abilityNotUsed(ability);
                    }
                } else {
                    abilityNotUsed(ability);
                    cerr<<"Invalid link!"<<endl;
                }
            }
            else
            {
                abilityNotUsed(ability);
                cerr<<"Invalid link!"<<endl;
            }

        }
        else if (ability=='K')
        {
            char link;
            if (iss >> link)
            {
                if (isValidLinkID(link)) {
                    if ((g.isPlayer1Turn && 'a' <= link && link <= 'h') ||
                        (!g.isPlayer1Turn && 'A' <= link && link <= 'H')) {
                        cout << "Knock out cannot be applied on your own links!" << endl;
                        abilityNotUsed(ability);
                    } else {
                        g.ability.knockOut(g.getLinkFromID(link));
                        cout << "Knock out applied on " << link << "!" << endl;

                        if (g.isPlayer1Turn) {
                            g.player1.setAbilityAvailable(false);
                        } else {
                            g.player2.setAbilityAvailable(false);
                        }
                    }
                } else {
                    abilityNotUsed(ability);
                    cerr<<"Invalid link!"<<endl;
                }
            }
            else
            {
                abilityNotUsed(ability);
                cerr<<"Invalid link!"<<endl;
            }
        }
      }
    } 
    else if (action == "board") 
    {
        ostringstream out;
       g.displayBoard(out);
        cout << out.str();
       s.render();

    } 
    else if (action == "sequence") 
    {
        string filename;
        iss >> filename;
        if (!iss) {
            cerr << "Error: Invalid sequence command format."<<endl;
            return;
        }
        ifstream file(filename);
        if (!file) {
            cerr << "Error: Unable to open file " << filename << "."<<endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            processCommand(line);
        }
    } 
    else if (action == "quit") 
    {
        cout << "Quitting the game."<<endl;
        exit(0);
    } 
    else 
    {
        cerr << "Error: Unknown command."<<endl;
    }
 
}

int main(int argc, char* argv[ ]) {
    string ability1 = DEFAULT_ABILITIES; 
    string ability2 = DEFAULT_ABILITIES; 
    vector<string> link1 = randomizeLinks(); 
    vector<string> link2 = randomizeLinks(); 
    bool graphics = false;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-ability1" && i + 1 < argc) {
            ability1 = argv[++i];
        } else if (arg == "-ability2" && i + 1 < argc) {
            ability2 = argv[++i];
        } else if (arg == "-link1" && i + 1 < argc) {
            link1 = readLinksFromFile(argv[++i]);
        } else if (arg == "-link2" && i + 1 < argc) {
            link2 = readLinksFromFile(argv[++i]);
        } else if (arg == "-graphics") {
            observers.emplace_back(make_shared<BoardGraphics>(&s, 8));
            graphics = true;
        }
    }

    g.initializeLinks(link1, 1);
    g.initializeLinks(link2, 2);
    for (char ability : ability1)
    {
        if(g.player1.abilities[ability] < 2)
        {
            g.player1.abilities[ability]++;
        }
    }
    for (char ability : ability2)
    {
        if(g.player2.abilities[ability]<2)
        {
            g.player2.abilities[ability]++;
        }
    }

    if(graphics)
    {
       processCommand("board");
    }
    string command;
    bool game = true;
    //ostringstream out;
     //  g.displayBoard(out);
      //  cout << out.str();
    cout << "Enter commands (type 'quit' to exit):\n";
    //setting up id and ability pairs for both players.

    int id = 1;
     for (const auto& pair : g.player1.abilities) {
            char ability = pair.first;

            if(g.player1.abilities[ability]>0)
            {
                // Print the ability name, ID, and value
                if (abilityNames.find(ability) != abilityNames.end()) {
                    abilityID_1[ability] = id;
                    id++;
                }
            }
    }

    id = 1;
    for (const auto& pair : g.player2.abilities) {
            char ability = pair.first;

        if(g.player2.abilities[ability]>0)
        {
            // Print the ability name, ID, and value
           if (abilityNames.find(ability) != abilityNames.end()) {
                abilityID_2[ability] = id;
                id++;
            }
        }
    }

    while (game) {
        for (int i = 0; i < 20; ++i) {
            cout << '=';
        }
        cout << endl;
    
        cout << "> ";
        getline(cin, command);
        if (command == "quit") game = false;
        processCommand(command);
        if(g.isGameOver()) game = false;
        
    }
    return 0;
}
