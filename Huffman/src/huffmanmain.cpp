/**
 * CS 106B/X Huffman Encoding
 * This file contains the main program and user interface for running your
 * Huffman Encoder.  It contains a text menu to allow you to test all of the
 * various functions of your program for encoding and decoding data.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp
 * @version 2016/11/12
 * - 17au 106x version; menu changes based on different required functions
 * @version 2016/11/12
 * - 16au 106x version
 * @version 2013/11/21
 * - updated to contain a proper 'F' for freeTree test
 * - updated to fix compiler errors with opening files on older compilers
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "bitstream.h"
#include "console.h"
#include "filelib.h"
#include "map.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "huffmannode.h"
#include "encoding.h"
#include "huffmanutil.h"

const bool SHOW_TREE_ADDRESSES = false;   // set to true to debug tree pointer issues
const std::string DEFAULT_COMPRESSED_FILE_EXTENSION = ".huf";
const std::string DEFAULT_DECOMPRESSED_FILE_EXTENSION = ".txt";

// function prototype declarations; see definitions below for documentation
void intro();
std::string menu();
void test_buildEncodingTree(HuffmanNode*& encodingTree, std::string& data, bool& isFile);
void test_encodeData(HuffmanNode* encodingTree, std::string& data, bool& isFile);
void test_decodeData(HuffmanNode* encodingTree);
void test_compress();
void test_uncompress();
void test_freeTree(HuffmanNode* encodingTree);
void test_binaryFileViewer();
void test_textFileViewer();
void test_sideBySideComparison();
std::istream* openInputStream(std::string data, bool isFile, bool isBits = false);
std::istream* openStringOrFileInputStream(std::string& data, bool& isFile, bool isBits = false);

int main() {
    intro();

    // these variables maintain state between steps 1-4
    std::string data;
    bool isFile = false;
    HuffmanNode* encodingTree = nullptr;

    // prompt user for options repeatedly
    while (true) {
        std::string choice = menu();
        if (choice == "Q") {
            break;
        } else if (choice == "B") {
            test_buildEncodingTree(/* out ref */ encodingTree, /* out ref */ data, /* out ref */ isFile);
        } else if (choice == "E") {
            test_encodeData(encodingTree, /* out ref */ data, /* out ref */ isFile);
        } else if (choice == "D") {
            test_decodeData(encodingTree);
        } else if (choice == "C") {
            test_compress();
        } else if (choice == "U") {
            test_uncompress();
        } else if (choice == "F") {
            test_freeTree(encodingTree);
            encodingTree = nullptr;
        } else if (choice == "VB") {
            test_binaryFileViewer();
        } else if (choice == "VT") {
            test_textFileViewer();
        } else if (choice == "VS") {
            test_sideBySideComparison();
        }
    }

    std::cout << "Exiting." << std::endl;
    return 0;
}

/*
 * Sets up the output console and explains the program to the user.
 */
void intro() {
    std::cout << "Welcome to CS 106B/X Shrink-It!" << std::endl;
    std::cout << "This program uses the Huffman coding algorithm for compression." << std::endl;
    std::cout << "Any file can be compressed by this method, often with substantial" << std::endl;
    std::cout << "savings. Decompression will faithfully reproduce the original." << std::endl;
}

/*
 * Prints a menu of choices for the user and reads/returns the user's response.
 */
std::string menu() {
    std::cout << std::endl;
    std::cout << "B)uild enc.tree   C)ompress     VB) View binary" << std::endl;
    std::cout << "E)ncode data      U)ncompress   VT) View text" << std::endl;
    std::cout << "D)ecode data      F)ree tree    VS) View side-by-side" << std::endl;
    std::cout << "Q)uit" << std::endl;
    std::cout << std::endl;

    std::string choice = toUpperCase(trim(getLine("Your choice? ")));
    return choice;
}

/*
 * Tests the buildEncodingTree function.
 * Accepts a frequency table map as a parameter, presumably the one generated
 * previously in step 1 by buildFrequencyTable.
 * Then prints the encoding tree in an indented sideways format.
 * Stores the built tree in the given output parameter encodingTree.
 */
void test_buildEncodingTree(HuffmanNode*& encodingTree, std::string& data, bool& isFile) {
    std::istream* input = openStringOrFileInputStream(data, isFile);
    std::cout << "Building encoding tree ..." << std::endl;
    encodingTree = buildEncodingTree(*input);
    printSideways(encodingTree, SHOW_TREE_ADDRESSES);
}

/*
 * Tests the encodeData function.
 * Accepts as a parameter a map of encodings, presumably the one generated
 * previously in step 3 by buildEncodingMap.
 * Allows the user to encode the same data from the original file/string,
 * or new data that is typed / data from a file.
 * Once encoding is done, prints the bits of the encoded data.
 */
