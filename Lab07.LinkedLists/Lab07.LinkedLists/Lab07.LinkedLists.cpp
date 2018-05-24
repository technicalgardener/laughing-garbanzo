// Lab07.LinkedLists.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
/*



 
else {
establish boolean variable "inserted" and initialize to false
set up my "movingPtr" to scan through the list by assigning listHead to it
while ( ! inserted )
is movingPtr->nextNode == NULL ? // Did we reach end of list?
if so, put the newNode AFTER the one movingPtr is pointing to and make inserted true
else is the value in the node AFTER the one pointed to by movingPtr >= the newNode?
if so, insert the newNode after movingPtr node and before the one beyond,
and make inserted true
else advance movingPtr to point to the next node in the list, and (inner) loop
} end of inner loop
} end of "else"
make newNode point to a new empty node and loop up to try to read a value into it
}
here I'm beyond both the inner and outer loop
I've got one extra node that didn't get read into. delete it..

Now print out the list.
*/
const string diskFileRd = "C:\\Users\\cmikk\\OneDrive\\School\\Q3 - Spring 2018\\Computer Science 2\\laughing-garbanzo\\Lab07.LinkedLists\\Lab07.LinkedLists\\Resource\\unsorted127.txt";

struct Numb {
	int numbValue;
	struct Numb * nextNumb;
};

int main()
{
	ifstream rdDB;
	int numbCount = 0;
	int listNumb = 1;
	bool isSorted;
	struct Numb * numbListHead = NULL;
	struct Numb * movingPtr;
	struct Numb * createdNode;

	// Open disk file
	rdDB.open(diskFileRd);
	if (rdDB.fail())
	{
		cout << "-Failed to Open File-" << endl;

		system("pause");
		exit(1);
	}

	// Create first node
	createdNode = new Numb;

	// Read 1st value from file into that node(check to see if file - read FAILED)
	if (!(rdDB >> createdNode->numbValue)) {
		cout << "No data in file.\n";

		system("pause");
		exit(1);
	} else { // Make that 1st node the listHead
		numbListHead = createdNode;  // Our new node BECOMES head of list!
		numbListHead->nextNumb = NULL;  // Show that no other nodes are in list...
		createdNode = new Numb; // Make a second node, named newNode (which begins life empty)
	} 

	// While (read from file into newNode successfully ? )
	// If NOT a successful read, not executing this loop!Have run out of file contents
	while (rdDB >> createdNode->numbValue)
	{

		if (createdNode->numbValue < numbListHead->numbValue) { // Is the newNode value < the value in headList?
			createdNode->nextNumb = numbListHead; // Must do this while waitListHead references existing list!
			numbListHead = createdNode;  // THEN can revise waitListHead to reference the new node
		}
		else 
		{
			movingPtr = numbListHead;  // We'll traverse, to FIND end of list
			while (!isSorted) {
				
				if (movingPtr->nextNumb == NULL) {
					movingPtr->nextNumb = createdNode;  // New node tacked on end of list
					createdNode->nextNumb = NULL;
					isSorted = true;
				}
				else if ((movingPtr->nextNumb)->numbValue >= createdNode->numbValue)
				{
					createdNode->nextNumb = movingPtr->nextNumb;
					movingPtr->nextNumb = createdNode;
					isSorted = true;
				}
				else
					movingPtr = movingPtr->nextNumb;
				// At this point, movingPtr is pointing at the last node in the list.
			}
		}
		createdNode = new Numb;
		cout << "+";
		numbCount++;
	}
	cout << endl << numbCount + 1 << endl;
	
	
	movingPtr = numbListHead;

	while  (movingPtr != NULL) {
		cout << listNumb << ":\t" << movingPtr->numbValue << endl;
		movingPtr = movingPtr->nextNumb;
		listNumb++;
	}
	

	system("pause");
    return 0;
}

