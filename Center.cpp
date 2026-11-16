#include "Center.h"
#include <cstdlib>

Center::Center()
	: currentTime(1), totalCars(0),
	N_DU(0), N_MU(0), N_EU(0), N_WU(0), WB_Cap(0), Pcancel(0), Presc(0),
	diagBays(nullptr), mechBays(nullptr), elecBays(nullptr), washBays(nullptr),
	CNCL(nullptr), FNSH(nullptr)
{
}

Center::~Center()
{
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

	for (int i = 0; i < N_DU; i++) delete diagBays[i];
	for (int i = 0; i < N_MU; i++) delete mechBays[i];
	for (int i = 0; i < N_EU; i++) delete elecBays[i];
	for (int i = 0; i < N_WU; i++) delete washBays[i];
	delete[] diagBays;
	delete[] mechBays;
	delete[] elecBays;
	delete[] washBays;
}

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

	int vt = 1;
	for (int id = 1; id <= n; id++)
	{
		vt += rand() % 2;
		int pt = vt + (rand() % 11) - 5;
		if (pt < 1) pt = 1;
		int dd = 1 + rand() % 5;
		IDLE.enqueue(new Car(id, vt, pt, dd));
	}
}

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

void Center::Step_LateRandomCancel()
{
	if (rand() % 100 >= P1_LATE_CANCEL_PROB)
		return;
	Car* car;
	if (LATE.CancelRandomCar(car))
		CancelCar(car);
}

void Center::Step_LateToRedy()
{
	Car* car;
	for (int i = 0; i < P1_LATE_TO_REDY && LATE.dequeue(car); i++)
	{
		car->setStatus(CarStatus::REDY);
		REDY.enqueue(car);
	}
}

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
		DIAG.enqueue(car, -endTime);
	}
}

void Center::Step_FinishDiagnosis()
{
	Car* car;
	int pri;
	while (DIAG.peek(car, pri) && -pri <= currentTime)
	{
		DIAG.dequeue(car, pri);
		Bay* bay = car->ReleaseBay();
		ReturnBay(bay);

		car->GenerateRandomServices();

		if (rand() % 100 < P1_DIAG_CANCEL_PROB)
			CancelCar(car);
		else
			MoveToRandomWaitList(car);
	}
}

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
			continue;

		Car* car;
		waitList.dequeue(car);
		int endTime = currentTime + P1_SERV_TIME;
		car->AssignBay(bay, endTime);
		car->setStatus(CarStatus::SERV);
		SERV.enqueue(car, -endTime);
	}
}

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
	Step_IdleToArrivalLists();
	Step_ErlyToRedy();
	Step_LateRandomCancel();
	Step_LateToRedy();
	Step_AssignDiagBays();
	Step_FinishDiagnosis();
	Step_AssignServiceBays();
	Step_FinishService();
}

void Center::RunRandomSimulation()
{
	CreateBays(P1_N_DU, P1_N_MU, P1_N_EU, P1_N_WU, P1_WB_CAP);
	GenerateRandomCars(P1_NUM_CARS);
	currentTime = 1;

	ui.PrintStartMessage();

	bool stepByStep = true;

	while (true)
	{
		SimulateRandomTimestep();

		if (stepByStep)
			PrintInterface();

		if (AllCarsDone())
			break;

		if (stepByStep)
		{
			stepByStep = ui.WaitForKey();
			if (!stepByStep)
				ui.PrintSkippingToEnd();
		}
		currentTime++;
	}

	if (!stepByStep)
		PrintInterface();

	ui.PrintSimulationEnd(currentTime, FNSH->getCount(), CNCL->getCount());
}

WaitList& Center::GetWaitList(UnitType t)
{
	switch (t)
	{
	case UnitType::EU: return WAIT_EU_List;
	case UnitType::WU: return WAIT_WU_List;
	default:           return WAIT_MU_List;
	}
}

void Center::MoveToRandomWaitList(Car* car)
{
	UnitType t = RandomBayType();
	if (!car->HasService(t))
		car->AddService(t, 1 + rand() % 10);

	car->setCurrentUnit(t);
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
	return static_cast<UnitType>(1 + rand() % 3);
}

UnitType Center::RandomBayType() const
{
	int r = rand() % 100;
	if (r < P1_MB_PROB)
		return UnitType::MU;
	if (r < P1_MB_PROB + P1_EB_PROB)
		return UnitType::EU;
	return UnitType::WU;
}

bool Center::AllCarsDone() const
{
	return FNSH->getCount() + CNCL->getCount() == totalCars;
}

void Center::PrintInterface() const
{
	ui.PrintTimestep(currentTime);

	ui.PrintSectionHeader("IDLE Cars [CarID, VT, PT]");
	ui.PrintList("cars", IDLE, " ", IDLE_PRINT_LIMIT);

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
