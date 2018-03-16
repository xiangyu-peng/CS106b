//Homework 5 (Linked Lists): Priority Queue
//Xiangyu Peng(xypeng_5) and Mingcheng Yi(mingchyi)
//Use binary heap  as internal data storage

#ifndef _heappriorityqueue_h
#define _heappriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class HeapPriorityQueue {
public:
    HeapPriorityQueue();
    ~HeapPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const HeapPriorityQueue& queue);

private:
    int hCapacity;
    int hSize;
    PQEntry *hPriQue;
    void bubUp(int loc);   // helper function for bubbling up
    void bubDown(int loc); // helper function for bubbling down
};

#endif
