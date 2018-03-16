//Homework 3 Grammar Solver
//Xiangyu Peng (xypeng5) and Mingcheng Yi (mingchyi)
//This function is to accept a reference to an input file representing a language grammar
//and generate the given number of random expansions of the given symbol
//and return them as a Vector of strings

#include "grammarsolver.h"
#include "strlib.h"
#include "map.h"
#include "Vector.h"
#include <fstream>
#include <iostream>
#include "filelib.h"
#include "simpio.h"

using namespace std;
Map <string, Vector<string>> generateMap(istream& input);
Vector<string> grammarGenerate(istream& input, string symbol, int times);
Vector<string> grammarGenerateOnce(string symbol, Map <string, Vector<string>>& map);

//Generate a vector with the random words obtained by the given string and times
Vector<string> grammarGenerate(istream& input, string symbol, int times){
    Map <string, Vector<string>> map = generateMap(input);  //Get the map with grammar
    Vector<string> vectorLine;
    Vector<string> vectorAllLine;
    string lines;
    for (int i = 0; i < times; i++){                       //Use the function n times.
        vectorLine = grammarGenerateOnce(symbol, map);
        for (int j = 0; j < vectorLine.size(); j++){
            lines += vectorLine[j] + " ";
        }
        lines = trim(lines);                              //Delete the unwanted spaces around its edges
        vectorAllLine.add(lines);                         //Get the vector with strings we generate
        lines.clear();                                    //Clear the vector
    }
    return vectorAllLine;
}

//Generate a map for grammar
Map <string, Vector<string>> generateMap(istream& input){
    string line;
    Map <string, Vector<string>> map;
    while(!input.eof()) {
        getline(input, line);
        Vector<string> wholeLine = stringSplit(line, "::=");                // Separate the non-terminal and rules
        Vector<string> valueWithoutSplit = stringSplit(wholeLine[1], "|");  //Separate rules
        map[wholeLine[0]] = valueWithoutSplit;                              // Generate the map
    }                                                                       //Key is non-terminal and value is a vector with rules
    return map;
}

//Generate a vector with the given string once~
Vector<string> grammarGenerateOnce(string symbol, Map <string, Vector<string>>& map){
    Vector<string> oneLine;
    if (!map.containsKey(symbol)){                                   // base case: if the symbol is terminal, return it
        oneLine.add(symbol);
        return oneLine;
    } else{                                                          //recursive case: if the symbol is non-terminal, recursion
        int randomNumber = randomInteger(0, map[symbol].size()-1);   //Randomly choose one rule from the non-terminal rules
        Vector<string> splitvalue;
        splitvalue = stringSplit(map[symbol][randomNumber], " ");    //Separate the rule when it contains several rules
        for (int i =0; i<splitvalue.size(); i++){                    //like <np>::=<dp> <adjp> <n>
            oneLine += grammarGenerateOnce(splitvalue[i], map);
        }
        return oneLine;
    }
}
