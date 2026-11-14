#pragma once
#include <iostream>
#include "Defs.h"
#include "Lists/RequiredServices.h"
using namespace std;

class Bay;

/*
  Car: one car request.
  Every Car object is allocated ONCE. All lists hold Car* pointers, so moving a
  car from one list to another only moves the pointer (SHARE, MOVE, Don't Copy).
*/
class Car
{
private:
	// ---- loaded from the input file (phase 2) / generated randomly (phase 1) ----
	int ID;					// CID
	int VT;					// arrival timestep
	int PT;					// appointment timestep
	int DD;					// diagnosis duration

	// ---- set by the diagnosis unit ----
	RequiredServices services;	// required services [type & duration]
	SequenceType seqType;		// SS or FS

	// ---- current state ----
	CarStatus status;
	Bay* assignedBay;		// the bay serving the car while it is in DIAG or SERV (else nullptr)
	UnitType currentUnit;	// the unit whose WAIT list / bay the car is in now
	int currentEndTime;		// timestep the current diagnosis / service ends

	// ---- statistics ----
	int TW;					// total waiting time (outside service bays)
	int TS;					// total service time (inside service bays)
	int FT;					// finish timestep (= cancel timestep for cancelled cars)
	int CT;					// cancel timestep (-1 if not cancelled)
	bool rescheduled;		// did the car reschedule its appointment?

public:
	Car(int id, int vt, int pt, int dd);

	// ---------- getters ----------
	int getID() const;
	int getVT() const;
	int getPT() const;
	int getDD() const;
	CarStatus getStatus() const;
	SequenceType getSequenceType() const;
	Bay* getBay() const;
	UnitType getCurrentUnit() const;
	int getCurrentEndTime() const;
	int getTW() const;
	int getTS() const;
	int getFT() const;
	int getCT() const;
	bool isRescheduled() const;

	// ---------- state changes ----------
	void setStatus(CarStatus s);

	// Puts the car inside a diagnosis or service bay until endTime
	void AssignBay(Bay* bay, int endTime);

	// Takes the car out of its bay and returns the bay so it can be freed
	Bay* ReleaseBay();

	// Marks which WAIT list (MU / EU / WU) the car joins
	void setCurrentUnit(UnitType u);

	// ---------- required services ----------
	// Diagnosis: create a RANDOM list of services and a random SS/FS type
	void GenerateRandomServices();
	void AddService(UnitType type, int duration);
	bool HasService(UnitType type) const;
	bool HasServicesLeft() const;
	int getServicesCount() const;

	// Type of the next service in the list (used for SS cars). false if none left.
	bool getFirstServiceType(UnitType& type) const;

	// Duration of the service the car is waiting for now (shown in WAIT lists)
	int getNextServiceDuration() const;

	// Removes the service of type t from the list after it is done, adds the time to TS
	void CompleteService(UnitType t, int timeSpent);

	// ---------- end of the car's life ----------
	void Finish(int timestep);		// FNSH: FT = timestep, TW = FT - VT - TS
	void Cancel(int timestep);		// CNCL: FT = CT = timestep

	// Early car reschedules: new VT and PT (PT_new > PT_old), goes back to IDLE
	void Reschedule(int newVT, int newPT);

	// Prints the car the way the output screen needs it, based on its status:
	// IDLE/ERLY/LATE/REDY: [CID, VT, PT]   DIAG/SERV: [CID, BayID]
	// WAIT: [CID, next serv time]          CNCL: [CID, CancelTime]   FNSH: [CID, FinishTime]
	void Print(ostream& out) const;
};

ostream& operator<<(ostream& out, const Car* car);
