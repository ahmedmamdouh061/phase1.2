#pragma once
#include <cstdlib>
#include "../DS/LinkedQueue.h"
#include "../Car.h"

/*
  LATE List (member of Center)
  A class derived from Queue.
  Why Queue: late cars wait in the order they arrive (FIFO).
  Why derived: to remove a random car when it cancels (Pcancel).

  Operations: enqueue O(1), dequeue O(1), peek O(1), CancelRandomCar O(n), print all O(n)
*/
class LATE_List : public LinkedQueue<Car*>
{
public:
	// Removes a random car from the list.
	// canPickFirst = false -> the first car is never picked (phase 1 rule 2.3:
	//                         "pick one random car (not the top one)").
	// canPickFirst = true  -> any car can be picked (phase 2 rule).   O(n)
	bool CancelRandomCar(Car*& car, bool canPickFirst = true)
	{
		int first = canPickFirst ? 0 : 1;
		if (count <= first)
			return false;			// no car can be picked
		int index = first + rand() % (count - first);
		return RemoveAt(index, car);
	}

private:
	// Removes the car at position index (0 = front). O(n)
	bool RemoveAt(int index, Car*& car)
	{
		if (index < 0 || index >= count)
			return false;
		if (index == 0)
			return dequeue(car);

		Node<Car*>* prev = frontPtr;
		for (int i = 0; i < index - 1; i++)
			prev = prev->getNext();

		Node<Car*>* target = prev->getNext();
		car = target->getItem();
		prev->setNext(target->getNext());
		if (target == backPtr)
			backPtr = prev;
		delete target;
		count--;
		return true;
	}
};
