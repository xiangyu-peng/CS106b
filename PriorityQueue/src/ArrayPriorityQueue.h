//Homework 5 (Linked Lists): Priority Queue
//Xiangyu Peng(xypeng_5) and Mingcheng Yi(mingchyi)
//Use an unsorted array as internal data storage


#ifndef _arraypriorityqueue_h
#define _arraypriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
using namespace std;

// TODO: comment
class ArrayPriorityQueue {
public:
    ArrayPriorityQueue();
    ~ArrayPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const ArrayPriorityQueue& queue);

private:
    PQEntry *aPriQue;
    int acapacity;
    int asize;
};

#endif
