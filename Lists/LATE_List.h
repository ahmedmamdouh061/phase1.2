#pragma once
#include <cstdlib>
#include "../DS/LinkedQueue.h"
#include "../Car.h"

class LATE_List : public LinkedQueue<Car*>
{
public:

	bool CancelRandomCar(Car*& car)
	{
		if (count < 2)
			return false;
		int index = 1 + rand() % (count - 1);
		return RemoveAt(index, car);
	}

private:

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
