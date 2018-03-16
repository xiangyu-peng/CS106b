//Homework 5 (Linked Lists): Priority Queue
//Xiangyu Peng(xypeng_5) and Mingcheng Yi(mingchyi)
//Use an unsorted array as internal data storage

#include "ArrayPriorityQueue.h"
#include "PQEntry.h"

ArrayPriorityQueue::ArrayPriorityQueue() {
    asize = 0;                          //priority queue's initial size
    acapacity = 30;                     //priority queue's initial capacity
    aPriQue = new PQEntry[acapacity];   //construct priorityQueue
}

ArrayPriorityQueue::~ArrayPriorityQueue() {
    delete [] aPriQue;
}

//modify the priority of a given existing value in the queue
void ArrayPriorityQueue::changePriority(string value, int newPriority) {
    for ( int i = 0; i < asize; i ++){
        if(aPriQue[i].value == value){
            aPriQue[i].priority = newPriority;
        }
    }
}

void ArrayPriorityQueue::clear() {
    asize = 0;
}

//remove the element with the most urgent priority from priority queue
string ArrayPriorityQueue::dequeue() {
    if (asize == 0){
        throw "exception";
    } else{
        int min = acapacity;
        int index = 0;
        string dequeuestring;
        for (int i = 0; i < asize; i++){
            if (min > aPriQue[i].priority){
                min = aPriQue[i].priority;              //find the one with lowest priority number and return the value
                dequeuestring = aPriQue[i].value;
                index = i;
            } else if (dequeuestring > aPriQue[i].value && min == aPriQue[i].priority){
                    dequeuestring = aPriQue[i].value;   //dequeue in alphabetical order
                    index = i;
            }
        }
        for (int j = index; j < asize - 1; j++){ // delete the PQEntry and move those after it forward
            aPriQue[j].value = aPriQue[j + 1].value;
            aPriQue[j].priority = aPriQue[j + 1].priority;
        }
        asize--;                                 // change the size!
        return dequeuestring;
    }
}

void ArrayPriorityQueue::enqueue(string value, int priority) {
    aPriQue[asize].value = value;       //add the given string value into priority queue withthe given priority
    aPriQue[asize].priority = priority;
    asize++;
}

bool ArrayPriorityQueue::isEmpty() const {
    return asize == 0;
}

string ArrayPriorityQueue::peek() const { //return the string element with the most urgent priority
    if (asize == 0){
        throw "exception";
    } else{
        int min = acapacity;
        string dequeuestring;
        for (int i = 0; i < asize; i++){               //find the one with lowest priority number and return the value
            if (min > aPriQue[i].priority){
                min = aPriQue[i].priority;
                dequeuestring = aPriQue[i].value;
            } else if (dequeuestring > aPriQue[i].value && min == aPriQue[i].priority){
                    dequeuestring = aPriQue[i].value;  //peek in alphabetical order
            }
        }
        return dequeuestring;
    }
}

//return the integer priority that is most urgent
int ArrayPriorityQueue::peekPriority() const {
    if (asize == 0){
        throw "exception";
    } else{
        int min = acapacity;
        for (int i = 0; i < asize; i++){      //find the one with lowest priority number and return the priority
            if (min > aPriQue[i].priority){
                min = aPriQue[i].priority;
            }
        }
        return min;
    }
}

int ArrayPriorityQueue::size() const {
    return asize;
}

//print priority queue to the console
ostream& operator<<(ostream& out, const ArrayPriorityQueue& queue) {
    out << "{";
    for(int i = 0; i < queue.asize; i++){
        out << queue.aPriQue[i];
        if (i != queue.asize - 1){
            out << ",";             //add ","
        }
    }
    out << "}";
    return out;
}
