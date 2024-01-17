/*************************************************************
 * File: pqueue-doublylinkedlist.cpp
 *
 * Implementation file for the DoublyLinkedListPriorityQueue
 * class.
 */

#include "pqueue-doublylinkedlist.h"
#include "util.h"

DoublyLinkedListPriorityQueue::DoublyLinkedListPriorityQueue() {
    // TODO: Fill this in!
    head = nullptr;
    count = 0;
}

DoublyLinkedListPriorityQueue::~DoublyLinkedListPriorityQueue() {
    // TODO: Fill this in!
    while (head !=  nullptr)  {//repeat
    node *p = head;  //t points to head
    head = head->next;
    delete p;
    }
}

int DoublyLinkedListPriorityQueue::size() {
    // TODO: Fill this in!
    return count;
}

bool DoublyLinkedListPriorityQueue::isEmpty() {
    // TODO: Fill this in!
    return count == 0;
}

void DoublyLinkedListPriorityQueue::enqueue(string value) {
    // TODO: Fill this in!'
    node *newbie = new node;
    newbie->data = value;
    newbie->prev = nullptr;
    if(head == nullptr) {
        newbie->next = nullptr;
        head  = newbie;
    }
else { newbie->next =head; 
head ->prev=newbie;
head = newbie;
}
count++;
}

 

string DoublyLinkedListPriorityQueue::peek() {
    // TODO: Fill this in!
    if (head == nullptr) {
    error("peek() called on empty pqueue"); }
    node *cur;
    string peek = head->data;

    for(cur = head; cur !=nullptr; cur= cur->next ){
    if(cur->data < peek) {
     peek = cur->data;
    }
}
return peek;

}

string DoublyLinkedListPriorityQueue::dequeueMin() {
    // TODO: Fill this in!
    if (head== nullptr) {   
    error("deq() called on empty pqueue"); }  

    node *curr;
    node *temp = head;
    string save = head->data;

    for(curr = head; curr !=nullptr; curr = curr->next ){
    if(curr->data <save ) {
            save = curr->data;
            temp = curr;
        }
    }
    remove(temp);
    count--;
    return save;

    }


void DoublyLinkedListPriorityQueue::remove(node *del) {  //removes node where starts from head or tails of list
    if(del->prev == nullptr){
        head = del->next;
        if (del->next !=nullptr){
            del->next->prev = nullptr;
        }
    }
    else 
        if(del->next == nullptr){
        del->prev->next = nullptr;
    }
    else{
        del->prev->next = del->next;
        del->next->prev = del->prev;
    }
    delete del;

}



                                                                                                                                  








































