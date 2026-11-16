#include "UI.h"

#ifdef _WIN32
#include <conio.h>		// _getch(): "press any key" on Windows
#endif

void UI::PrintStartMessage() const
{
	cout << "Car Service Center - Phase 1 Random Simulation (interactive mode)" << endl;
}

void UI::PrintTimestep(int timestep) const
{
	cout << endl << "Current Timestep:" << timestep << endl;
}

void UI::PrintSectionHeader(const char* title) const
{
	cout << "==============  " << title << "  ==============" << endl;
}

void UI::PrintSubHeader(const char* title) const
{
	cout << "-------------- " << title << " --------------" << endl;
}

void UI::WaitForKey() const
{
	cout << "PRESS ANY KEY TO MOVE TO NEXT STEP !" << endl;
#ifdef _WIN32
	_getch();
#else
	cin.get();
#endif
}

void UI::PrintSimulationEnd(int timestep, int finished, int cancelled) const
{
	cout << endl << "Simulation ended at timestep " << timestep
		<< ": " << finished << " finished, " << cancelled << " cancelled." << endl;
}
