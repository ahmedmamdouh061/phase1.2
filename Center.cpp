#include "Center.h"
#include <cstdlib>

// =====================================================================
//                       construction / destruction
// =====================================================================

Center::Center()
	: currentTime(1), totalCars(0),
	N_DU(0), N_MU(0), N_EU(0), N_WU(0), WB_Cap(0), Pcancel(0), Presc(0),
	diagBays(nullptr), mechBays(nullptr), elecBays(nullptr), washBays(nullptr),
	CNCL(nullptr), FNSH(nullptr)
{
}

Center::~Center()
{
	// Each car is in exactly one list, so deleting the cars of every list deletes each car once.
	Car* car;
	int pri;
	while (IDLE.dequeue(car)) delete car;
	while (ERLY.dequeue(car, pri)) delete car;
	while (LATE.dequeue(car)) delete car;
	while (REDY.dequeue(car)) delete car;
	while (DIAG.dequeue(car, pri)) delete car;
	while (WAIT_MU_List.dequeue(car)) delete car;
	while (WAIT_EU_List.dequeue(car)) delete car;
	while (WAIT_WU_List.dequeue(car)) delete car;
	while (SERV.dequeue(car, pri)) delete car;
	if (CNCL) { while (CNCL->pop(car)) delete car; delete CNCL; }
	if (FNSH) { while (FNSH->pop(car)) delete car; delete FNSH; }

	// Bays: the free lists only point to them, the arrays own them
	for (int i = 0; i < N_DU; i++) delete diagBays[i];
	for (int i = 0; i < N_MU; i++) delete mechBays[i];
	for (int i = 0; i < N_EU; i++) delete elecBays[i];
	for (int i = 0; i < N_WU; i++) delete washBays[i];
	delete[] diagBays;
	delete[] mechBays;
	delete[] elecBays;
	delete[] washBays;
}

// =====================================================================
//                               setup
// =====================================================================

void Center::CreateBays(int nDU, int nMU, int nEU, int nWU, int wbCap)
{
	N_DU = nDU; N_MU = nMU; N_EU = nEU; N_WU = nWU; WB_Cap = wbCap;

	diagBays = new DiagBay*[N_DU];
	for (int i = 0; i < N_DU; i++)
	{
		diagBays[i] = new DiagBay(i + 1);
		Free_DU_List.enqueue(diagBays[i]);
	}
	mechBays = new MechBay*[N_MU];
	for (int i = 0; i < N_MU; i++)
	{
		mechBays[i] = new MechBay(i + 1);
		Free_MU_List.enqueue(mechBays[i]);
	}
	elecBays = new ElecBay*[N_EU];
	for (int i = 0; i < N_EU; i++)
	{
		elecBays[i] = new ElecBay(i + 1);
		Free_EU_List.enqueue(elecBays[i]);
	}
	washBays = new WashBay*[N_WU];
	for (int i = 0; i < N_WU; i++)
	{
		washBays[i] = new WashBay(i + 1, WB_Cap);
		Free_WU_List.enqueue(washBays[i]);
	}
}

void Center::GenerateRandomCars(int n)
{
	totalCars = n;
	CNCL = new ArrayStack<Car*>(n);
	FNSH = new ArrayStack<Car*>(n);

	// Cars are generated sorted by VT, so the IDLE list is sorted by VT
	int vt = 1;
	for (int id = 1; id <= n; id++)
	{
		vt += rand() % 2;						// next car arrives at the same or the next timestep
		int pt = vt + (rand() % 11) - 5;		// appointment within +-5 timesteps of arrival
		if (pt < 1) pt = 1;
		int dd = 1 + rand() % 5;				// diagnosis duration 1..5
		IDLE.enqueue(new Car(id, vt, pt, dd));
	}
}

// =====================================================================
//                     phase 1 random simulator steps
// =====================================================================

// 2.1 Pick the top 50 cars from IDLE and distribute them randomly on LATE, ERLY and REDY
void Center::Step_IdleToArrivalLists()
{
	Car* car;
	for (int i = 0; i < P1_IDLE_PICK && IDLE.dequeue(car); i++)
	{
		switch (rand() % 3)
		{
		case 0:
			car->setStatus(CarStatus::LATE);
			LATE.enqueue(car);
			break;
		case 1:
			car->setStatus(CarStatus::ERLY);
			ERLY.AddCar(car);
			break;
		default:
			car->setStatus(CarStatus::REDY);
			REDY.enqueue(car);
			break;
		}
	}
}

// 2.2 Move top 6 cars from ERLY to REDY
void Center::Step_ErlyToRedy()
{
	Car* car;
	int pri;
	for (int i = 0; i < P1_ERLY_TO_REDY && ERLY.dequeue(car, pri); i++)
	{
		car->setStatus(CarStatus::REDY);
		REDY.enqueue(car);
	}
}

// 2.3 With probability 10%, pick one random car (not the top one) from LATE and move it to CNCL
void Center::Step_LateRandomCancel()
{
	if (rand() % 100 >= P1_LATE_CANCEL_PROB)
		return;
	Car* car;
	if (LATE.CancelRandomCar(car))
		CancelCar(car);
}

