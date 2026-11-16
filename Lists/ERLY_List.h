#pragma once
#include <cstdlib>
#include "../DS/priQueue.h"
#include "../Car.h"

class ERLY_List : public priQueue<Car*>
{
public:

	void AddCar(Car* car)
	{
		enqueue(car, -car->getPT());
	}

	bool RemoveRandomCar(Car*& car)
	{
		if (isEmpty())
			return false;
		return RemoveAt(rand() % count, car);
	}

private:

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