void test_encodeData(HuffmanNode* encodingTree, std::string& data, bool& isFile) {
    if (!encodingTree) {
        std::cout << "Can't encode data; encoding tree is null." << std::endl;
    } else {
        std::istream* input = nullptr;
        bool reuse = getYesOrNo("Reuse your previous string/file data for encoding? ");
        if (reuse) {
            input = openInputStream(data, isFile);
        } else {
            input = openStringOrFileInputStream(data, isFile);
        }

        ostringbitstream output;
        std::cout << "Encoding data ..." << std::endl;
        encodeData(*input, encodingTree, output);
        output.flush();
        std::string text = output.str();
        std::cout << "Here is the binary encoded data (" << text.length() << " bytes):" << std::endl;
        printBits(text);
        delete input;
    }
}

/*
 * Tests the decodeData function.
 * Uses the given encoding tree, presumably one encoded previously in step 2
 * by buildEncodingTree.
 * Prompts for a file or string of binary input data and decodes it into a
 * string output stream, then prints the text on the console.
 */
void test_decodeData(HuffmanNode* encodingTree) {
    if (encodingTree == nullptr) {
        std::cout << "Can't decode; encoding tree is null." << std::endl;
    } else {
        std::string data;
        bool isFile;
        ibitstream* input = (ibitstream*) openStringOrFileInputStream(data, isFile, /* isBits */ true);
        std::ostringstream output;

        std::cout << "Decoding data ..." << std::endl;
        decodeData(*input, encodingTree, output);
        output.flush();

        std::string decoded = output.str();
        std::cout << "Here is the decoded data ("
             << decoded.length() << " bytes):" << std::endl;
        std::cout << decoded << std::endl;

        delete input;
    }
}

/*
 * Tests the compress function.
 * Prompts for input/output file names and opens streams on those files.
 * Then calls your compress function and displays information about how many
 * bytes were written, if any.
 */
void test_compress() {
    std::string inputFileName = promptUserForFile("Input file name: ");
    std::ifstream input;
    ofbitstream output;
    std::string defaultOutputFileName = getRoot(inputFileName) + DEFAULT_COMPRESSED_FILE_EXTENSION;
    std::string outputFileName = trim(getLine("Output file name (Enter for "
                                    + defaultOutputFileName + "): "));
    if (outputFileName == "") {
        outputFileName = defaultOutputFileName;
    }
    if (inputFileName == outputFileName) {
        std::cout << "You cannot specify the same file as both the input file" << std::endl;
        std::cout << "and the output file.  Aborting." << std::endl;
        return;
    }
    if (!confirmOverwrite(outputFileName)) {
        return;
    }

    int inputFileSize = fileSize(inputFileName);
    std::cout << "Reading " << inputFileSize << " uncompressed bytes." << std::endl;
    input.open(inputFileName.c_str(), std::ifstream::binary);
    output.open(outputFileName.c_str());
    std::cout << "Compressing ..." << std::endl;
    compress(input, output);
    input.close();
    output.flush();
    output.close();

    if (fileExists(outputFileName)) {
        std::cout << "Wrote " << fileSize(outputFileName) << " compressed bytes." << std::endl;
    } else {
        std::cout << "Compressed output file was not found; perhaps there was an error." << std::endl;
    }
}

/*
 * Tests the decompress function.
 * Prompts for input/output file names and opens streams on those files.
 * Then calls your decompress function and displays information about how many
 * bytes were written, if any.
 */
void test_uncompress() {
    std::string inputFileName = promptUserForFile("Input file name: ");
    ifbitstream input;
    std::ofstream output;
    std::string defaultOutputFileName = getRoot(inputFileName) + "-out" + DEFAULT_DECOMPRESSED_FILE_EXTENSION;
    std::string outputFileName = trim(getLine("Output file name (Enter for "
                                    + defaultOutputFileName + "): "));
    if (outputFileName == "") {
        outputFileName = defaultOutputFileName;
    }
    if (inputFileName == outputFileName) {
        std::cout << "You cannot specify the same file as both the input file" << std::endl;
        std::cout << "and the output file.  Aborting." << std::endl;
        return;
    }
    if (!confirmOverwrite(outputFileName)) {
        return;
    }

    int inputFileSize = fileSize(inputFileName);
    std::cout << "Reading " << inputFileSize << " compressed bytes." << std::endl;
    input.open(inputFileName.c_str());
    output.open(outputFileName.c_str(), std::ofstream::binary);
    std::cout << "Decompressing ..." << std::endl;
    uncompress(input, output);
    input.close();
    output.flush();
    output.close();

    if (fileExists(outputFileName)) {
        std::cout << "Wrote " << fileSize(outputFileName) << " decompressed bytes." << std::endl;
    } else {
        std::cout << "Decompressed output file was not found; perhaps there was an error." << std::endl;
    }
}

