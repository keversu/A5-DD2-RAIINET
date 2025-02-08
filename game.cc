#include "game.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

// Constructor
Game::Game()
    :  player1(), player2(), ability(), isPlayer1Turn(true) {
    initializeBoard();
}

// Initialize the board
void Game::initializeBoard() {
    // Set up empty board
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j] = '.';
        }
    }

    // Set up server ports
    board[0][3] = 'S'; board[0][4] = 'S';
    board[7][3] = 'S'; board[7][4] = 'S';

    board[0][0] = 'a';
    board[0][1] = 'b';
    board[0][2] = 'c';
    board[1][3] = 'd';
    board[1][4] = 'e';
    board[0][5] = 'f';
    board[0][6] = 'g';
    board[0][7] = 'h';

    board[7][0] = 'A';
    board[7][1] = 'B';
    board[7][2] = 'C';
    board[6][3] = 'D';
    board[6][4] = 'E';
    board[7][5] = 'F';
    board[7][6] = 'G';
    board[7][7] = 'H';

}

// Display the board //Need to change according to requirements
ostream& Game::displayBoard(ostream& out) const {


    out<<"Player 1: "<<endl;
    out<<"Downloaded: "<< player1.getDownloadedData()<< "D, " << player1.getDownloadedViruses() << "V"<<endl;
    out<<"Abilities: "<< player1.abilitycount() << endl;
    player1.displayLinks(out, isPlayer1Turn, 1);

    for (int i = 0; i < BOARD_SIZE * 2 - 1; ++i) {
        out << '=';
    } 
    out << endl;


    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            out << board[i][j] << " ";
        }
        out << endl;
    }

    for (int i = 0; i < BOARD_SIZE * 2 - 1; ++i) {
        out << '=';
    } 
    out << endl;

    out<<"Player 2: "<<endl;
    out<<"Downloaded: "<< player2.getDownloadedData()<< "D, " << player2.getDownloadedViruses() << "V"<<endl;
    out<<"Abilities: "<< player2.abilitycount() << endl;
    player2.displayLinks(out, isPlayer1Turn, 2);

    return out;
}

// Check if the game is over
bool Game::isGameOver() const {
    if (player1.getDownloadedData() >= 4) {
        cout << "Player 1 has downloaded 4 data!" << endl;
        cout << "Player 1 wins!" << endl;
        return true;
    }
    if (player2.getDownloadedData() >= 4) {
        cout << "Player 2 has downloaded 4 data!" << endl;
        cout << "Player 2 wins!" << endl;
        return true;
    }
    if (player1.getDownloadedViruses() >= 4) {
        cout << "Player 1 has downloaded 4 viruses!" << endl;
        cout << "Player 2 wins!" << endl;
        return true;
    }
    if (player2.getDownloadedViruses() >= 4) {
        cout << "Player 2 has downloaded 4 viruses!" << endl;
        cout << "Player 1 wins!" << endl;
        return true;
    }
    return false;
}

