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