// 2.4 Move top 6 cars from LATE to REDY
void Center::Step_LateToRedy()
{
	Car* car;
	for (int i = 0; i < P1_LATE_TO_REDY && LATE.dequeue(car); i++)
	{
		car->setStatus(CarStatus::REDY);
		REDY.enqueue(car);
	}
}

// 2.5 For each available DIAG bay, assign one car from REDY
// 2.6 Diagnosis takes 3 timesteps for each car
void Center::Step_AssignDiagBays()
{
	DiagBay* bay;
	Car* car;
	while (!Free_DU_List.isEmpty() && !REDY.isEmpty())
	{
		Free_DU_List.dequeue(bay);
		REDY.dequeue(car);
		int endTime = currentTime + P1_DIAG_TIME;
		car->AssignBay(bay, endTime);
		car->setStatus(CarStatus::DIAG);
		DIAG.enqueue(car, -endTime);		// earliest end time = highest priority
	}
}

// 2.7 Pick all cars that spent 3 timesteps in DIAG and move them randomly to one of the
//     WAIT lists, or to CNCL with probability 20%. Free their DIAG bays.
void Center::Step_FinishDiagnosis()
{
	Car* car;
	int pri;
	while (DIAG.peek(car, pri) && -pri <= currentTime)
	{
		DIAG.dequeue(car, pri);
		Bay* bay = car->ReleaseBay();
		ReturnBay(bay);

		car->GenerateRandomServices();		// the diagnosis unit assigns random services

		if (rand() % 100 < P1_DIAG_CANCEL_PROB)
			CancelCar(car);
		else
			MoveToRandomWaitList(car);
	}
}

// 2.8 Pick top 6 cars from WAIT lists and assign each to a random bay with
//     probability MB = 60%, EB = 30%, WB = 10%.
//     If there is no available bay to assign, don't pick the car from the WAIT list.
// 2.9 Any service takes 4 timesteps.
void Center::Step_AssignServiceBays()
{
	for (int i = 0; i < P1_WAIT_PICK; i++)
	{
		UnitType t = RandomBayType();
		WaitList& waitList = GetWaitList(t);
		if (waitList.isEmpty())
			continue;

		Bay* bay = TakeFreeBay(t);
		if (!bay)
			continue;			// no free bay: the car stays in the WAIT list

		Car* car;
		waitList.dequeue(car);
		int endTime = currentTime + P1_SERV_TIME;
		car->AssignBay(bay, endTime);
		car->setStatus(CarStatus::SERV);
		SERV.enqueue(car, -endTime);		// earliest end time = highest priority
	}
}

// 2.10 Pick all cars that spent 4 timesteps in SERV and move each of them to FNSH with
//      probability 40%, or to a WAIT list again. Free their bays.
void Center::Step_FinishService()
{
	Car* car;
	int pri;
	while (SERV.peek(car, pri) && -pri <= currentTime)
	{
		SERV.dequeue(car, pri);
		Bay* bay = car->ReleaseBay();
		UnitType servedBy = bay->getType();
		ReturnBay(bay);

		car->CompleteService(servedBy);

		if (rand() % 100 < P1_FINISH_PROB)
			FinishCar(car);
		else
			MoveToRandomWaitList(car);
	}
}

void Center::SimulateRandomTimestep()
{
	Step_IdleToArrivalLists();	// 2.1
	Step_ErlyToRedy();			// 2.2
	Step_LateRandomCancel();	// 2.3
	Step_LateToRedy();			// 2.4
	Step_AssignDiagBays();		// 2.5, 2.6
	Step_FinishDiagnosis();		// 2.7
	Step_AssignServiceBays();	// 2.8, 2.9
	Step_FinishService();		// 2.10
}

void Center::RunRandomSimulation()
{
	CreateBays(P1_N_DU, P1_N_MU, P1_N_EU, P1_N_WU, P1_WB_CAP);
	GenerateRandomCars(P1_NUM_CARS);		// 1. at least 1000 IDLE cars
	currentTime = 1;

	ui.PrintStartMessage();

	bool stepByStep = true;				// false after the user presses E

	while (true)
	{
		SimulateRandomTimestep();		// 2.1 .. 2.10

		if (stepByStep)
			PrintInterface();			// 2.11

		if (AllCarsDone())				// 3. all cars are in FNSH or CNCL
			break;

		if (stepByStep)
		{
			stepByStep = ui.WaitForKey();	// any key: next step, E: go to the end
			if (!stepByStep)
				ui.PrintSkippingToEnd();
		}
		currentTime++;
	}

	if (!stepByStep)
		PrintInterface();				// show the lists at the last timestep

	ui.PrintSimulationEnd(currentTime, FNSH->getCount(), CNCL->getCount());
}

// =====================================================================
//                                helpers
// =====================================================================

WaitList& Center::GetWaitList(UnitType t)
{
	switch (t)
	{
	case UnitType::EU: return WAIT_EU_List;
	case UnitType::WU: return WAIT_WU_List;
	default:           return WAIT_MU_List;
	}
}

