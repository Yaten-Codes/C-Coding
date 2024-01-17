/*************************************************************
 * File: pqueue-vector.cpp
 *
 * Implementation file for the VectorPriorityQueue
 * class.
 */

#include "pqueue-vector.h"
#include "util.h"
#include "algorithm"

VectorPriorityQueue::VectorPriorityQueue() {
vector<string> vec;
}

VectorPriorityQueue::~VectorPriorityQueue() {
    // TODO: Fill this in!

    
}

int VectorPriorityQueue::size() {
    // TODO: Fill this in!
    return vec.size();
}

bool VectorPriorityQueue::isEmpty() {
    // TODO: Fill this in!
    return vec.size() == 0;
}

void VectorPriorityQueue::enqueue(string value) {
    // TODO: Fill this in!
    vec.push_back(value);
}

string VectorPriorityQueue::peek() {
    // TODO: Fill this in!
    if(isEmpty()) {
        error("empty queue");
    }
    string peek = vec[0];
    for(int i = 1; i < vec.size(); i++){
        if(vec[i] < peek) {
            peek = vec[i];
        }
    }
    return peek;

}

string VectorPriorityQueue::dequeueMin() {
    // TODO: Fill this in!
    if(isEmpty()) {
    error("empty queue");
 }

string del = peek();
auto iterator = find(vec.begin(), vec.end(), del);
if(iterator != vec.end()){
    vec.erase(iterator);
}
return del;

}





