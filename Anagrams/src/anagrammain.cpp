/*
 * CS 106B/X Anagram Solver
 * This instructor-provided file contains the main program and user interface
 * for running your anagram solver.
 *
 * Please DO NOT MODIFY this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp
 * @version 2016/10/26
 * - bug fix for missing letterinventory.h (removed)
 */

#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "lexicon.h"
#include "random.h"
#include "set.h"
#include "simpio.h"
#include "strlib.h"

// constant for default dictionary file name to use if the user leaves it blank
static const std::string DICTIONARY_FILE_DEFAULT = "dict1.txt";

// constants for maximum phrase length or # of words found in it to avoid long/slow searches
// static const int MAX_PHRASE_LENGTH = 16;
static const int MAX_SMALL_DICTIONARY_SIZE = 400;

// function prototype declarations
int findAnagrams(std::string phrase, int max, Set<std::string>& dictionary);   // written by you, in anagramsolver.cpp
std::string promptForDictionaryFile();
int promptForMax();
void pruneDictionary(const Lexicon& dictionary, const std::string& phrase, Set<std::string>& smallDictionary);
bool scrambleOnePhrase(const Lexicon& dictionary);

int main() {
    std::cout << "Welcome to the CS 106B/X Anagram Solver!" << std::endl;

    // read dictionary of words from a file
    std::string dictionaryFile = promptForDictionaryFile();
    std::cout << "Reading dictionary ..." << std::endl;
    Lexicon dictionary(dictionaryFile);

    while (scrambleOnePhrase(dictionary)) {
        // empty
    }

    std::cout << "Have a nice day." << std::endl;
    return 0;
}

/*
 * Repeatedly prompts the user until they type a dictionary file name for
 * a file that exists in the res/ folder, then returns that file name.
 * Or if the user types a blank line, the default file is returned.
 */
std::string promptForDictionaryFile() {
    std::string dictionaryFile;
    while (dictionaryFile.empty() || !fileExists(dictionaryFile)) {
        dictionaryFile = trim(getLine("Dictionary file name (blank for "
                                      + DICTIONARY_FILE_DEFAULT + ")? "));
        if (dictionaryFile.empty()) {
            dictionaryFile = DICTIONARY_FILE_DEFAULT;
        } else if (!stringContains(dictionaryFile, ".")) {
            dictionaryFile += ".txt";   // default .txt extension
        }
    }
    return dictionaryFile;
}

/*
 * Repeatedly prompts the user until they type a non-negative max integer,
 * then returns that integer.
 * Or if the user types a blank line, 0 is returned.
 */
int promptForMax() {
    int max = 0;
    while (true) {
        std::string maxStr = trim(getLine("Max words to include (Enter for none)? "));
        if (maxStr.empty()) {
            max = 0;
            break;
        } else if (stringIsInteger(maxStr)) {
            max = stringToInteger(maxStr);
            if (max >= 0) {
                break;
            }
        }
    }
    return max;
}

/*
 * Performs one anagram search, prompting for the phrase and max words,
 * and then printing all anagrams along with a count of how many were printed.
 * Returns true if the user wants to keep going or false to quit.
 */
bool scrambleOnePhrase(const Lexicon& dictionary) {
    // prompt for phrase to find anagrams of, and max # words
    std::cout << std::endl;
    std::string phrase = trim(getLine("Phrase to scramble (or Enter to quit)? "));
    if (phrase.empty()) {
        return false;
    }
    int max = promptForMax();
    
    // create a smaller "pruned" dictionary of only the relevant words
    Set<std::string> smallDictionary;
    pruneDictionary(dictionary, phrase, smallDictionary);
    
    // bail-out option if phrase or dict are too big (avoid long loops)
    if ( // phrase.length() > MAX_PHRASE_LENGTH ||
            smallDictionary.size() > MAX_SMALL_DICTIONARY_SIZE) {
        bool yesNo = getYesOrNo("Phrase contains many words. Search may be slow. Are you sure? (Y/n) ",
                                /* reprompt */ "",
                                /* defaultValue */ "Y");
        if (!yesNo) {
            return true;
        }
    }
    
    // call student's function to find and print the anagrams
    std::cout << "Searching for anagrams ..." << std::endl << std::endl;
    int count = findAnagrams(phrase, max, smallDictionary);
    std::cout << "Total anagrams found: " << count << std::endl;
    return true;
}

/*
 * Reduces the given dictionary lexicon into a smaller set of words that
 * contains only words that are found in the given phrase.
 * This helps optimize the process of searching for anagrams by
 * avoiding words that are not relevant to the phrase in question.
 */
void pruneDictionary(const Lexicon& dictionary, const std::string& phrase, Set<std::string>& smallDictionary) {
    // intentionally bad implementation so you won't copy it (don't copy it)
    smallDictionary.clear();
    for(const std::string& word:dictionary){
        std::string w2=word,p2=phrase;
        size_t m=0,n=0;
        for(size_t i=w2.length()*2;i>0;i+=-(4>>1)){
            unsigned int q=((w2.at((i>>1)-(2>>1)))&0x0fff)|(1<<5);
            if(q<97||q>122)continue;
            m++;
            for(size_t j=p2.length()*2;j>0;j+=-(4>>1)){
                unsigned int q2=((p2.at((j>>1)-(2>>1)))&0x0fff)|(1<<5);
                if(q==q2){
                    p2.at((j>>1)-(2>>1))=32;
                    n++;
                    break;
                }
            }
        }
        if(m==n)smallDictionary.add(word);
    }
}
