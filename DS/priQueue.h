#pragma once
#include "priNode.h"
#include <iostream>
using namespace std;


//This class impelements the priority queue as a sorted list (Linked List)
//The item with highest priority is at the front of the queue
template <typename T>
class priQueue
{
protected:  //in case you need to inherit for the project
    priNode<T>* head;
    int count;      // [Project] number of items in the queue
public:
    priQueue() : head(nullptr), count(0) {}

    ~priQueue() {
        T tmp;
        int p;
        while (dequeue(tmp,p));
    }

    //insert the new node in its correct position according to its priority
    void enqueue(const T& data, int priority) {
        priNode<T>* newNode = new priNode<T>(data, priority);
        count++;

        if (head == nullptr || priority > head->getPri()) {
            
            newNode->setNext(head);
            head = newNode;
            return;
        }
       
        priNode<T>* current = head;        
        while (current->getNext() && priority <= current->getNext()->getPri()) {
            current = current->getNext();
        }
        newNode->setNext( current->getNext());
        current->setNext( newNode);        
    }

    bool dequeue(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        priNode<T>* temp = head;
        head = head->getNext();
        delete temp;
        count--;
        return true;
    }

    bool peek(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        return true;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    // [Project] added for the project: count and print

    // O(1)
    int getCount() const { return count; }

    // O(n): prints items from highest to lowest priority separated by "separator".
    // maxItems = -1 prints all. T must have operator<< (overloaded for Car*).
    void print(const char* separator = " ", int maxItems = -1) const {
        priNode<T>* ptr = head;
        int printed = 0, pri;
        while (ptr && (maxItems < 0 || printed < maxItems)) {
            if (printed > 0) cout << separator;
            cout << ptr->getItem(pri);
            ptr = ptr->getNext();
            printed++;
        }
    }

    // The queue owns its nodes: copying it would share them, so copying is disabled.
    priQueue(const priQueue<T>&) = delete;
    priQueue<T>& operator=(const priQueue<T>&) = delete;
};
