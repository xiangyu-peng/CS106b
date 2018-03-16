/*
 * CS 106B/X Anagram Solver
 * This instructor-provided file contains the implementation of the
 * LetterInventory class members. See LetterInventory.h for declaration of
 * the class and each member.
 *
 * Please DO NOT MODIFY this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @version 2015/10/18
 * @author Marty Stepp
 */

#include "letterinventory.h"
#include <cctype>
#include <sstream>
#include "error.h"
#include "hashcode.h"
#include "strlib.h"

const int LetterInventory::MAX = 26;

/*
 * Constructs a letter inventory that counts the letters in the given string.
 * Capitalization is ignored; all non-alphabetic characters are not counted.
 * For example, "hello, THERE!" yields {eeehhllort}.
 */
LetterInventory::LetterInventory(std::string s) {
    // zero out / initialize array
    for (int i = 0; i <= MAX; ++i) {
        counts[i] = 0;
    }
    
    s = toLowerCase(s);
    for (int i = 0; i < (int) s.length(); ++i) {
        char ch = s[i];
        if (ch >= 'a' && ch <= 'z') {
            counts[ch - 'a']++;
            counts[MAX]++;
        }
    }
}

/*
 * Adds all letters from the given inventory to this one.
 * For example, {abc} plus {bccd} yields {abbcccd}.
 */
void LetterInventory::add(LetterInventory& other) {
    for (int i = 0; i < MAX; ++i) {
        counts[i] += other.counts[i];
    }
    counts[MAX] += other.size();
}

/*
 * Adds all alphabetic letters from the given string to this one.
 * For example, {abc} plus "bccd" yields {abbcccd}.
 */
void LetterInventory::add(const std::string& s) {
    LetterInventory other(s);
    add(other);
}

/*
 * Returns true if this inventory contains at least as many of every letter
 * as the given other inventory.
 * For example, {abbcccdddd} contains {bccd} but not {aabc} or {cde}.
 */
bool LetterInventory::contains(LetterInventory& other) const {
    for (int i = 0; i < MAX; ++i) {
        if (counts[i] < other.counts[i]) {
            return false;
        }
    }
    return true;
}

/*
 * Returns true if this inventory contains at least as many of every
 * alphabetic letter as the given string.
 * For example, {abbcccdddd} contains "bccd" but not "aabc" or "cde".
 */
bool LetterInventory::contains(const std::string& s) const {
    LetterInventory other(s);
    return contains(other);
}

/*
 * Returns the count of occurrences of the given character in this inventory.
 * The character can be passed in either case.
 * If the character passed is non-alphabetic, returns 0.
 */
int LetterInventory::getCount(char letter) const {
    letter = tolower(letter);
    if (letter < 'a' || letter > 'z') {
        return 0;
    } else {
        return counts[letter - 'a'];
    }
}

/*
 * Returns true if this inventory does not contain any letters; that is,
 * if all 26 of its counters are 0.
 */
bool LetterInventory::isEmpty() const {
    return size() == 0;
}

/*
 * Sets the count of occurrences of the given character in this inventory
 * to be the given number.
 * The character can be passed in either case.
 * If the character passed is non-alphabetic, silently does nothing.
 * If the count passed is negative, sets the count to 0 instead.
 */
void LetterInventory::setCount(char letter, int count) {
    letter = tolower(letter);
    count = count < 0 ? 0 : count;
    if (letter < 'a' || letter > 'z' || count < 0) {
        return;
    }
    int oldCount = counts[letter - 'a'];
    counts[letter - 'a'] = count;
    counts[MAX] += count - oldCount;
}

/*
 * Subtracts all alphabetic letters in the given inventory from this one.
 * For example, {aabbccdd} minus {bccd} yields {aabd}.
 * If the given other inventory is not contained in this one,
 * then the subtraction is silently aborted and no change is made.
 */
void LetterInventory::subtract(LetterInventory& other) {
    if (!contains(other)) {
        return;
    }
    for (int i = 0; i < MAX; i++) {
        counts[i] -= other.counts[i];
    }
    counts[MAX] -= other.size();
}

/*
 * Subtracts all alphabetic letters in the given string from this inventory.
 * For example, {aabbccdd} minus "bccd" yields {aabd}.
 * If the given other string is not contained in this inventory,
 * then the subtraction is silently aborted and no change is made.
 */
void LetterInventory::subtract(const std::string& s) {
    LetterInventory other(s);
    subtract(other);
}

/*
 * Returns the total of all counts in this inventory.
 * For example, {eeehhllort} has a size of 10.
 */
int LetterInventory::size() const {
    return counts[MAX];
}

/*
 * Returns a string representation of this inventory, such as "{eeehhllort}".
 */
std::string LetterInventory::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

/*
 * Returns an integer hash code for an inventory so that it could be
 * put into a HashSet or HashMap.
 */
int hashCode(const LetterInventory& li) {
    int code = hashSeed();
    for (char ch = 'a'; ch <= 'z'; ch++) {
        code = hashMultiplier() * code + hashCode(li.getCount(ch));
    }
    return int(code & hashMask());
}

/*
 * Overloaded operators
 */
bool operator ==(const LetterInventory& li1, const LetterInventory& li2) {
    if (li1.size() != li2.size()) {
        return false;
    }
    for (char ch = 'a'; ch <= 'z'; ch++) {
        if (li1.getCount(ch) != li2.getCount(ch)) {
            return false;
        }
    }
    return true;
}

bool operator !=(const LetterInventory& li1, const LetterInventory& li2) {
    return !(li1 == li2);
}

bool operator <(const LetterInventory& li1, const LetterInventory& li2) {
    return li1.toString() < li2.toString();
}

bool operator <=(const LetterInventory& li1, const LetterInventory& li2) {
    return li1.toString() <= li2.toString();
}

bool operator >(const LetterInventory& li1, const LetterInventory& li2) {
    return li1.toString() > li2.toString();
}

bool operator >=(const LetterInventory& li1, const LetterInventory& li2) {
    return li1.toString() >= li2.toString();
}

/*
 * Prints the inventory in the same format as its toString method.
 */
std::ostream& operator <<(std::ostream& out, const LetterInventory& li) {
    out << "{";
    for (char ch = 'a'; ch <= 'z'; ++ch) {
        for (int i = li.getCount(ch); i > 0; --i) {
            out << ch;
        }
    }
    out << "}";
    return out;
}