// Process a move command
void Game::processMove(char linkID, const string& direction) {
    // Check for valid selection of units
    if ((isPlayer1Turn && ('A' <= linkID && linkID <= 'H')) ||
        (!isPlayer1Turn && ('a' <= linkID && linkID <= 'h'))) {
        cerr << "Invalid: Players can only move units on their own team!" << endl;
        return;
    }

    // Find the link on the board
    pair<int, int> linkPos = findLinkPosition(linkID);
    if (linkPos.first == -1) {
        cerr << "Link not found!" << endl;
        return;
    }
    
    // Get new position based on direction
    int x = linkPos.first;
    int y = linkPos.second;
    pair<int, int> newPos = getNewPosition(x, y, direction, getLinkFromID(linkID)); //IMP: if a link has been boosted it can't access server ports, need to check?
    if (!isValidMove(newPos)) {
        cerr << "Invalid move!" << endl;
        return;
    }

    char targetPos = board[newPos.first][newPos.second];

    if(checkWall(linkID))
    {
        board[x][y] = '.';
       if(!isValidWall(newPos))
       {
         board[x][y] = linkID;
         cout << "Walls can't move in that position!" << endl;
         return;
       }
    }
    else
    {
        if(board[newPos.first][newPos.second] == '+')
        {
          cout << "Sorry, A wall stops this move!" << endl;
          return;
        }
    }
    
    if(firewallBoard[x][y]==1)
    {
        board[x][y] = 'm';
    }
    else if (firewallBoard[x][y]==2)
    {
         board[x][y] = 'w';
    }
    else
    {
        board[x][y] = '.';
    }

    bool firewall_dwnld = false;

    if (isPlayer1Turn && firewallBoard[newPos.first][newPos.second] == 2) { // Check for firewall
        ability.firewall(player1, getLinkFromID(linkID)); // add prompts to reveal and download prompt
        cout << "Firewall applied on " << linkID << "!" << endl;
        if (getLinkFromID(linkID).isDataLink()) {
            cout << "Link revealed! " << linkID << " is a data!" << endl;
        } else {
            cout << "Player 1 downloads a virus!" << endl;
            pair<int, int> pos = findLinkPosition(linkID);
            if(checkWall(linkID))
            {
                board[x][y-1] = '.';
                board[x][y+1] = '.';
                ability.linkWithWalls.erase(remove(ability.linkWithWalls.begin(), ability.linkWithWalls.end(), linkID), ability.linkWithWalls.end());
            }
            linkID = '.';
            board[pos.first][pos.second] = '.'; 
            firewall_dwnld = true;
        }
    } else if (!isPlayer1Turn && firewallBoard[newPos.first][newPos.second] == 1) {
        ability.firewall(player2, getLinkFromID(linkID));
        cout << "Firewall applied on " << linkID << "!" << endl;
        if (getLinkFromID(linkID).isDataLink()) {
            cout << "Link revealed! " << linkID << " is a data!" << endl;
        } else {
            cout << "Player 2 downloads a virus!" << endl;
            pair<int, int> pos = findLinkPosition(linkID);
            if(checkWall(linkID))
            {
                board[x][y-1] = '.';
                board[x][y+1] = '.';
                ability.linkWithWalls.erase(remove(ability.linkWithWalls.begin(), ability.linkWithWalls.end(), linkID), ability.linkWithWalls.end());
            }
            linkID = '.';
            board[pos.first][pos.second] = '.';
            firewall_dwnld = true;
        }
    }

    if ((isPlayer1Turn && ('A' <= targetPos && targetPos <= 'H') && firewall_dwnld == false) ||
        (!isPlayer1Turn && ('a' <= targetPos && targetPos <= 'h') && firewall_dwnld == false)) { // Check for battle


        getLinkFromID(linkID).reveal(true);
        getLinkFromID(targetPos).reveal(true);
        Link battleWinner = handleBattle(getLinkFromID(linkID), getLinkFromID(targetPos));
        char winnerID = battleWinner.getIdentifier();

        Link battleLoser = getLoser(getLinkFromID(linkID), getLinkFromID(targetPos));
        
        board[newPos.first][newPos.second] = winnerID;

        if ('a' <= winnerID && winnerID <= 'h') {             // Player 1 won the battle
            handleDownload(player1, battleLoser, 1);
        } else {                                              // Player 2 won the battle
            handleDownload(player2, battleLoser, 2);            
        }

        if(battleLoser.getIdentifier()!=winnerID && checkWall(battleLoser.getIdentifier()))
        {
           board[x][y-1] = '.';
           board[x][y+1] = '.';
           ability.linkWithWalls.erase(remove(ability.linkWithWalls.begin(), ability.linkWithWalls.end(), linkID), ability.linkWithWalls.end());
        }
        else if(checkWall(winnerID))
        {
           if(isValidWall({newPos}))
           {
                board[newPos.first][newPos.second-1] = '+';
                board[newPos.first][newPos.second+1] = '+';
                board[x][y-1] = '.';
                board[x][y+1] = '.';
           }
           else
           {
             cout<<"You have placed your walls out of bounds. You lose this ability"<<endl;
             ability.linkWithWalls.erase(remove(ability.linkWithWalls.begin(), ability.linkWithWalls.end(), winnerID), ability.linkWithWalls.end());
           }
        }

    } else if (targetPos == 'S' && firewall_dwnld == false) {                                    // Check for server
        if (isPlayer1Turn) {
            handleDownload(player2, getLinkFromID(linkID), 2);
        } else {
            handleDownload(player1, getLinkFromID(linkID), 1);
        }

        if(checkWall(linkID))
        {
              board[x][y-1] = '.';
              board[x][y+1] = '.';
              ability.linkWithWalls.erase(remove(ability.linkWithWalls.begin(), ability.linkWithWalls.end(), linkID), ability.linkWithWalls.end());
        }
    } else if(firewall_dwnld == false){                                                          // Move to empty space
        board[newPos.first][newPos.second] = linkID;

        if(checkWall(linkID))
        {
           if(isValidWall({newPos}))
           {
                board[newPos.first][newPos.second-1] = '+';
                board[newPos.first][newPos.second+1] = '+';
                if(direction=="up" || direction=="down")
                {
                  board[x][y-1] = '.';
                  board[x][y+1] = '.'; 
                }
                else if(direction=="left")
                {
                  board[x][y+1] = '.';
                }
                else if(direction=="right")
                {
                  board[x][y-1] = '.'; 
                }
           }
           else
           {
             cout<<"You have placed your walls out of bounds. You lose this ability"<<endl;
             ability.linkWithWalls.erase(remove(ability.linkWithWalls.begin(), ability.linkWithWalls.end(), linkID), ability.linkWithWalls.end());
           }
        }
    }
    
    std::cout << "Move completed!" << std::endl;

    for (int i = 0; i < 15; ++i) {
        cout << '=';
    }
    cout << endl;

    nextPlayer();
}

