#pragma once
#include <iostream>
#include "Defs.h"
#include "Lists/RequiredServices.h"
using namespace std;

class Bay;

class Car
{
private:

	int ID;
	int VT;
	int PT;
	int DD;

	RequiredServices services;
	SequenceType seqType;

	CarStatus status;
	Bay* assignedBay;
	UnitType currentUnit;
	int currentEndTime;

	int FT;
	int CT;
	int TW;
	int TS;
	bool rescheduled;

public:
	Car(int id, int vt, int pt, int dd);

	int getID() const;
	int getVT() const;
	int getPT() const;
	int getDD() const;
	CarStatus getStatus() const;
	SequenceType getSequenceType() const;
	Bay* getBay() const;
	UnitType getCurrentUnit() const;
	int getCurrentEndTime() const;
	int getFT() const;
	int getCT() const;
	int getTW() const;
	int getTS() const;
	bool isRescheduled() const;

	void setStatus(CarStatus s);

	void AssignBay(Bay* bay, int endTime);

	Bay* ReleaseBay();

	void setCurrentUnit(UnitType u);

	void GenerateRandomServices();
	void AddService(UnitType type, int duration);
	bool HasService(UnitType type) const;
	bool HasServicesLeft() const;
	int getServicesCount() const;

	int getNextServiceDuration() const;

	void CompleteService(UnitType t);

	void Finish(int timestep);
	void Cancel(int timestep);

	void Print(ostream& out) const;
};

ostream& operator<<(ostream& out, const Car* car);
