/**
 * CS 106B/X Huffman Encoding
 * This file defines various utility functions used by the main client program.
 * See huffmanutil.h for documentation of each function.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp
 * @version 2016/11/12
 * - 16au 106x version
 * @version 2013/11/16
 * - updated to fix compiler errors with opening files on older compilers
 */

#include "huffmanutil.h"
#include "bitstream.h"
#include "filelib.h"
#include "simpio.h"

std::string bitsToBytes(const std::string& text) {
    istringbitstream input(text);
    std::ostringstream out;
    while (true) {
        int bit = input.readBit();
        if (input.fail()) {
            break;
        }
        out.put(bit == 1 ? '1' : '0');
    }
    return out.str();
}

std::string bytesToBits(const std::string& text) {
    ostringbitstream out;
    for (int i = 0; i < (int) text.length(); i++) {
        out.writeBit(text[i] == '1' ? 1 : 0);
    }
    return out.str();
}

bool confirmOverwrite(const std::string& filename) {
    if (!fileExists(filename)) {
        return true;
    } else {
        return getYesOrNo(filename + " already exists. Overwrite? (y/n) ");
    }
}

void printBits(const std::string& text) {
    istringbitstream input(text);
    int i = 0;
    while (true) {
        i++;
        int bit = input.readBit();
        if (input.fail()) {
            break;
        }
        std::cout << bit;
        if (i > 0 && i % 8 == 0) {
            std::cout << " ";
        }
        if (i > 0 && i % 64 == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}
