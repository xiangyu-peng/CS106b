//Homework3 String Problems
//Xiangyu Peng (xypeng5) and Mingcheng Yi (mingchyi)

#include "stringproblems.h"
#include <iostream>
#include "strlib.h"

using namespace std;
string reverse(string s);
string lettersOnly(string s);

//Searching for a string in another string
bool contains(string s1, string s2) {
    if (s2.size() == 0){                                   //base case
        return true;
    } else if (s1.size() == 0 && s2.size() != 0 ){
        return false;
    } else {
        if (toLowerCase(s1[0]) == toLowerCase(s2[0])){     //recursive case
            return contains(s1.substr(1), s2.substr(1));
        } else{
            return contains(s1.substr(1), s2);
        }
    }
}

//Filtering strings to letters only
string lettersOnly(string s) {
    if (s.size() == 0){                                                //base case
        return "";
    } else{
        if (s[0] >= 'a' && s[0] <= 'z' || s[0] >= 'A' && s[0] <= 'Z'){ //recursive case
            return s[0] + lettersOnly(s.substr(1));
        } else{
            return lettersOnly(s.substr(1));
        }
    }
}

//Print to the console the binary (base-2) representation of the given base-10 integer n.
void printBinary(int n) {
    if (n/2 == 0){                   //base case
        cout << n;
    } else{
        printBinary(n/2);            //recursive case
        cout << abs(n % 2);
    }
}

//Return a string with the same characters as its parameter s but in the opposite order.
string reverse(string s) {
    if (s.size() == 0){                       //base case
        return s;
    } else{
        return reverse(s.substr(1)) + s[0];   //recursive case
    }
}