/*
 * Tests the freeTree function by freeing the given encoding tree.
 * If the tree is null, your freeTree function is supposed to have no effect.
 */
void test_freeTree(HuffmanNode* encodingTree) {
    std::cout << "Freeing memory for encoding tree ..." << std::endl;
    freeTree(encodingTree);
}

/*
 * Binary file viewer function.
 * Prompts the user for a file name and then prints all bits/bytes of that file.
 */
void test_binaryFileViewer() {
    std::string filename = promptUserForFile("File name to display: ");
    ifbitstream input;
    input.open(filename.c_str());
    std::string fileText = readEntireStream(input);
    input.close();
    std::cout << "Here is the binary encoded data (" << fileText.length() << " bytes):" << std::endl;
    printBits(fileText);
}

/*
 * Text file viewer function.
 * Prompts the user for a file name and prints all text in that file.
 */
void test_textFileViewer() {
    std::string filename = promptUserForFile("File name to display: ");
    std::ifstream input;
    std::ostringstream output;
    input.open(filename.c_str(), std::ifstream::binary);
    while (true) {
        int ch = input.get();
        if (input.fail()) {
            break;
        }
        output.put(ch);
    }
    std::string fileText = output.str();
    std::cout << "Here is the text data (" << fileText.length() << " bytes):" << std::endl;
    std::cout << fileText << std::endl;
    input.close();
}

/*
 * Side-by-side file comparison function.
 * Prompts for two file names and then checks their contents,
 * printing information about differences between the two.
 * Most of this code is written by Keith Schwarz.
 */
void test_sideBySideComparison() {
    std::string filename1 = promptUserForFile("First file name: ");
    std::string filename2 = promptUserForFile("Second file name: ");
    std::string fileText1 = readEntireFile(filename1);
    std::string fileText2 = readEntireFile(filename2);

    // compare the two sequences to find a mismatch
    std::pair<std::string::const_iterator, std::string::const_iterator> diff =
        mismatch(fileText1.begin(), fileText1.end(), fileText2.begin());
    if (diff.first != fileText1.end()) {
        std::ptrdiff_t offset = diff.first - fileText1.begin();
        std::cout << "File data differs at byte offset " << offset << ":" << std::endl;
        std::cout << std::setw(16) << filename1 << " has value " << std::setw(3) << (int) (*diff.first)  << " ("
             << toPrintable(*diff.first)  << ")" << std::endl;
        std::cout << std::setw(16) << filename2 << " has value " << std::setw(3) << (int) (*diff.second) << " ("
             << toPrintable(*diff.second) << ")" << std::endl;
        int size1 = fileSize(filename1);
        int size2 = fileSize(filename2);
        if (size1 != size2) {
            std::cout << "File sizes differ! " << size1 << " vs. " << size2 << " bytes." << std::endl;
        }
    } else {
        std::cout << "Files match!" << std::endl;
    }
}

/*
 * Opens an input stream based on the given parameters and returns a pointer
 * to the stream that was opened.
 * If isFile is true, treats data as a file name and opens that file.
 * If isFile is false, treats data as a string of data and opens a string stream
 * over that data.
 * If isBits is true, opens the equivalent bit input stream rather than byte based.
 */
std::istream* openInputStream(std::string data, bool isFile, bool isBits) {
    if (isFile) {
        if (isBits) {
            return new ifbitstream(data);
        } else {
            std::ifstream* input = new std::ifstream;
            input->open(data.c_str(), std::ifstream::binary);
            return input;
        }
    } else {
        if (isBits) {
            return new istringbitstream(bytesToBits(data));
        } else {
            return new std::istringstream(data);
        }
    }
}

/*
 * Prompts the user to choose between reading from a string or file.
 * If the user wants to read from a string, asks the user to type said string.
 * If the user wants to read from a file, asks the user for the file name.
 * Then opens an input stream for the appropriate type of data and returns
 * a pointer to the stream.
 * The memory for the stream must be freed by the caller.
 */
std::istream* openStringOrFileInputStream(std::string& data, bool& isFile, bool isBits) {
    while (true) {
        std::string choice = toLowerCase(trim(getLine("Read from a s)tring or f)ile? ")));
        if (startsWith(choice, 's')) {
            isFile = false;
            data = getLine("Type the string to process: ");
            if (isBits) {
                // strip spaces because user may have copy/pasted from printBits output
                stringReplaceInPlace(data, " ", "");
                stringReplaceInPlace(data, "\t", "");
            }
            break;
        } else if (startsWith(choice, 'f')) {
            isFile = true;
            data = promptUserForFile("File name to process: ");
            break;
        }
    }
    return openInputStream(data, isFile, isBits);
}
