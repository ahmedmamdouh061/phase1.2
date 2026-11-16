#include "UI.h"

#ifdef _WIN32
#include <conio.h>		// _getch(): reads one key press on Windows
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

bool UI::WaitForKey() const
{
	cout << "PRESS ANY KEY TO MOVE TO NEXT STEP !   (or press E to go to the end)" << endl;
#ifdef _WIN32
	int key = _getch();
	return !(key == 'e' || key == 'E');
#else
	// Other systems: press Enter for the next step, or type E then Enter to go to the end
	int key = cin.get();
	int c = key;
	while (c != '\n' && c != EOF)		// ignore the rest of the line
		c = cin.get();
	if (c == EOF)
		return false;
	return !(key == 'e' || key == 'E');
#endif
}

void UI::PrintSkippingToEnd() const
{
	cout << endl << "Going to the end of the simulation ..." << endl;
}

void UI::PrintSimulationEnd(int timestep, int finished, int cancelled) const
{
	cout << endl << "Simulation ended at timestep " << timestep
		<< ": " << finished << " finished, " << cancelled << " cancelled." << endl;
}