Link& Game::handleBattle(Link &attacker, Link &defender) {
    if (attacker.getStrength() >= defender.getStrength()) {
        return attacker;
    }
    return defender;
}

Link& Game::getLoser(Link &attacker, Link &defender) {
    if (attacker.getStrength() >= defender.getStrength()) {
        return defender;
    }
    return attacker;
}

bool Game::isValidMove(const pair<int, int>& newPos) const {
    // Ensures the player is not moving its link into its own server or link
    if (isPlayer1Turn) {
        if (newPos.first == 0 && (newPos.second == 3 || newPos.second == 4)) { // server port
            return false;
        }
        if ('a' <= board[newPos.first][newPos.second] && board[newPos.first][newPos.second] <= 'h') { // crashing into own link
            return false;
        } 
    } else {
        if (newPos.first == 7 && (newPos.second == 3 || newPos.second == 4)) { // server port
            return false;
        }
        if ('A' <= board[newPos.first][newPos.second] && board[newPos.first][newPos.second] <= 'H') { // crashing into own link
            return false;
        }
    }

    // Ensure the move is within the bounds of the board
    if (newPos.first < 0 || newPos.first >= BOARD_SIZE || newPos.second < 0 || newPos.second >= BOARD_SIZE) {
        return false;
    }
    return true;
}

bool Game::isValidWall(const pair<int, int>& newPos) const
{
    if((newPos.second - 1 < 0 || newPos.second + 1 < 0 || newPos.second - 1 >= BOARD_SIZE || newPos.second + 1 >= BOARD_SIZE) || (board[newPos.first][newPos.second-1] != '.') || (board[newPos.first][newPos.second+1] != '.') )
    {
        return false;
    }
    else
    {
        return true;
    }
}

