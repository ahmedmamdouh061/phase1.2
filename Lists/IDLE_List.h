#pragma once
#include "../DS/LinkedQueue.h"
#include "../Car.h"

class IDLE_List : public LinkedQueue<Car*>
{
public:

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
