#pragma once
#include <cstdlib>
#include "../DS/priQueue.h"
#include "../Car.h"

/*
  ERLY List (member of Center)
  A class derived from Priority Queue.
  Why Priority Queue: early cars must be ordered by appointment time (PT), so the
                      car with the earliest PT is checked first.
  Why derived: to remove a random car when it reschedules (Presc).

  The lab priQueue puts the HIGHEST priority first, so we use priority = -PT
  (smallest PT -> highest priority).

  Operations: enqueue (sorted by PT) O(n), dequeue O(1), peek O(1),
              RemoveRandomCar O(n), print all O(n)
*/
class ERLY_List : public priQueue<Car*>
{
public:
	// Adds an early car in its place according to its PT. O(n)
	void AddCar(Car* car)
	{
		enqueue(car, -car->getPT());
	}

	// Removes a random car (any position). Used for rescheduling. O(n)
	bool RemoveRandomCar(Car*& car)
	{
		if (isEmpty())
			return false;
		return RemoveAt(rand() % count, car);
	}

private:
	// Removes the car at position index (0 = head). O(n)
	bool RemoveAt(int index, Car*& car)
	{
		if (index < 0 || index >= count)
			return false;

		priNode<Car*>* prev = nullptr;
		priNode<Car*>* curr = head;
		for (int i = 0; i < index; i++)
		{
			prev = curr;
			curr = curr->getNext();
		}
		int pri;
		car = curr->getItem(pri);
		if (prev)
			prev->setNext(curr->getNext());
		else
			head = curr->getNext();
		delete curr;
		count--;
		return true;
	}
};
