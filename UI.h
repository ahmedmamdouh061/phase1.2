#pragma once
#include <iostream>
using namespace std;

enum class SimMode
{
	INTERACTIVE,
	SILENT
};

/*
  UI: all console input/output of the program goes through this class.
  The Center calls these functions to print each part of the output screen.
*/
class UI
{
private:
	SimMode mode;

public:
	UI();

	// Asks the user to choose interactive or silent mode
	SimMode ReadMode();
	SimMode getMode() const;

	void PrintTimestep(int timestep) const;

	// "==============  title  =============="
	void PrintSectionHeader(const char* title) const;

	// "-------------- title --------------"
	void PrintSubHeader(const char* title) const;

	// Prints one list line:  "<count> <label>: <items>"
	// maxItems = -1 prints all. If the list has more items, prints ".... and so on"
	// The list type L must have getCount() and print(separator, maxItems).
	template <typename L>
	void PrintList(const char* label, const L& list, const char* separator = " ", int maxItems = -1) const
	{
		cout << list.getCount() << " " << label << ": ";
		list.print(separator, maxItems);
		if (maxItems >= 0 && list.getCount() > maxItems)
			cout << " .... and so on";
		cout << endl;
	}

	void WaitForKey() const;

	void PrintSilentStart() const;
	void PrintSilentEnd() const;
	void PrintSimulationEnd(int timestep, int finished, int cancelled) const;
};
