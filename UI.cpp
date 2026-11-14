#include "UI.h"

#ifdef _WIN32
#include <conio.h>		// _getch(): "press any key" on Windows
#endif

UI::UI() : mode(SimMode::INTERACTIVE)
{
}

SimMode UI::ReadMode()
{
	int choice = 0;
	while (choice != 1 && choice != 2)
	{
		cout << "Select the program mode:" << endl;
		cout << "  1) Interactive mode" << endl;
		cout << "  2) Silent mode" << endl;
		cout << "Your choice: ";
		if (!(cin >> choice))
		{
			cin.clear();
			cin.ignore(10000, '\n');
			choice = 0;
		}
	}
	cin.ignore(10000, '\n');	// clear the rest of the line before "press any key"
	mode = (choice == 1) ? SimMode::INTERACTIVE : SimMode::SILENT;
	return mode;
}

SimMode UI::getMode() const
{
	return mode;
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

void UI::PrintSilentStart() const
{
	cout << "Simulation Starts in Silent mode ..." << endl;
}

void UI::PrintSilentEnd() const
{
	// Phase 1 has no output file. In phase 2 write the file first, then print:
	// "Simulation ends, Output file created"
	cout << "Simulation ends" << endl;
}

void UI::PrintSimulationEnd(int timestep, int finished, int cancelled) const
{
	cout << endl << "Simulation ended at timestep " << timestep
		<< ": " << finished << " finished, " << cancelled << " cancelled." << endl;
}
