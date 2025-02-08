#include "studio.h"
#include "link.h"
#include "game.h"

void Studio::render() {
    notifyObservers();
}

char Studio::getState(int row, int col) const {
    return a->getState(row, col); // Use Game's direct board access
}

char Studio::getDataorVirus(int row, int col) const {
    char c = a->getState(row, col);
    if(c != '.' && c!= '+' && c!='S' && c!='m' && c!='w')
    {
        if(a->getLinkFromID(c).isRevealed())
        {
            if(a->getLinkFromID(c).isDataLink())
            {
                return 'D';
            }
            else
            {
                return 'V';
            }
        } 
    }
    return 'U';
}