// Phase 1: the car joins a random WAIT list. If the car has no remaining service of that
// type, one is added, so its "next Serv Time" in the WAIT list is always meaningful.
// The list is chosen with the same odds as the bays in step 2.8 (MU 60%, EU 30%, WU 10%).
// With equal odds (1/3 each) a third of the cars wait for the wash unit, but a wash bay is
// picked only 10% of the time, so the WB list keeps growing and the run takes ~1400 steps.
// To use equal odds instead, replace RandomBayType() with RandomServiceUnit().
void Center::MoveToRandomWaitList(Car* car)
{
	UnitType t = RandomBayType();
	if (!car->HasService(t))
		car->AddService(t, 1 + rand() % 10);

	car->setCurrentUnit(t);			// must be set before enqueue (WAIT total uses it)
	car->setStatus(CarStatus::WAIT);
	GetWaitList(t).enqueue(car);
}

Bay* Center::TakeFreeBay(UnitType t)
{
	switch (t)
	{
	case UnitType::MU:
	{
		MechBay* bay;
		return Free_MU_List.dequeue(bay) ? bay : nullptr;
	}
	case UnitType::EU:
	{
		ElecBay* bay;
		return Free_EU_List.dequeue(bay) ? bay : nullptr;
	}
	case UnitType::WU:
	{
		// A wash bay stays in the free list while it has capacity left
		WashBay* bay;
		if (!Free_WU_List.peek(bay))
			return nullptr;
		bay->addCar();
		if (bay->isFull())
			Free_WU_List.dequeue(bay);
		return bay;
	}
	case UnitType::DU:
	{
		DiagBay* bay;
		return Free_DU_List.dequeue(bay) ? bay : nullptr;
	}
	}
	return nullptr;
}

void Center::ReturnBay(Bay* bay)
{
	switch (bay->getType())
	{
	case UnitType::DU:
		Free_DU_List.enqueue(static_cast<DiagBay*>(bay));
		break;
	case UnitType::MU:
		Free_MU_List.enqueue(static_cast<MechBay*>(bay));
		break;
	case UnitType::EU:
		Free_EU_List.enqueue(static_cast<ElecBay*>(bay));
		break;
	case UnitType::WU:
	{
		// A full wash bay is not in the free list; it goes back once a car leaves it
		WashBay* wb = static_cast<WashBay*>(bay);
		bool wasFull = wb->isFull();
		wb->removeCar();
		if (wasFull)
			Free_WU_List.enqueue(wb);
		break;
	}
	}
}

void Center::CancelCar(Car* car)
{
	car->Cancel(currentTime);
	CNCL->push(car);
}

void Center::FinishCar(Car* car)
{
	car->Finish(currentTime);
	FNSH->push(car);
}

UnitType Center::RandomServiceUnit() const
{
	return static_cast<UnitType>(1 + rand() % 3);	// MU, EU or WU
}

UnitType Center::RandomBayType() const
{
	int r = rand() % 100;
	if (r < P1_MB_PROB)
		return UnitType::MU;					// 60%
	if (r < P1_MB_PROB + P1_EB_PROB)
		return UnitType::EU;					// 30%
	return UnitType::WU;						// 10%
}

bool Center::AllCarsDone() const
{
	return FNSH->getCount() + CNCL->getCount() == totalCars;
}

// =====================================================================
//                       interactive mode screen
// =====================================================================

void Center::PrintInterface() const
{
	ui.PrintTimestep(currentTime);

	ui.PrintSectionHeader("IDLE Cars [CarID, VT, PT]");
	ui.PrintList("cars", IDLE, " ", IDLE_PRINT_LIMIT);		// ONLY the first 15 cars

	ui.PrintSectionHeader("ERLY & LATE & REDY Cars [CarID, VT, PT]");
	ui.PrintList("ERLY", ERLY);
	ui.PrintList("LATE", LATE);
	ui.PrintList("REDY", REDY);

	ui.PrintSubHeader("Free Diagnosis Bays IDs");
	ui.PrintList("Diag Bays", Free_DU_List, ", ");

	ui.PrintSectionHeader("DIAG Cars [CarID, Diag Bay ID]");
	ui.PrintList("Cars in Diag", DIAG);

	ui.PrintSectionHeader("Cars waiting for Service [CarID, next Serv Time]");
	ui.PrintList("MB cars waiting", WAIT_MU_List);
	ui.PrintList("EB cars waiting", WAIT_EU_List);
	ui.PrintList("WB cars waiting", WAIT_WU_List);

	ui.PrintSubHeader("Free Bays IDs");
	ui.PrintList("MB free", Free_MU_List, ", ");
	ui.PrintList("EB free", Free_EU_List, ", ");
	ui.PrintList("WB free [ID(remaining capacity)]", Free_WU_List, ", ");

	ui.PrintSectionHeader("In-Service Cars [CarID, Bay ID]");
	ui.PrintList("Cars in Serv", SERV);

	ui.PrintSectionHeader("CNCL Cars [CarID, CancelTime]");
	ui.PrintList("cancelled", *CNCL);

	ui.PrintSectionHeader("FNSH Cars [CarID, FinishTime]");
	ui.PrintList("DONE", *FNSH);
}
