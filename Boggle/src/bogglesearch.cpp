// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include <string>
#include "grid.h"
#include "lexicon.h"
#include "set.h"
#include "map.h"
#include "bogglegui.h"

using namespace std;
bool humanWordSearch(Grid<char>& board, Lexicon& dictionary, string word);
bool humanWordSearchHelper(Grid<char>& board, string word, Set<Vector<int>>& locationUsed, Map<Vector<int>, char>& neighborsMap);
void firstNeighborMap(Grid<char>& board, Map<Vector<int>, char>& neighborsMap);
void neighborMap(Grid<char>& board, Vector<int>& wordLocation, Map<Vector<int>, char>& neighborsMap);

bool humanWordSearch(Grid<char>& board, Lexicon& dictionary, string word) {
    word = toUpperCase(word);
    if (!dictionary.contains(word)) {
        return false;
    } else if (word.size() < BoggleGUI::MIN_WORD_LENGTH) {
        return false;
    } else{
        Set<Vector<int>> locationUsed;
        Map<Vector<int>, char> neighborsMap;
        firstNeighborMap(board, neighborsMap);
        return humanWordSearchHelper(board, word, locationUsed, neighborsMap);
    }
}

bool humanWordSearchHelper(Grid<char>& board, string word, Set<Vector<int>>& locationUsed, Map<Vector<int>, char>& neighborsMap) {
    if (word.size() == 0){
        return true;
    } else{
        char s = word[0];
        cout<<s<<endl;
        for (Vector<int> key : neighborsMap){
            cout<<key<<endl;
            cout<<neighborsMap[key]<<endl;
            if (neighborsMap[key] == s && !locationUsed.contains(key)){
                Map<Vector<int>, char> oldneighborsMap = neighborsMap;
                neighborsMap.clear();
                neighborMap(board, key, neighborsMap);
                cout<<neighborsMap<<endl;
                word.erase(0);
                locationUsed.add(key);
                BoggleGUI::setHighlighted(key[0], key[1]);
                return humanWordSearchHelper(board, word, locationUsed, neighborsMap);
                BoggleGUI::clearHighlighting();
                word.insert(0,1,s);
                neighborsMap = oldneighborsMap;
            }
        }
        return false;
    }
}

void firstNeighborMap(Grid<char>& board, Map<Vector<int>, char>& neighborsMap){
    for (int r = 0; r < board.numRows(); r++){
        for (int c = 0; c < board.numCols(); c++){
            Vector<int> locations;
            locations.add(r);
            locations.add(c);
            neighborsMap.put(locations,board[r][c]);
            locations.clear();
        }
    }
}
void neighborMap(Grid<char>& board, Vector<int>& wordLocation, Map<Vector<int>, char>& neighborsMap){
    neighborsMap.clear();
    int row = wordLocation[0];
    int column = wordLocation[1];
    for (int r = row - 1; r <= row + 1; r++){
        for (int c = column - 1; c <= column +1; c++){
            if (!(c == column && r == row)){
                if (board.inBounds(r,c)){
                    Vector<int> locations;
                    locations.add(r);
                    locations.add(c);
                    neighborsMap.put(locations,board[r][c]);
                    locations.clear();
                }
            }
        }
    }
}

Set<string> computerWordSearch(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords) {
    // TODO: write this function
    Set<string> todo;   // this is only here so it will compile
    return todo;        // this is only here so it will compile
}
