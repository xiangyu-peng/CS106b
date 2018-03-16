/*
 * CS 106B/X Anagram Solver
 * This instructor-provided file contains the declaration of the LetterInventory
 * class that should be utilized by your anagram solver.
 * 
 * Each LetterInventory object represents a set of counts of every alphabet
 * letter from A-Z, case-insensitively.  The objects contain methods to allow
 * you to add, subtract, and test whether one letter inventory contains another.
 * See LetterInventory.cpp for implementation and documentation for each member.
 *
 * Please DO NOT MODIFY this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @version 2015/10/18
 * @author Marty Stepp
 */

#ifndef _letterinventory_h
#define _letterinventory_h

#include <iostream>
#include <string>

class LetterInventory {
public:
    /* constant for maximum index in the array (26) */
    static const int MAX;
    
    /* constructor */
    LetterInventory(std::string s = "");
    
    /* member functions */
    void add(LetterInventory& other);
    void add(const std::string& s);
    bool contains(LetterInventory& other) const;
    bool contains(const std::string& s) const;
    int getCount(char letter) const;
    bool isEmpty() const;
    void setCount(char letter, int count);
    void subtract(LetterInventory& other);
    void subtract(const std::string& s);
    int size() const;
    std::string toString() const;

private:
    /* member variables */
    int counts[27];   // set of counters from A (0) to Z (25), plus total sum (index 26)
};

/*
 * Hashing function for letter inventories
 */
int hashCode(const LetterInventory& li);

/*
 * Overloaded operators to print an inventory and compare them to each other.
 */
std::ostream& operator <<(std::ostream& out, const LetterInventory& li);
bool operator ==(const LetterInventory& li1, const LetterInventory& li2);
bool operator !=(const LetterInventory& li1, const LetterInventory& li2);
bool operator <(const LetterInventory& li1, const LetterInventory& li2);
bool operator <=(const LetterInventory& li1, const LetterInventory& li2);
bool operator >(const LetterInventory& li1, const LetterInventory& li2);
bool operator >=(const LetterInventory& li1, const LetterInventory& li2);


#endif // _letterinventory_h
