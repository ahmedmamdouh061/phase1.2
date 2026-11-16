#include "UI.h"

#ifdef _WIN32
#include <conio.h>
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
	cout << "PRESS N TO MOVE TO NEXT STEP, OR E TO GO TO THE END !" << endl;
	while (true)
	{
#ifdef _WIN32
		int key = _getch();
#else
		int key = cin.get();
		int c = key;
		while (c != '\n' && c != EOF)
			c = cin.get();
		if (c == EOF)
			return false;
#endif
		if (key == 'n' || key == 'N')
			return true;
		if (key == 'e' || key == 'E')
			return false;
	}
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
