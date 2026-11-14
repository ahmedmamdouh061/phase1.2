#pragma once
#include "../DS/LinkedQueue.h"
#include "../Car.h"

/*
  Service Waiting Lists: WAIT_MU, WAIT_EU, WAIT_WU (members of Center)
  Three classes derived from Queue.
  Why Queue: cars are picked FCFS when a bay of that unit becomes free.
  Why derived: to keep a running total of the service time of the waiting cars,
               so an FS car can pick the list with the minimum total in O(1).

  Operations: enqueue + update total O(1), dequeue + update total O(1),
              GetTotalServiceTime O(1), print all O(n)

  The car must have its current unit set (Car::setCurrentUnit) before it is
  enqueued, so getNextServiceDuration() returns the service it waits for.
  That value does not change while the car is waiting, so the same amount is
  added on enqueue and subtracted on dequeue.
*/
class WaitList : public LinkedQueue<Car*>
{
protected:
	int totalServiceTime;		// sum of the next-service durations of the waiting cars

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

class WAIT_MU : public WaitList {};		// cars waiting for the Mechanical Unit
class WAIT_EU : public WaitList {};		// cars waiting for the Electrical Unit
class WAIT_WU : public WaitList {};		// cars waiting for the Wash Unit
