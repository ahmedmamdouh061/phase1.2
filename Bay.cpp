#include "Bay.h"

// ======================= Bay =======================

Bay::Bay(int id, UnitType t) : ID(id), type(t)
{
}

int Bay::getID() const { return ID; }

UnitType Bay::getType() const { return type; }

char Bay::getPrefix() const
{
	switch (type)
	{
	case UnitType::DU: return 'D';
	case UnitType::MU: return 'M';
	case UnitType::EU: return 'E';
	case UnitType::WU: return 'W';
	}
	return '?';
}

void Bay::Print(ostream& out) const
{
	out << getPrefix() << ID;
}

// ================== Derived bays ==================

DiagBay::DiagBay(int id) : Bay(id, UnitType::DU) {}

MechBay::MechBay(int id) : Bay(id, UnitType::MU) {}

ElecBay::ElecBay(int id) : Bay(id, UnitType::EU) {}

WashBay::WashBay(int id, int cap) : Bay(id, UnitType::WU), capacity(cap > 0 ? cap : 1), carsInside(0)
{
}

int WashBay::getCapacity() const { return capacity; }

int WashBay::getRemainingCapacity() const { return capacity - carsInside; }

bool WashBay::isFull() const { return carsInside >= capacity; }

bool WashBay::addCar()
{
	if (isFull())
		return false;
	carsInside++;
	return true;
}

bool WashBay::removeCar()
{
	if (carsInside == 0)
		return false;
	carsInside--;
	return true;
}

void WashBay::Print(ostream& out) const
{
	out << getPrefix() << ID << "(" << getRemainingCapacity() << ")";
}

// ================== operator<< ==================

ostream& operator<<(ostream& out, const Bay* bay)
{
	if (bay)
		bay->Print(out);
	else
		out << "null";
	return out;
}
