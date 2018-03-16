// CS106b HW6 Huffman Encoding
// Xiangyu Peng (xypeng5) and Mingcheng Yi (mingchyi)
//This file enables the Huffman encoding and decoding functions for files and strings

#include "encoding.h"
#include "map.h"
#include "bitstream.h"
#include "pqueue.h"
#include "huffmannode.h"
#include "strlib.h"
#include "filelib.h"
using namespace std;

//This is a helper function to count frequency of ints in the input and is to be used in encoding and compressing processes
void frequencyHelper(istream& input, Map<int,int>& count){
    int oneChar;
    oneChar = input.get();
    while(oneChar != EOF){                             //read the file and create a map with int and frequency
        count[oneChar]++;
        oneChar = input.get();
    }
    count[PSEUDO_EOF] ++;                              //add a pseudo eof to the map
}

//This is a helper function using frequency map to create a Huffman Tree
HuffmanNode* buildTreeHelper(Map<int,int>& count){
    PriorityQueue<HuffmanNode*> pq;                             //create the priority queue
    for (int eachchar:count.keys()){
        HuffmanNode* eachnode = new HuffmanNode(eachchar, count[eachchar]);
        pq.enqueue(eachnode,count[eachchar]);
    }
    while(pq.size() > 1){                                       //create the Huffman tree with the priority queue
        HuffmanNode* Node0 = pq.dequeue();
        HuffmanNode* Node1 = pq.dequeue();
        int newPri = Node0->count + Node1->count;
        HuffmanNode* parent = new HuffmanNode(NOT_A_CHAR, newPri, Node0, Node1);
        pq.enqueue(parent, newPri);
    }                                                          //now the pqueue has only one element, which is the desired tree
    return pq.dequeue();
}

//This is the function to encode the Huffman Tree by calling the previous two helper functions
HuffmanNode* buildEncodingTree(istream& input) {
    Map<int,int> count;
    frequencyHelper(input, count);                              //those functions are decomposed so that the compressing/umcompressing steps would be easier
    return buildTreeHelper(count);
}

//This is the function to help translate the huffman tree to simplify the encoding process
void TreeHelper(string chosen, HuffmanNode* &treenode, Map<int,string> &translation){
    if(treenode == nullptr){
        return;
    }
    if(treenode->isLeaf()){
        translation[treenode->character] = chosen;                        //base case: a leaf is found: create a tranlation table entry
    }else{
        TreeHelper(chosen + '0', treenode->zero, translation);            //recursive backtracking to establish a translation table
        TreeHelper(chosen + '1', treenode->one, translation);
    }
}

//This function encodes the given input into huffman codes
void encodeData(istream& input, HuffmanNode* encodingTree, obitstream& output) {
    Map<int,string> translation;
    string chosen;
    TreeHelper(chosen, encodingTree,translation);
    string transCode;
    int oneChar;
    oneChar = input.get();
    while (oneChar != EOF){
        transCode += translation[oneChar];                     //translate the file/string into codes
        oneChar = input.get();
    }
    transCode += translation[PSEUDO_EOF];                      //add pseudo end of file
    for(int j = 0; j < transCode.size(); j++){
        if(transCode[j] == '0'){                               //output the tranlated code
            output.writeBit(0);
        }else{
            output.writeBit(1);
        }
    }

}

//This function is the helper function of decoding process
int decodeHelper(ibitstream& input, HuffmanNode*& treeNode){
    if (treeNode->isLeaf()){
        return treeNode->character;                         //base case: a leaf is found, return it
    }else{
        int oneDigit = input.readBit();                     //recursive case: move left/right according to the coding
        if (oneDigit == 0){
            return decodeHelper(input, treeNode->zero);
        }else{
            return decodeHelper(input, treeNode->one);
        }
    }
}

//This function will convert huffman codes back into the original file
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    int oneChar;
    while (true){
        oneChar = decodeHelper(input, encodingTree);
        if (oneChar == PSEUDO_EOF || input.fail()){
            break;
        }else{
            char realChar = oneChar;                    //convert ASCII codes back into characters/symbols
            output << realChar;
        }
    }
}

//This function compresses a file using the functions established above
void compress(istream& input, obitstream& output) {
    Map<int,int> count;
    frequencyHelper(input, count);                      //establish Huffman tree
    HuffmanNode* encodingTree = buildTreeHelper(count);
    output << count;                                    //output the frequency table as header of compressed file
    rewindStream(input);
    encodeData(input, encodingTree, output);            //compress the file
}

//This function uncompresses a file using the functions established above
void uncompress(ibitstream& input, ostream& output) {
    Map<int,int> count;
    input >> count;                                     //read the header and get the frequency table
    if (input.fail()){
        return;
    }
    HuffmanNode* encodingTree = buildTreeHelper(count); //rebuild the Huffman tree with the frequency table
    decodeData(input, encodingTree, output);            //uncompressed the file with the recovered Huffman Tree
}

//This function is used to clear the memory occupied by the tree
void freeTree(HuffmanNode* node) {
    if (node == nullptr){
        return;                         //base case 1: emptry tree
    }else if(node->isLeaf()){
        delete node;                    //base case 2: a leaf
        return;
    }else{
        freeTree(node->zero);           //recursive case: not a leaf
        freeTree(node->one);
    }
}
