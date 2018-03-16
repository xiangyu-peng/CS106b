/**
 * CS 106B/X Huffman Encoding
 * This file declares the functions that you will need to write in this
 * assignment for your Huffman Encoder in huffmanencoding.cpp.
 *
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp
 * @version 2017/11/09
 * - 106x 17au version
 * @version 2016/11/12
 * - 106x 16au version
 * @version 2013/11/14
 * - initial version
 */

#ifndef _encoding_h
#define _encoding_h

#include <iostream>
#include <string>
#include "bitstream.h"
#include "huffmannode.h"

// function prototype declarations
// (If you want to declare your own helper function prototypes, do that near
//  the top of your .cpp file, not here.  You should NOT modify this file.)
HuffmanNode* buildEncodingTree(std::istream& input);
void encodeData(std::istream& input, HuffmanNode* encodingTree, obitstream& output);
void decodeData(ibitstream& input, HuffmanNode* encodingTree, std::ostream& output);
void compress(std::istream& input, obitstream& output);
void uncompress(ibitstream& input, std::ostream& output);
void freeTree(HuffmanNode* node);

#endif // _encoding_h

/*
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 */
