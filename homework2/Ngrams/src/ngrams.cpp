// CS106b HW2 N-grams
//Mingcheng Yi (mingchyi) /Xiangyu Peng (xypeng5)
//This program produces a sentence with a chosen length using the given text as a reference

#include <iostream>
#include <fstream>
#include "console.h"
#include "simpio.h"
#include "filelib.h"
#include "map.h"
#include "vector.h"
#include "random.h"
using namespace std;

void welcome();
bool generrateFileMap(Map<Vector<string>, Vector<string>>& fileMap, int& n);
void generateText(Map<Vector<string>, Vector<string>>& fileMap, int& n);

int main() {
    welcome();
    Map<Vector<string>, Vector<string>> fileMap;
    int n;
    bool cont = generrateFileMap(fileMap, n);
    if (cont == true){
        generateText(fileMap, n);
    }
    cout << "Exiting." << endl;
    return 0;
}

//Welcome Message and Rules
void welcome(){
    cout << "Welcome to CS 106B/X Random Writer ('N-Grams')!"<< endl;
    cout << "This program generates random text based on a document."<< endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl<<endl;
}

//This function opens the file, save all the words into a vector, and create a Map with key/value pairs
bool generrateFileMap(Map<Vector<string>, Vector<string>>& fileMap, int& n){
    ifstream input;
    promptUserForFile(input, "Input file name?");
    Vector<string> file;
    string word;
    while(input >> word){
        file.add(word);
    }
    n = getInteger("Value of N?");
    cout << endl;
    while (true){
        if (n < 2) {                                            //continuing asking for a number of N < 2
            cout << "N must be 2 or greater." << endl;
            n = getInteger("Value of N?");
            cout << endl;
        }
        else if (n > file.size()){                                //exit the program of N > length of text
            cout << "N is larger than the number of words in the file." << endl;
            cout << "Unable to compute N-grams." <<endl;
            return false;
        }
        else {
            break;                                              //continue when N meets the requirements
        }
    }

    for(int i = 0 ;i < n - 1; i++){                              //This is to "wrap" the text by adding the
        file.add(file[i]);                                       //first N-1 words to the last
    }

    for(int i = 0; i < file.size() - n + 1; i++){                //This is to creating a map with a vector of
        fileMap[file.subList(i, (n - 1))] += file[i + n - 1];         //N-1 words as keys
    }
    return true;
}

//This function uses the map generated in the previous function to create a sentence
void generateText(Map<Vector<string>, Vector<string>>& fileMap, int& n){
    int m;
    Vector<string> text;
    while (true){
        m = getInteger("# of random words to generate (0 to quit)?");
        text.clear();
        if (m == 0) {
            break;
        }
        else if (m < n) {
            cout << "Must be at least " << n << " words" << endl;
        }
        else{
            Vector<string> ptText = fileMap.keys()[randomInteger(0, fileMap.keys().size() - 1)]; //get a random start
            text += ptText;
            for (int i = 0; i < m - n + 1; i++){
                text += fileMap.get(ptText)[randomInteger(0, fileMap.get(ptText).size() - 1)];   //add words according to
                ptText = text.subList(i + 1, n - 1);                                    //the previous words
            }
            cout << "... ";
            for (int j = 0; j < text.size(); j++){
                cout << text[j] << " ";                                                 //print out the sentence
            }
            cout << " ..." << endl << endl;
        }
    }
}
