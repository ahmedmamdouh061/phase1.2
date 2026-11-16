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

class Center
{
private:
	int currentTime;
	int totalCars;

	int N_DU, N_MU, N_EU, N_WU;
	int WB_Cap;
	int Pcancel, Presc;

	DiagBay** diagBays;
	MechBay** mechBays;
	ElecBay** elecBays;
	WashBay** washBays;

	IDLE_List IDLE;
	ERLY_List ERLY;
	LATE_List LATE;
	LinkedQueue<Car*> REDY;

	LinkedQueue<DiagBay*> Free_DU_List;
	priQueue<Car*> DIAG;

	WAIT_MU WAIT_MU_List;
	WAIT_EU WAIT_EU_List;
	WAIT_WU WAIT_WU_List;

	LinkedQueue<MechBay*> Free_MU_List;
	LinkedQueue<ElecBay*> Free_EU_List;
	LinkedQueue<WashBay*> Free_WU_List;

	priQueue<Car*> SERV;

	ArrayStack<Car*>* CNCL;
	ArrayStack<Car*>* FNSH;

	UI ui;

	void CreateBays(int nDU, int nMU, int nEU, int nWU, int wbCap);
	void GenerateRandomCars(int n);

	void Step_IdleToArrivalLists();
	void Step_ErlyToRedy();
	void Step_LateRandomCancel();
	void Step_LateToRedy();
	void Step_AssignDiagBays();
	void Step_FinishDiagnosis();
	void Step_AssignServiceBays();
	void Step_FinishService();
	void SimulateRandomTimestep();

	WaitList& GetWaitList(UnitType t);
	void MoveToRandomWaitList(Car* car);
	Bay* TakeFreeBay(UnitType t);
	void ReturnBay(Bay* bay);
	void CancelCar(Car* car);
	void FinishCar(Car* car);
	UnitType RandomServiceUnit() const;
	UnitType RandomBayType() const;
	bool AllCarsDone() const;

	void PrintInterface() const;

public:
	Center();
	~Center();

	void RunRandomSimulation();

	Center(const Center&) = delete;
	Center& operator=(const Center&) = delete;
};
