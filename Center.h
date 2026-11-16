#pragma once
#include "Defs.h"
#include "Car.h"
#include "Bay.h"
#include "UI.h"

#include "DS/LinkedQueue.h"
#include "DS/priQueue.h"
#include "DS/ArrayStack.h"

#include "Lists/IDLE_List.h"
#include "Lists/ERLY_List.h"
#include "Lists/LATE_List.h"
#include "Lists/WaitLists.h"

/*
  Center: the car service center. It owns all cars, all bays and all lists,
  and runs the simulation one timestep at a time.

  All lists hold POINTERS. Each car / bay is allocated once (SHARE, MOVE, Don't Copy).
  Busy bays are not kept in any list: each car in DIAG/SERV points to its bay.
*/
class Center
{
private:
	int currentTime;		// current timestep (starts at 1)
	int totalCars;			// number of car requests

	// ---------------- units info ----------------
	int N_DU, N_MU, N_EU, N_WU;	// number of bays in each unit
	int WB_Cap;					// capacity of each wash bay
	int Pcancel, Presc;			// cancel / reschedule probabilities (0..100)

	// All bays (to delete them at the end). The free lists below point to the same objects.
	DiagBay** diagBays;
	MechBay** mechBays;
	ElecBay** elecBays;
	WashBay** washBays;

	// ---------------- lists ----------------
	IDLE_List IDLE;							// cars that have not arrived yet (sorted by VT)
	ERLY_List ERLY;							// early cars sorted by PT
	LATE_List LATE;							// late cars (FIFO)
	LinkedQueue<Car*> REDY;					// cars ready for diagnosis (FIFO)

	LinkedQueue<DiagBay*> Free_DU_List;		// free diagnosis bays
	priQueue<Car*> DIAG;					// cars being diagnosed, by diagnosis end time

	WAIT_MU WAIT_MU_List;					// cars waiting for a mechanical bay
	WAIT_EU WAIT_EU_List;					// cars waiting for an electrical bay
	WAIT_WU WAIT_WU_List;					// cars waiting for a wash bay

	LinkedQueue<MechBay*> Free_MU_List;		// free mechanical bays
	LinkedQueue<ElecBay*> Free_EU_List;		// free electrical bays
	LinkedQueue<WashBay*> Free_WU_List;		// wash bays that still have free capacity

	priQueue<Car*> SERV;					// cars in service, by service end time

	ArrayStack<Car*>* CNCL;					// cancelled cars (top = latest cancel time)
	ArrayStack<Car*>* FNSH;					// finished cars (top = latest finish time)

	UI ui;

	// ---------------- setup ----------------
	void CreateBays(int nDU, int nMU, int nEU, int nWU, int wbCap);
	void GenerateRandomCars(int n);			// phase 1: at least 1000 random IDLE cars

	// ---------------- phase 1 random simulator steps (project document, page "Random Simulation") ----------------
	void Step_IdleToArrivalLists();		// 2.1
	void Step_ErlyToRedy();				// 2.2
	void Step_LateRandomCancel();		// 2.3
	void Step_LateToRedy();				// 2.4
	void Step_AssignDiagBays();			// 2.5 + 2.6
	void Step_FinishDiagnosis();		// 2.7
	void Step_AssignServiceBays();		// 2.8 + 2.9
	void Step_FinishService();			// 2.10
	void SimulateRandomTimestep();

	// ---------------- helpers ----------------
	WaitList& GetWaitList(UnitType t);
	void MoveToRandomWaitList(Car* car);
	Bay* TakeFreeBay(UnitType t);		// nullptr if no free bay of that type
	void ReturnBay(Bay* bay);			// puts a bay back in its free list
	void CancelCar(Car* car);
	void FinishCar(Car* car);
	UnitType RandomServiceUnit() const;	// MU, EU or WU with equal chance
	UnitType RandomBayType() const;		// MB 60%, EB 30%, WB 10%
	bool AllCarsDone() const;

	void PrintInterface() const;		// the interactive mode screen (2.11)

public:
	Center();
	~Center();

	// Runs the phase 1 random simulator
	void RunRandomSimulation();

	// No copies of the center (it owns all cars and bays)
	Center(const Center&) = delete;
	Center& operator=(const Center&) = delete;
};
