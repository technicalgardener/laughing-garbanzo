// lab07.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const string diskFileRd = "C:\\Users\\cmikk\\source\\repos\\lab07\\lab07\\Resource\\unsorted127.txt";

struct Numb {
	int numbValue;
	struct Numb * nextValue; // So-called "self-referential" structure
};

int main()
{
	ifstream rdDB;
	int numbCount = 0;

	struct Numb * waitListHead = NULL;
	struct Numb * movingPtr;
	struct Numb * createdNode = NULL;

	rdDB.open(diskFileRd); // Opens the global file constant vehicle.txt
	if (rdDB.fail()) // Checks for a load error, if cannot load file, then stop program with error.
	{
		cout << "Open File Error" << endl;

		system("pause");
		exit(1);
	}

	if (!(rdDB >> createdNode->numbValue))
	{
		cout << "No data in file.\n";
		system("pause");
		exit(1);
	}

	do
	{
		numbCount++;
		createdNode = new Numb;  // Dynamically create new node
		rdDB >> createdNode->numbValue;

		if (waitListHead == NULL) { // No list exists
			waitListHead = createdNode;  // Our new node BECOMES head of list!
			waitListHead->nextValue = NULL;  // Show that no other nodes are in list...
		}

		// NOT an empty list.  Put new node at END of list.
		else {
			movingPtr = waitListHead;  // We'll traverse, to FIND end of list
			while (movingPtr->nextValue != NULL)
				movingPtr = movingPtr->nextValue;

			// At this point, movingPtr is pointing at the last node in the list.
			movingPtr->nextValue = createdNode;  // New node tacked on end of list
			createdNode->nextValue = NULL;		// Show that no nodes are beyond...
		}
		//cout << createdNode->numbValue << endl;
	} while (rdDB >> createdNode->numbValue);

	movingPtr = waitListHead;  // Start at head of list
	
	while (movingPtr != NULL) // Any MORE list to go?
	{
		cout << movingPtr->numbValue << endl; // Show party name in this node
		movingPtr = movingPtr->nextValue; // Traverse to next node in the list
										  //movingPtr = (*movingPtr).nextParty; // Clunkier syntax that also works...
	}
	
	/*
	while (movingPtr != NULL)
		if ((movingPtr->nextValue)->numbValue < movingPtr->numbValue)
	*/

	system("pause");
    return 0;
}

