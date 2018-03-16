//Homework 5 (Linked Lists): Priority Queue
//Xiangyu Peng(xypeng_5) and Mingcheng Yi(mingchyi)
//Use binary heap  as internal data storage

#include "HeapPriorityQueue.h"

HeapPriorityQueue::HeapPriorityQueue() {
    hCapacity = 20;
    hSize = 0;
    hPriQue = new PQEntry[hCapacity];
}

HeapPriorityQueue::~HeapPriorityQueue() {
    delete [] hPriQue;
}

void HeapPriorityQueue::changePriority(string value, int newPriority) {
    for ( int i = 1; i < hSize + 1; i ++){        //modify the priority of a given existing value in the queue
        if(hPriQue[i].value == value){
            int formerPri = hPriQue[i].priority;
            hPriQue[i].priority = newPriority;
            if (newPriority > formerPri){
                bubDown(i);
            }else{
                bubUp(i);
            }
        }
    }
}

void HeapPriorityQueue::clear() {
    hSize = 0;

}

string HeapPriorityQueue::dequeue() {
    if (hSize == 0){
        throw "exception";
    } else{
        string dequeuevalue = hPriQue[1].value;
        if (hSize > 1){
            hPriQue[1].priority = hPriQue[hSize].priority;
            hPriQue[1].value = hPriQue[hSize].value;
            hSize--;
            bubDown(1);
        }else{      //if size = 1; after dequeue size = 0;
            hSize--;
        }
        return dequeuevalue;
    }
}

void HeapPriorityQueue::enqueue(string value, int priority) {
    hPriQue[hSize + 1].value = value;       //add the given string value into priority queue withthe given priority
    hPriQue[hSize + 1].priority = priority;
    bubUp(hSize + 1);
    hSize++;
}

bool HeapPriorityQueue::isEmpty() const {
    return hSize == 0;
}

string HeapPriorityQueue::peek() const {
    if (hSize == 0){
        throw "exception";
    } else{
        return hPriQue[1].value;
    }
}

int HeapPriorityQueue::peekPriority() const {
    if (hSize == 0){
        throw "exception";
    } else{
        return hPriQue[1].priority;
    }
}

int HeapPriorityQueue::size() const {
    return hSize;
}

ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    for(int i = 1; i < queue.hSize + 1; i++){
        out << queue.hPriQue[i];
        if (i != queue.hSize){
            out << ",";
        }
    }
    out << "}";
    return out;
}

// "bubbling up" hepler function
//"percolating up" by swapping it with its parent index (i/2)
//so long as it hasa more urgent (lower) priority than its parent.
void HeapPriorityQueue::bubUp(int loc){
    int priChange;
    string valueChange;
    while (loc > 1 && hPriQue[loc].priority < hPriQue[loc / 2].priority ||
    (hPriQue[loc].priority == hPriQue[loc / 2].priority && hPriQue[loc].value < hPriQue[loc / 2].value)){
        priChange = hPriQue[loc].priority;
        valueChange = hPriQue[loc].value;
        hPriQue[loc].priority = hPriQue[loc / 2].priority;
        hPriQue[loc].value = hPriQue[loc / 2].value;
        hPriQue[loc / 2].priority = priChange;
        hPriQue[loc / 2].value = valueChange;
        loc = loc / 2;
    }
}

//"bubbling down" helper function
//"percolating down" by swapping it with its more urgent-priority child index (i*2 or i*2+1)
//so long as it has a less urgent (higher) priority than its child
void HeapPriorityQueue::bubDown(int loc){
    int priChange;
    string valueChange;
    bool target;       //When finding the queue doesnot nessd to move, return false.
    int locConsider;   //consider only one of loc * 2 + 1 and loc * 2
    while (target){
        if (loc * 2 + 1 > hSize){  // if loc * 2 + 1 does not exist, only consider loc * 2
            locConsider = 2 * loc;
        } else if ((hPriQue[loc * 2].priority < hPriQue[loc * 2 + 1].priority)||
        (hPriQue[loc * 2].priority == hPriQue[loc * 2 + 1].priority && hPriQue[loc* 2].value < hPriQue[loc * 2 + 1].value)){
            locConsider = 2 * loc; // find the min priority between loc * 2 + 1 and loc * 2
        }else{
            locConsider = 2 * loc + 1;
        }
        //Check if we need to swap~
        if (hPriQue[loc].priority > hPriQue[locConsider].priority ||
        (hPriQue[loc].priority == hPriQue[locConsider].priority && hPriQue[loc].value > hPriQue[locConsider].value)){
            priChange = hPriQue[loc].priority;
            valueChange = hPriQue[loc].value;
            hPriQue[loc].priority = hPriQue[locConsider].priority;
            hPriQue[loc].value = hPriQue[locConsider].value;
            hPriQue[locConsider].priority = priChange;
            hPriQue[locConsider].value = valueChange;
            loc = locConsider;
            target = true;
        }else{
            target = false;     //If we does not need swap, stop now
        }
        if (loc * 2 > hSize){
            target = false;    // If the pointer reaches the end, stop now
        }
    }
}
