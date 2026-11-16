#include "Car.h"
#include "Bay.h"
#include <cstdlib>

Car::Car(int id, int vt, int pt, int dd)
	: ID(id), VT(vt), PT(pt), DD(dd),
	seqType(SequenceType::SS), status(CarStatus::IDLE), assignedBay(nullptr),
	currentUnit(UnitType::DU), currentEndTime(-1),
	FT(-1), CT(-1), TW(0), TS(0), rescheduled(false)
{
}

int Car::getID() const { return ID; }
int Car::getVT() const { return VT; }
int Car::getPT() const { return PT; }
int Car::getDD() const { return DD; }
CarStatus Car::getStatus() const { return status; }
SequenceType Car::getSequenceType() const { return seqType; }
Bay* Car::getBay() const { return assignedBay; }
UnitType Car::getCurrentUnit() const { return currentUnit; }
int Car::getCurrentEndTime() const { return currentEndTime; }
int Car::getFT() const { return FT; }
int Car::getCT() const { return CT; }
int Car::getTW() const { return TW; }
int Car::getTS() const { return TS; }
bool Car::isRescheduled() const { return rescheduled; }

void Car::setStatus(CarStatus s) { status = s; }

void Car::AssignBay(Bay* bay, int endTime)
{
	assignedBay = bay;
	currentEndTime = endTime;
	if (bay)
		currentUnit = bay->getType();
}

Bay* Car::ReleaseBay()
{
	Bay* bay = assignedBay;
	assignedBay = nullptr;
	currentEndTime = -1;
	return bay;
}

void Car::setCurrentUnit(UnitType u) { currentUnit = u; }

void Car::GenerateRandomServices()
{
	int n = 1 + rand() % 3;
	for (int i = 0; i < n; i++)
	{
		UnitType t = static_cast<UnitType>(1 + rand() % 3);
		AddService(t, 1 + rand() % 10);
	}
	seqType = (rand() % 2 == 0) ? SequenceType::SS : SequenceType::FS;
}

void Car::AddService(UnitType type, int duration)
{
	services.enqueue(Service(type, duration));
}

bool Car::HasService(UnitType type) const { return services.HasType(type); }

bool Car::HasServicesLeft() const { return !services.isEmpty(); }

int Car::getServicesCount() const { return services.getCount(); }

int Car::getNextServiceDuration() const
{
	return services.GetDurationOfType(currentUnit);
}

void Car::CompleteService(UnitType t)
{
	Service done;
	if (!services.RemoveByType(t, done))
		services.dequeue(done);
}

void Car::Finish(int timestep)
{
	status = CarStatus::FNSH;
	FT = timestep;
}

void Car::Cancel(int timestep)
{
	status = CarStatus::CNCL;
	CT = timestep;
	FT = timestep;
}

void Car::Print(ostream& out) const
{
	out << "[C" << ID << ", ";
	switch (status)
	{
	case CarStatus::IDLE:
	case CarStatus::ERLY:
	case CarStatus::LATE:
	case CarStatus::REDY:
		out << VT << ", " << PT;
		break;
	case CarStatus::DIAG:
	case CarStatus::SERV:

		if (assignedBay)
			out << assignedBay->getPrefix() << assignedBay->getID();
		break;
	case CarStatus::WAIT:
		out << getNextServiceDuration();
		break;
	case CarStatus::CNCL:
		out << CT;
		break;
	case CarStatus::FNSH:
		out << FT;
		break;
	}
	out << "]";
}

ostream& operator<<(ostream& out, const Car* car)
{
	if (car)
		car->Print(out);
	else
		out << "null";
	return out;
}