pair<int, int> Game::getNewPosition(int x, int y, const string& direction, const Link &l) const {
    if (direction == "up") {
        return {x - l.getJumps(), y};
    } else if (direction == "down") {
        return {x + l.getJumps(), y};
    } else if (direction == "left") {
        return {x, y - l.getJumps()};
    } else if (direction == "right") {
        return {x, y + l.getJumps()};
    } else {
        return {-1, -1}; // invalid move
    }
}

// Find the position of a link on the board
pair<int, int> Game::findLinkPosition(char linkID) const {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == linkID) {
                return {i, j};
            }
        }
    }
    return {-1, -1}; // Return invalid position if not found
}

// Handle server port download
void Game::handleDownload(Player &p, Link &l, int player) {
    if (player == 1) {
        cout << "Player 1 downloads a ";
    } else {
        cout << "Player 2 downloads a ";
    }
    if (l.isDataLink()) {
        p.addDownloadedData();
        cout << "data!" << endl;
    } else {
        p.addDownloadedVirus();
        cout << "virus!" << endl;
    }
}

//Initialize the links i.e. type, strength, and id for player 1 and 2
void Game::initializeLinks(vector<string> link, int player)
{
  if(player==1)
  {
    vector<char> ascii_1;
    for (char ch = 'a'; ch <= 'h'; ++ch) {
        ascii_1.push_back(ch);
    }

    for(size_t i=0; i<link.size(); i++)
    {
        if(link[i]=="V1")
        {
            player1.addLink(Link(false, 1, ascii_1[i], false, 1));
        }
        else if(link[i]=="V2")
        {
            player1.addLink(Link(false, 2, ascii_1[i], false, 1));
        }
        else if(link[i]=="V3")
        {
            player1.addLink(Link(false, 3, ascii_1[i], false, 1));
        }
        else if(link[i]=="V4")
        {
            player1.addLink(Link(false, 4, ascii_1[i], false, 1));
        }
        else if(link[i]=="D1")
        {
            player1.addLink(Link(true, 1, ascii_1[i], false, 1));
        }
        else if(link[i]=="D2")
        {
            player1.addLink(Link(true, 2, ascii_1[i], false, 1));
        }
        else if(link[i]=="D3")
        {
            player1.addLink(Link(true, 3, ascii_1[i], false, 1));
        }
        else if(link[i]=="D4")
        {
            player1.addLink(Link(true, 4, ascii_1[i], false, 1));
        }
    }
  }
  else
  {
    vector<char> ascii_2;
    for (char ch = 'A'; ch <= 'H'; ++ch) {
        ascii_2.push_back(ch);
    }
    
    for(size_t i=0; i<link.size(); i++)
    {
        if(link[i]=="V1")
        {
            player2.addLink(Link(false, 1, ascii_2[i], false, 1));
        }
        else if(link[i]=="V2")
        {
            player2.addLink(Link(false, 2, ascii_2[i], false, 1));
        }
        else if(link[i]=="V3")
        {
            player2.addLink(Link(false, 3, ascii_2[i], false, 1));
        }
        else if(link[i]=="V4")
        {
            player2.addLink(Link(false, 4, ascii_2[i], false, 1));
        }
        else if(link[i]=="D1")
        {
            player2.addLink(Link(true, 1, ascii_2[i], false, 1));
        }
        else if(link[i]=="D2")
        {
            player2.addLink(Link(true, 2, ascii_2[i], false, 1));
        }
        else if(link[i]=="D3")
        {
            player2.addLink(Link(true, 3, ascii_2[i], false, 1));
        }
        else if(link[i]=="D4")
        {
            player2.addLink(Link(true, 4, ascii_2[i], false, 1));
        }
    }
 }
}

Link& Game::getLinkFromPos(pair<int, int> &pos) {
    char linkID = board[pos.first][pos.second];
    return getLinkFromID(linkID);
}

Link& Game::getLinkFromID(char linkID) {
    for (auto &link : player1.links) {
        if (link.identifier == linkID) {
            return link;
        }
    }

    for (auto &link : player2.links) {
        if (link.identifier == linkID) {
            return link;
        }
    }

     throw invalid_argument("Invalid link ID");
}

