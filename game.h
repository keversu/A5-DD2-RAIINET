#ifndef GAME_H
#define GAME_H

#include <vector>
#include <map>
#include "link.h"
#include "player.h"
#include "ability.h"

using namespace std;

class Game {
private:
    static const int BOARD_SIZE = 8;
    char board[BOARD_SIZE][BOARD_SIZE]; // 8x8 game board

    void initializeBoard();        // Set up initial board layout
    Link &handleBattle(Link &attacker, Link &defender); // Handle battle logic
    Link &getLoser(Link &attacker, Link &defender); // Handle battle logic
    void handleDownload(Player &p, Link &l, int player); // Handle download at server port
    bool isValidMove(const pair<int, int>& newPos) const;
    pair<int, int> getNewPosition(int x, int y, const std::string& direction, const Link &l) const;
    Link &getLinkFromPos(pair<int, int> &pos); // Returns the link given the position
    void nextPlayer();
    void examineKnockedOut(Link &l, const pair<int, int> &pos);
    bool isValidWall(const pair<int, int>& newPos) const;

public:
    Game();
    Player player1;           // Player 1
    Player player2;           // Player 2
    Ability ability;
    int firewallBoard[BOARD_SIZE][BOARD_SIZE]; // 8x8 firewall board to check for firewalls
    void start(); // Start the game loop
    bool isPlayer1Turn;       // Whose turn it is
    void initializeLinks(vector<string> link, int player);        // Set up link position on board for player 1
    ostream& displayBoard(ostream& out) const;     // Print the current state of the board
    void processMove(char linkID, const std::string& direction); // Move a link
    Link &getLinkFromID(char linkID); // Returns the link given the link ID
    bool isGameOver() const;       // Check if the game is over
    char getState(int row, int col) const;
    pair<int, int> findLinkPosition(char linkID) const; // Find the position of a link
    void setCell(const pair<int, int>& pos, const char c);  // Sets the cell to the given char
    bool setWalls(char linkID, int x, int y1, int y2);
    bool checkWall(char linkID);

    //for the observers feeding
    virtual ~Game();
    virtual char charAt(int row, int col);
};

#endif
