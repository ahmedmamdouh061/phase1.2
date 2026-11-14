#pragma once
#include "../DS/LinkedQueue.h"
#include "../Car.h"

/*
  IDLE List (member of Center)
  A class derived from Queue.
  Why Queue: cars are loaded in this list according to VT and are picked in the
             same order. So it is FIFO.
  Why derived: to add InsertCar() that inserts an ERLY car that rescheduled
               in its correct place (sorted by its new VT).

  Operations: enqueue O(1), dequeue O(1), print first 15 cars O(1), InsertCar O(n)
*/
class IDLE_List : public LinkedQueue<Car*>
{
public:
	// Inserts the car so the list stays sorted by VT.
	// Cars with the same VT keep their arrival order (new car goes after them). O(n)
	void InsertCar(Car* car)
	{
		Node<Car*>* newNode = new Node<Car*>(car);
		count++;

		if (isEmpty())
		{
			frontPtr = backPtr = newNode;
			return;
		}
		if (car->getVT() < frontPtr->getItem()->getVT())
		{
			newNode->setNext(frontPtr);
			frontPtr = newNode;
			return;
		}
		Node<Car*>* curr = frontPtr;
		while (curr->getNext() && curr->getNext()->getItem()->getVT() <= car->getVT())
			curr = curr->getNext();

		newNode->setNext(curr->getNext());
		curr->setNext(newNode);
		if (curr == backPtr)
			backPtr = newNode;
	}
};
