#pragma once
#include <iostream>
#include "Defs.h"
using namespace std;

class Bay
{
protected:
	int ID;
	UnitType type;
	int busyTime;

public:
	Bay(int id, UnitType t);
	virtual ~Bay() {}

	int getID() const;
	UnitType getType() const;
	int getBusyTime() const;

	char getPrefix() const;

	virtual void Print(ostream& out) const;
};

class DiagBay : public Bay
{
public:
	DiagBay(int id);
};

class MechBay : public Bay
{
public:
	MechBay(int id);
};

class ElecBay : public Bay
{
public:
	ElecBay(int id);
};

class WashBay : public Bay
{
private:
	int capacity;
	int carsInside;

public:
	WashBay(int id, int cap);

	int getCapacity() const;
	int getRemainingCapacity() const;
	bool isFull() const;
	bool addCar();
	bool removeCar();

	void Print(ostream& out) const override;
};

ostream& operator<<(ostream& out, const Bay* bay);