void Game::nextPlayer() {
    if (isPlayer1Turn) {
        isPlayer1Turn = false;
        player2.setAbilityAvailable(true);
        for (auto &link : player2.links) {
            if (link.getKnockedOut()) {
                examineKnockedOut(link, findLinkPosition(link.getIdentifier()));
                if(!link.getKnockedOut()) {
                    cout<<endl;
                    cout << link.identifier << " is freed from knock out! It can move again!" << endl;
                    cout<<endl;
                }
            }
            
        }
    } else {
        isPlayer1Turn = true;
        player1.setAbilityAvailable(true);
        for (auto &link : player1.links) {
            if (link.getKnockedOut()) {
                examineKnockedOut(link, findLinkPosition(link.getIdentifier()));
                if(!link.getKnockedOut()) {
                    cout<<endl;
                    cout << link.identifier << " is freed from knock out! It can move again!" << endl;
                    cout<<endl;
                }
            }
        }
    }
}


char Game::getState(int row, int col) const {
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
        return board[row][col]; // Valid cell access
    }
    return '.'; // Return default for invalid indices
}

Game::~Game() {}

char Game::charAt(int row, int col) {
    return '0';
}

void Game::setCell(const pair<int, int>& pos, const char c) {
    board[pos.first][pos.second] = c;
}

void Game::examineKnockedOut(Link &l, const pair<int, int> &pos) {
    if (isPlayer1Turn) {
        if (pos.first > 0) { // check top
            if ('a' <= board[pos.first - 1][pos.second] && board[pos.first - 1][pos.second] <= 'h') {
                l.setKnockedOut(false);
            }
        }
        if (pos.first < BOARD_SIZE) { // check down
            if ('a' <= board[pos.first + 1][pos.second] && board[pos.first + 1][pos.second] <= 'h') {
                l.setKnockedOut(false);
            }
        } 
        if (pos.second > 0) { // check left
            if ('a' <= board[pos.first][pos.second - 1] && board[pos.first][pos.second - 1] <= 'h') {
                l.setKnockedOut(false);
            }
        } 
        if (pos.second < BOARD_SIZE) { // check right
            if ('a' <= board[pos.first][pos.second + 1] && board[pos.first][pos.second + 1] <= 'h') {
                l.setKnockedOut(false);
            }
        } 
    } else {
        if (pos.first > 0) { // check top
            if ('A' <= board[pos.first - 1][pos.second] && board[pos.first - 1][pos.second] <= 'H') {
                l.setKnockedOut(false);
            }
        }
        if (pos.first < BOARD_SIZE) { // check down
            if ('A' <= board[pos.first + 1][pos.second] && board[pos.first + 1][pos.second] <= 'H') {
                l.setKnockedOut(false);
            }
        } 
        if (pos.second > 0) { // check left
            if ('A' <= board[pos.first][pos.second - 1] && board[pos.first][pos.second - 1] <= 'H') {
                l.setKnockedOut(false);
            }
        } 
        if (pos.second < BOARD_SIZE) { // check right
            if ('A' <= board[pos.first][pos.second + 1] && board[pos.first][pos.second + 1] <= 'H') {
                l.setKnockedOut(false);
            }
        } 
    }
    
}
bool Game::setWalls(char linkID, int x, int y1, int y2){
    if(board[x][y1] != '.' || board[x][y2] != '.' || x < 0 || x >= BOARD_SIZE || y1 < 0 || y1 >= BOARD_SIZE || y2 < 0 || y2 >= BOARD_SIZE)
    {
       return false;
    }
    else
    {
        setCell({x, y1}, '+');
        setCell({x,y2}, '+');
        return true;
    }
}

bool Game::checkWall(char linkID)
{
    auto it = find(ability.linkWithWalls.begin(), ability.linkWithWalls.end(), linkID);
    
    if(it != ability.linkWithWalls.end())
    {
         return true;
    }
    return false;
}


