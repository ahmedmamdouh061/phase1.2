// Windows headers first (before UI.h's "using namespace std") to avoid name clashes
#ifdef _WIN32
#include <conio.h>		// _kbhit(), _getch(): read a key press on Windows
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>	// GetAsyncKeyState(): SHIFT alone does not produce a key press, so we read its state
#endif

#include "UI.h"

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
	cout << "PRESS ANY KEY TO MOVE TO NEXT STEP !   (or press SHIFT to go to the end)" << endl;
#ifdef _WIN32
	while (true)
	{
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)		// SHIFT is pressed
		{
			while (GetAsyncKeyState(VK_SHIFT) & 0x8000)	// wait until it is released
				Sleep(10);
			while (_kbhit())							// ignore any other key pressed with it
				_getch();
			return false;
		}
		if (_kbhit())									// any other key
		{
			_getch();
			while (_kbhit())							// arrow/function keys send 2 codes
				_getch();
			return true;
		}
		Sleep(10);
	}
#else
	// Other systems cannot read SHIFT alone: press Enter for the next step,
	// or type s then Enter to go to the end.
	int c = cin.get();
	bool skip = (c == 's' || c == 'S');
	while (c != '\n' && c != EOF)
		c = cin.get();
	if (c == EOF)
		return false;
	return !skip;
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
