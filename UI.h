#pragma once
#include <iostream>
using namespace std;

class UI
{
public:
	void PrintStartMessage() const;
	void PrintTimestep(int timestep) const;

	void PrintSectionHeader(const char* title) const;

	void PrintSubHeader(const char* title) const;

	template <typename L>
	void PrintList(const char* label, const L& list, const char* separator = " ", int maxItems = -1) const
	{
		cout << list.getCount() << " " << label << ": ";
		list.print(separator, maxItems);
		if (maxItems >= 0 && list.getCount() > maxItems)
			cout << " .... and so on";
		cout << endl;
	}

	bool WaitForKey() const;

	void PrintSkippingToEnd() const;

	void PrintSimulationEnd(int timestep, int finished, int cancelled) const;
};
