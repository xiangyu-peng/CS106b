/**
 * CS 106B/X Huffman Encoding
 * This file declares various utility functions used by the main client program.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp
 * @version 2016/11/12
 * - 16au 106x version
 * @version 2013/11/14
 * - initial version
 */

#ifndef _huffmanutil_h
#define _huffmanutil_h

#include <iostream>
#include <string>

/*
 * Takes a string of 0 and 1 binary bits and unpacks it so that it actually
 * stores them with each "bit" being its own ASCII character of '0' or '1'.
 */
std::string bitsToBytes(const std::string& text);

/*
 * Takes a string of '0' and '1' characters and packs it so that it actually
 * stores them as bits rather than each "bit" being its own ASCII character.
 */
std::string bytesToBits(const std::string& text);

/*
 * Checks whether the given file exists; if it does, prompts the user whether
 * they want to overwrite the file.  Returns true if the user does want to
 * overwrite, and false if not.
 */
bool confirmOverwrite(const std::string& filename);

/*
 * Displays a detailed dump of every bit and byte of the given string.
 * It prints every 8 bits (one byte) followed by a space, 8 bytes per line.
 * e.g. 10010010 10110011 10100010 00011101 ...
 */
void printBits(const std::string& text);

#endif // _huffmanutil_h
