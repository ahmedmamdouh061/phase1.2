#pragma once
#include "../DS/LinkedQueue.h"
#include "../Defs.h"

// One required service: its type (MU / EU / WU) and its duration
struct Service
{
	UnitType type;
	int duration;

	Service() : type(UnitType::MU), duration(0) {}
	Service(UnitType t, int d) : type(t), duration(d) {}
};

/*
  Required Services List (member of Car)
  A class derived from Queue (of service type and duration).
  Why Queue: an SS car must take its services in order.
  Why derived: an FS car may take any remaining service, so we add
               RemoveByType() to remove a service of a chosen type.

  Operations: enqueue O(1), dequeue O(1), RemoveByType O(k), getCount O(1)
  (k = the car's number of services)
*/
class RequiredServices : public LinkedQueue<Service>
{
public:
	// Removes the first service of the given type. O(k)
	bool RemoveByType(UnitType t, Service& removed)
	{
		Node<Service>* prev = nullptr;
		Node<Service>* curr = frontPtr;
		while (curr)
		{
			if (curr->getItem().type == t)
			{
				removed = curr->getItem();
				if (prev)
					prev->setNext(curr->getNext());
				else
					frontPtr = curr->getNext();
				if (curr == backPtr)
					backPtr = prev;
				delete curr;
				count--;
				return true;
			}
			prev = curr;
			curr = curr->getNext();
		}
		return false;
	}

	// Is there any remaining service of this type? O(k)
	bool HasType(UnitType t) const
	{
		for (Node<Service>* p = frontPtr; p; p = p->getNext())
			if (p->getItem().type == t)
				return true;
		return false;
	}

	// Returns the duration of the first service of this type, or 0. O(k)
	int GetDurationOfType(UnitType t) const
	{
		for (Node<Service>* p = frontPtr; p; p = p->getNext())
			if (p->getItem().type == t)
				return p->getItem().duration;
		return 0;
	}
};
