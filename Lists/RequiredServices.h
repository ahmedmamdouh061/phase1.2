#pragma once
#include "../DS/LinkedQueue.h"
#include "../Defs.h"

struct Service
{
	UnitType type;
	int duration;

	Service() : type(UnitType::MU), duration(0) {}
	Service(UnitType t, int d) : type(t), duration(d) {}
};

class RequiredServices : public LinkedQueue<Service>
{
public:

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

	bool HasType(UnitType t) const
	{
		for (Node<Service>* p = frontPtr; p; p = p->getNext())
			if (p->getItem().type == t)
				return true;
		return false;
	}

	int GetDurationOfType(UnitType t) const
	{
		for (Node<Service>* p = frontPtr; p; p = p->getNext())
			if (p->getItem().type == t)
				return p->getItem().duration;
		return 0;
	}
};
