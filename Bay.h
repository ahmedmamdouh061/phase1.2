#pragma once
#include <iostream>
#include "Defs.h"
using namespace std;

/*
  Bay: a place inside a unit that serves cars.
  - DiagBay / MechBay / ElecBay serve one car at a time.
  - WashBay can hold several cars at the same time (up to its capacity).
  Busy bays are NOT kept in a list: each car in DIAG/SERV points to its bay.
*/
class Bay
{
protected:
	int ID;				// bay number inside its unit (1, 2, 3, ...)
	UnitType type;		// which unit the bay belongs to
	int busyTime;		// total timesteps the bay was busy

public:
	Bay(int id, UnitType t);
	virtual ~Bay() {}

	int getID() const;
	UnitType getType() const;
	int getBusyTime() const;

	// Letter printed before the ID: D, M, E or W
	char getPrefix() const;

	// Prints the bay as it appears on the output screen, e.g. "D4"
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
	int capacity;		// max cars the bay can hold at the same time (WB_Cap)
	int carsInside;		// cars currently inside the bay

public:
	WashBay(int id, int cap);

	int getCapacity() const;
	int getRemainingCapacity() const;
	bool isFull() const;
	bool addCar();		// returns false if the bay is full
	bool removeCar();	// returns false if the bay is empty

	// Free wash bays are printed with their remaining capacity, e.g. "W2(1)"
	void Print(ostream& out) const override;
};

// Prints any bay through a pointer. Works for DiagBay*, MechBay*, ElecBay*, WashBay*.
ostream& operator<<(ostream& out, const Bay* bay);
