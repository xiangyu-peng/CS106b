/**
 * CS 106B/X Huffman Encoding
 * This file implements the members of the HuffmanNode structure that you will
 * use in your Huffman encoding tree.  See HuffmanNode.h for documentation of
 * each member.
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

#include "huffmannode.h"
#include <cctype>
#include "huffmanutil.h"

static void printHuffmanNode(std::ostream& out, const HuffmanNode& node, bool showAddress = false);

HuffmanNode::HuffmanNode(int character, int count, HuffmanNode* zero, HuffmanNode* one) {
    this->character = character;
    this->count = count;
    this->zero = zero;
    this->one = one;
}

bool HuffmanNode::isLeaf() const {
    return !zero && !one;
}

std::string HuffmanNode::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

void printSideways(HuffmanNode* node, bool showAddresses, std::string indent) {
    if (node) {
        printSideways(node->one, showAddresses, indent + "  ");
        std::cout << indent;
        printHuffmanNode(std::cout, *node, showAddresses);
        std::cout << std::endl;
        printSideways(node->zero, showAddresses, indent + "  ");
    }
}

std::ostream& operator <<(std::ostream& out, const HuffmanNode& node) {
    printHuffmanNode(out, node, false);
    return out;
}

static void printHuffmanNode(std::ostream& out, const HuffmanNode& node, bool showAddress) {
    if (showAddress) {
        out << "@" << &node;
    }
    out << "{";

    if (node.character == NOT_A_CHAR) {
        out << "NOT, ";
    } else {
        out << toPrintable(node.character)
            << " (" << node.character << "), ";
    }
    out << "count=" << node.count;

    if (showAddress) {
        if (node.zero) {
            out << ", zero=" << node.zero;
        } else {
            out << ", zero=null";
        }
        if (node.one) {
            out << ", one=" << node.one;
        } else {
            out << ", one=null";
        }
    }
    out << "}";
}
