//	This is an updated version of code originally
//  created by Frank M. Carrano and Timothy M. Henry.
//  Copyright (c) 2017 Pearson Education, Hoboken, New Jersey.

/** ADT stack: Array-based implementation.
 @file ArrayStack.h */

#ifndef ARRAY_STACK_
#define ARRAY_STACK_

#include "StackADT.h"

#include <iostream>
using namespace std;

//Unless spesificed by the stack user, the default size is 100
// [Project] the size is now a constructor parameter (the project has 1000+ cars,
//           so the fixed size of 100 is too small). The array is allocated once.
template<typename T>
class ArrayStack : public StackADT<T>
{
	enum { MAX_SIZE = 100 };
protected:  //in case you need to inherit for the project
	T* items;		// Array of stack items
	int top;                   // Index to top of stack
	int capacity;              // [Project] size of the array

public:

	ArrayStack(int size = MAX_SIZE)
	{
		capacity = (size > 0) ? size : MAX_SIZE;
		items = new T[capacity];
		top = -1;
	}  // end default constructor

	~ArrayStack()
	{
		delete[] items;
	}

	// The stack owns its array: copying would share it, so copying is disabled.
	ArrayStack(const ArrayStack<T>&) = delete;
	ArrayStack<T>& operator=(const ArrayStack<T>&) = delete;

	bool isEmpty() const
	{
		return top == -1;	
	}  // end isEmpty

	bool push(const T& newEntry)
	{
		if( top == capacity-1 ) return false;	//Stack is FULL

		top++;
		items[top] = newEntry;   
		return true;
	}  // end push

	bool pop(T& TopEntry)
	{
		if (isEmpty()) return false;
		
		TopEntry = items[top];		 
		top--;
		return true;
	}  // end pop
	
	bool peek(T& TopEntry) const
	{
		if (isEmpty()) return false;
		
		TopEntry = items[top];		 
		return true;
	}  // end peek

	// [Project] added for the project: count and print

	// O(1)
	int getCount() const
	{
		return top + 1;
	}

	// O(n): prints from top to bottom (most recent first) separated by "separator".
	// maxItems = -1 prints all. T must have operator<< (overloaded for Car*).
	void print(const char* separator = " ", int maxItems = -1) const
	{
		int printed = 0;
		for (int i = top; i >= 0 && (maxItems < 0 || printed < maxItems); i--)
		{
			if (printed > 0) cout << separator;
			cout << items[i];
			printed++;
		}
	}

}; // end ArrayStack

#endif
