//CS106b HW2 wordladder
//Mingcheng Yi (mingchyi) /Xiangyu Peng (xypeng5)
//This program produces a word ladder from word 1 to word 2 using the existing words
//in a dictionary chosen by the user

#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "strlib.h"
#include "vector.h"
#include "stack.h"
#include "queue.h"
#include "simpio.h"
#include "hashset.h"
#include <fstream>
using namespace std;

void intro();
void readDic(HashSet<string>& words);
void enter(const HashSet<string> words, string& word1, string& word2, int &point);
void findTheLadder(string word1, HashSet<string> words, string word2, Stack <string>& lastStack, int& point);
bool findNei(Queue<Stack <string>> ladder, HashSet <string> words, Queue <Stack <string>>& ladder1, Stack <string>& lastStack, string word2, int& point, HashSet <string> & wordsadd);
void output(Stack <string> lastStack, string word1, string word2, int point);

int main() {
    intro();
    HashSet<string> words;
    readDic(words);
    string word1;
    string word2;
    int point = - 1;                                        //points explained: point = 1 for exiting, point = 0 for ladder not found,
    while (point < 1){                                      //point = 1 for ladder found
        enter(words, word1, word2, point);
        if (point < 0){
            Stack <string> lastStack;
            findTheLadder(word1, words, word2, lastStack, point);
            output(lastStack, word1, word2, point);
            point = -1;
        }
    }
    cout << "Have a nice day." << endl;
    return 0;
}

//Welcome Message and Introduction
void intro(){
    cout << "Welcome to CS 106B/X Word Ladder!" << endl;
    cout << "Please give me two English words, and I will convert the" << endl;
    cout << "first into the second by modifying one letter at a time." << endl <<endl;
}

//This function reads the dictionary file provided by the user and save the words
//to the HashSet called 'words'
void readDic(HashSet<string>& words){
    ifstream input;
    promptUserForFile(input,"Dictionary file name:");
    cout << endl;
    string word;
    while (input >> word){
        words.add(word);
    }
}

//obtain two words from the user and check whether the words are valid
void enter(const HashSet<string> words, string& word1, string& word2,int & point){
    while (point < 1){
        while (true) {
            string word1get = getLine("Word 1 (or Enter to quit):");
            if (word1get.size() == 0){
                point = 1;
                break;
            }
            word1 = toLowerCase(word1get);
            string word2get = getLine("Word 2 (or Enter to quit):");
            if (word2get.size() == 0){
                point = 1;
                break;
            }
            word2 = toLowerCase(word2get);
            if (words.contains(word1) && words.contains(word2)){
                break;
            }else{
                cout << "The two words must be found in the dictionary." << endl;
            }
        }
        if (point<1){
            if (word1.length() != word2.length()){
                cout << "The two words must be the same length." << endl;
            }else if (word1 == word2){
                cout << "The two words must be different." << endl;
            }else{
                break;
            }
        }
    }
}

//initiate 'the queue of stacks'
void findTheLadder(string word1, HashSet<string> words, string word2, Stack <string>& lastStack, int& point){
    Queue<Stack <string>> ladder;
    Queue<Stack <string>> ladder1;
    Stack<string> stacker;
    stacker.push(word1);                //initiate the stack of one possible path
    ladder.enqueue(stacker);            //initiate the queue of all possible stacks
    HashSet<string> wordsadd;           //initiate a HashSet of all used words
    wordsadd.add(word1);
    while (lastStack.isEmpty()){
        findNei(ladder,words,ladder1,lastStack,word2,point,wordsadd);
        ladder = ladder1;
    }
}


//This function is to process the two words provided by the user and look for a word ladder between them
bool findNei(Queue<Stack <string>> ladder, HashSet <string> words, Queue <Stack <string>>& ladder1,Stack <string>& lastStack,string word2,int& point,HashSet<string>& wordsadd){
    int size = ladder.size();
    int count = 0;                                  // If after the whole loop below found no new neighbor, count stays 0.
    for(int j = 0; j < size; j++){                  // It means there are no ladder between these two words!
        Stack<string> temStack;
        temStack = ladder.dequeue();                //Obtain the first stack of ladder in the queue
        string wordNei = temStack.peek();           //Obtain the last word of the stack
        Stack<string> temsStackRemove;
        temsStackRemove = temStack;
        string wordRemoveCon;
        if (temsStackRemove.size() > 1){
            temsStackRemove.pop();                  //Get the second-to-last word for future use
            wordRemoveCon = temsStackRemove.pop();
        }else{
            wordRemoveCon = "";
        }
        string wordNeiOld = wordNei;
        for(char ch = 'a'; ch <= 'z'; ch++){
            for(int i = 0; i < wordNei.length(); i++){
                wordNei[i] = ch;
                string wordRemove = wordNei;
                string wordRemoveAgain = wordRemoveCon;
                wordRemove.erase(i,1);                                       // Get the word's letter other than the changed one
                if (wordRemoveAgain != ""){
                    wordRemoveAgain.erase(i,1);                              // Get the second-to-last word's letter other than the changed one
                }
                Stack<string> temStackTem = temStack;
                if (!equalsIgnoreCase(wordRemove,wordRemoveAgain)){           //Do not reuse the neighbor of the second-to-last word!!!
                    string wordAdd = wordNei;                                 //Iy is also proved that we won't use the reused word
                    if (words.contains(wordNei) && wordNei != wordNeiOld){   //add the word to the stack if
                        if (!wordsadd.contains(wordNei)){                    //(if) the word is not the previous one
                            temStack.push(wordAdd);
                            ladder.enqueue(temStack);
                            wordsadd.add(wordAdd);
                            count = 1;                                       // count=1 indicates finding the ladder
                        }
                    }
                    if (wordNei == word2){                                    //word ladder found! :)
                        lastStack = temStack;
                        return true;
                    }
                }
                wordNei = wordNeiOld;
                temStack = temStackTem;
            }
        }
    }
    if (count < 1){
        point = 0;                                                             //word ladder not found :(
        string nothing = "sadness";
        lastStack.push(nothing);
    }
    ladder1 = ladder;
    return false;
}

//This function is to output the solution ladder to the console
void output(Stack <string> lastStack,string word1, string word2,int point){
    if (point == 0){
        cout << "No word ladder found from " << word2 << " back to " << word1 << endl << endl;
    }else{
        cout << "A ladder from " << word2 << " back to " << word1 << endl;
        while(!lastStack.isEmpty()){
            cout << lastStack.pop() << " ";
        }
        cout << endl << endl;
    }
}







