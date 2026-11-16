#pragma once
#include "../DS/LinkedQueue.h"
#include "../Car.h"

class WaitList : public LinkedQueue<Car*>
{
protected:
	int totalServiceTime;

public:
	WaitList() : totalServiceTime(0) {}

	bool enqueue(Car* const& car) override
	{
		LinkedQueue<Car*>::enqueue(car);
		totalServiceTime += car->getNextServiceDuration();
		return true;
	}

	bool dequeue(Car*& car) override
	{
		if (!LinkedQueue<Car*>::dequeue(car))
			return false;
		totalServiceTime -= car->getNextServiceDuration();
		return true;
	}

	int GetTotalServiceTime() const
	{
		return totalServiceTime;
	}
};

class WAIT_MU : public WaitList {};
class WAIT_EU : public WaitList {};
class WAIT_WU : public WaitList {};
