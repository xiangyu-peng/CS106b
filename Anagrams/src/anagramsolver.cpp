//Homework 4 Anagrams
//Xiangyu Peng (xypeng5) and Mingcheng Yi (mingchyi)
//This program is to use a dictionary to find and print all anagram phrases that match a given word or phrase

#include <string>
#include "letterinventory.h"
#include "set.h"
#include "Vector.h"
#include "hashset.h"

using namespace std;
int findAnagrams(string phrase, int max, Set<string>& dictionary);
void sublist(string& letters, int i, string& chosen,Set<string>& combinations);
void permutation(string& subCombinations, string chosens, Set<string>& chosenPer);
string getstring(string phrase);
Set<string> wordlist(Set<string>& dictionary, string& letterString);
void findAnagramsHelper(LetterInventory& letters, Set<string>& words, Vector<string>& chosen, Vector<Vector<string>>& lastchosen, int& max);


int findAnagrams(string phrase, int max, Set<string>& dictionary) {
    if (max < 0){
        throw "int exception";
        return 0;
    } else if (max == 0){
        max = phrase.size();
    }
    string letterString = getstring(phrase);
    Set<string> words = wordlist(dictionary, letterString); // permute all the substrings and keep those which are in dictionary
    LetterInventory letters(phrase);
    Vector<string> chosen;
    Vector<Vector<string>> lastchosen;
    findAnagramsHelper(letters, words, chosen, lastchosen, max); // use helper function
    return lastchosen.size();   // this is only here so it will compile
}

void sublist(string& letters, int i, string& chosen,Set<string>& combinations){
    if (i >= letters.size()){
        if (chosen != "") {
            combinations.add(chosen);
        }
    } else{
        sublist(letters, i+1, chosen, combinations);
        chosen+=letters[i];
        sublist(letters, i+1, chosen, combinations);
        chosen.erase(chosen.size() - 1, 1);
    }
}

void permutation(string& subCombinations, string chosens, Set<string>& chosenPer){
    if (subCombinations == ""){
        chosenPer.add(chosens);

    } else{
        for (int i = 0; i < subCombinations.size(); i++){
            char s = subCombinations[i];
            subCombinations.erase(i, 1);
            chosens.append(1,s);
            permutation(subCombinations, chosens, chosenPer);
            chosens.erase(chosens.size()-1, 1);
            subCombinations.insert(i, 1, s);
        }
    }
}

string getstring(string phrase){
    LetterInventory letters(phrase);   //change phrase to string without backspace
    string letterString;
    letterString = letters.toString();
    letterString.erase(0,1);
    letterString.erase(letterString.size() - 1, 1);
    return letterString;
}


Set<string> wordlist(Set<string>& dictionary, string& letterString){
    string chosen;
    Set<string> combinations;
    sublist(letterString, 0, chosen, combinations);  //get all the substring of the phrase
    Set<string> WordsVector;
    HashSet<string> permuSets;
    for (string word : combinations){
        cout<<"permutaioning"<<endl;
        string chosens;
        Set<string> chosenPer;
        permutation(word, chosens, chosenPer);
        cout<<"permutaioningover"<<endl;
        for (string permuWord : chosenPer){

            if (dictionary.contains(permuWord)){
                WordsVector.add(permuWord);
            }
        }
    }
    return WordsVector;
}

void findAnagramsHelper(LetterInventory& letters, Set<string>& words, Vector<string>& chosen, Vector<Vector<string>>& lastchosen, int& max){
    if (letters.isEmpty()){    //base case
        if (chosen.size() <= max){
            cout<<chosen<<endl;
            lastchosen.add(chosen);
        }
    }else {
        for (string word :words){    //recursive case
            if (letters.contains(word)){
                chosen.add(word);
                letters.subtract(word);
                findAnagramsHelper(letters, words, chosen,lastchosen, max);
                chosen.remove(chosen.size()-1);
                letters.add(word);
            }
        }
    }
}
