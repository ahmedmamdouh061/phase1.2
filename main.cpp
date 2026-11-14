// Car Service Center - Phase 1.2
// CMPS103 Data Structures and Algorithms - Fall 2026 - Team T14

#include <cstdlib>
#include <ctime>
#include "Center.h"

int main()
{
	srand(static_cast<unsigned>(time(nullptr)));

	Center center;
	center.RunRandomSimulation();

	return 0;
}
