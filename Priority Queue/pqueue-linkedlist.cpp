/*************************************************************
 * File: pqueue-linkedlist.cpp
 *
 * Implementation file for the LinkedListPriorityQueue
 * class.
 */

#include <iostream>
#include "pqueue-linkedlist.h"
#include "util.h"

LinkedListPriorityQueue::LinkedListPriorityQueue() {
    // TODO: Done
    //set up empty list
    head = nullptr;
    count = 0;
}

LinkedListPriorityQueue::~LinkedListPriorityQueue() {
    // TODO: Done
    //delete dynamically allocated memory for each node
    while (head !=  nullptr)  {//repeat
    node *t = head;  //t points to head
    head = head->next;
    delete t;
    }


}

int LinkedListPriorityQueue::size() {
    // TODO: Done
    return count;
}

bool LinkedListPriorityQueue::isEmpty() {
    // TODO: Done
    return count == 0;
}

void LinkedListPriorityQueue::enqueue(string value) {
    // TODO: create new node with given value,
    //insert into correct position in list
    node *newbie = new node; 
    newbie->data = value; 
    node *cur;
    node*prev;
    cur = head;   
    prev = nullptr;

    for (cur = head; cur!= nullptr; cur = cur->next) {
        if (value < cur->data)
            break;   //found the right spot
        prev = cur;    //prev goes back to prev curr point
    }
    newbie->next = cur;
    if (prev!=nullptr)
    prev->next =newbie;
    else   //if only one insert in the beginning by
    head = newbie;
    count++;
}

string LinkedListPriorityQueue::peek() {
    // TODO:
    if (head== nullptr)
    error("peek() called on empty pqueue");
    return head->data;
    
}

string LinkedListPriorityQueue::dequeueMin() {
    // TODO: Fill this in!
    /*
        0. If queue is empty, call error()
        1. Save the string head->value so you can return it later
        2. made new pointer "temporary" pointer set to point first node in list (head)
        3. Change head->next to next
        4. delete "temporary" pointer same as deconstruct
        5. count -- 
        6.return the string you saved in step 1
    */
   if (head == nullptr) {
    error("error queue is empty");
   }
    string save = head->data;
    node *temp = head;
    head = head->next;
    delete temp;
    count--;
    return save;


}
