// Lab04.cpp
// Author: Charles Mikkelborg, Brett Turner
// Description: Reads an input file to poplulate a class object in an array that can be output to the console and a file.
//		Includes functionality to view the database, enter a new vehicle, update how many weeks a car has been the lot,
//		and search by VIN number.
// Last modified: 05/01/2018
/*
Part 1.  Modify your existing code to deal with the option-package lines, making use of a private dynamic string[ ] array variable.  
Make necessary changes to all constructors.  Eliminate your existing rdDBEntry() and wrtDBEntry() functions, 
and instead provide functions that overload the << and  >> operators.  
The overloaded operator should be capable of dealing with an entire Vehicle object at once -- i.e., ">>"  reads in all normal fields, 
notices whether optional lines are there, builds dynamic storage as needed, populates the storage.  
The operators should work with either the console stream or a file stream.  
The operators should then be used elsewhere in your code (like the viewDB() menu-handler, the file I/O operations, etc.)   
You do NOT need to change your (N)ew vehicle handler to support the action of specifying additional options for a new vehicle.  
Finally, write a "destructor" function that releases an object's dynamic storage back to the freestore.

Part 2.  Add a "copy constructor" and a function to overload the assignment operator.  To test that these are working, add a menu option (R)earrange.  
Prompt the user to enter two numbers, which represent the index values in the Vehicle[array] of two of the vehicles.  
Then, swap the two specified cars, which will invoke both the copy constructor and the assignment operator.  The next time that (V)iew is done, the order will be different...
*/

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

/******* Our only globals are CONST variables  **************/
const string diskFileRd = "xvehicles_db.txt";
const int VA_SIZE = 10;


/****************  Member functions  *****************/
class Vehicle
{
public:
	/* Mutator and Accessor declarations */
	void wrtVin(string wVin);
	void wrtPrice(double wPrice);
	void vinBreaker(string& wVin);
	string wholeVin();
	string getSOut();

	/* Constructor declarations */
	Vehicle();
	Vehicle(string vin);
	Vehicle(string vin, const string);

	/* Destructor declarations */
	~Vehicle();

	friend ostream& operator << (ostream& outS, const Vehicle& v);
	friend istream& operator >> (istream& inS, Vehicle& v);

	/* Helper functions */
	friend void operator ++(Vehicle& v);
	friend bool operator ==(const Vehicle& v1, const Vehicle& v2);

	/* Copy and Rearrange*/
	Vehicle(const Vehicle& original); //Copy Constructor
	void operator =(const Vehicle& rightSide); //Assignment Operator

private:
	void makeUC(string& s);	// A private make uppercase function
	string v1 = "";
	string vin;
	string v3 = "";
	double price;
	int weeksOnLot;
	int listSize = 0;
	string * listPtr = NULL;
	int storageSize;  // Size of the container to create
	string * stg;		// Dynamically created to be exact fit
	int next2fill;  // Which slot in the container is the next to fill
	int next2get;   // Which slot is the next to empty
};

/****************  Regular functions  ***************/
void viewDB(Vehicle V[], int size);
int add2DB(Vehicle V[], int size);
void updateWeeksOnLot(Vehicle V[], int size);
void search4Vehicle(Vehicle V[], int size);
void rearrange(Vehicle V[], int size);

int main()
{
	ifstream rdDB;
	ofstream wrtDB;
	Vehicle VA[VA_SIZE];
	int vaSize = 0;

	// Opens vehicle.txt and populates vehicle class objects.
	// class objects are then put into an array.
	rdDB.open(diskFileRd);
	if (rdDB.fail())
	{
		cout << "Open File Error";
		exit(1);
	}

	while (rdDB >> VA[vaSize])
	{
		if (++vaSize >= VA_SIZE)
		{
			cout << "Array full! Skipping remaining DB entries.";
			break;
		}
	}
	rdDB.close();

	/* Menu */
	char menuChoice;
	do
	{
		cout << "\n\nMenu: (V)iew DB, (N)ew entry, (U)pdate, (S)earch, (R)earrange, e(X)it program: ";
		cin >> menuChoice;

		switch (menuChoice)
		{
		case 'X': case 'x':
			// writes changes made to the array to file on exit.
			cout << "Goodbye!\n";
			return(0);
		case 'V': case 'v':
			viewDB(VA, vaSize);
			break;
		case 'N': case 'n':
			// add new vehicle
			if (vaSize < VA_SIZE)
				vaSize = add2DB(VA, vaSize);
			else
				cout << "Can't add a vehicle! Array already full.";
			break;
		case 'U': case 'u':
			updateWeeksOnLot(VA, vaSize);
			break;
		case 'S': case 's':
			search4Vehicle(VA, vaSize);
			break;
		case 'R': case 'r':
			rearrange(VA, vaSize);
		default:
			cout << "Illegal choice.  Try again. \n";
		}
	} while (true);

	return 0;
}

/************* "Friend" functions and operator overload ***************/
void operator ++(Vehicle& v) // Operator overload function to update "weeksOnLot" by one.
{
	v.weeksOnLot = v.weeksOnLot++;
}

bool operator==(const Vehicle & v1, const Vehicle & v2) // Operator overload function to compare two vins.
{
	return (v1.vin == v2.vin);
}

void Vehicle::operator =(const Vehicle& rightSide) //Assignment Operator
{
	if (stg != NULL)  // If dynamic storage in use on left side, delete it
		delete[] stg;
	storageSize = rightSide.storageSize;
	if (storageSize) // If dynamic storage in use on right side...
	{
		stg = new string[storageSize]; //    ...make independent copy.
		for (int ix = 0; ix < storageSize; ix++)
			stg[ix] = rightSide.stg[ix];
	}
	else stg = NULL;
	next2fill = rightSide.next2fill;
	next2get = rightSide.next2get;
}

/**********************************************************************/
// Here are paired MUTATOR and ACCESSOR functions for private Vehicle variables

void Vehicle::wrtVin(string wVin)		// Mutator to change private variable "vin"
{
	makeUC(wVin);		// Assure UC vin through "helper" function
	vinBreaker(wVin);
}

void Vehicle::wrtPrice(double wPrice)		// Mutator to change private variable "price"
{
	price = wPrice;
}

void Vehicle::vinBreaker(string& wVin) // Mutator to break down a given vin number into 3 parts
{
	cout << endl << "--------> " << wVin << endl;
	v1 = wVin.substr(0, 1);
	v3 = wVin.substr(4, 5);
	vin = wVin.substr(1, 3);
	
}

string Vehicle::wholeVin() // Accessor function that retreives all parts of a vin and returns them as a single string
{
	return (v1 + vin + v3);
}

string Vehicle::getSOut()
{
	string s = "";  // Return value will be built in "s"
	if (stg != NULL) {  // Guard against calling getSOut() when it's empty!
		while (next2get < next2fill) {  // For all chars in storage...
			s += *(stg + next2get);		// ...concatenate each in turn onto "s"
			next2get++;
		}
		delete[] stg;  // Make container empty ...
		stg = NULL;
	}
	next2get = next2fill = 0; // Show container as empty...
	return(s);
}

/**********************************************************************/
// Here are CONSTRUCTOR definitions
Vehicle::Vehicle() : vin("?"), price(0.00), weeksOnLot(0) // Default constructor
{
	cout << "+";
	listSize = 0;
	listPtr = NULL;
	next2fill = next2get = 0;
	stg = NULL;
}

Vehicle::Vehicle(string s) // Constructor with vin field
{
	makeUC(s);	// UC vin through "helper" function
	vin = s;
	price = 0.00;
	weeksOnLot = 0;
}

Vehicle::Vehicle(string s, const string l) // Constructor with vin field
{
makeUC(s);	// UC vin through "helper" function
vin = s;
price = 0.00;
weeksOnLot = 0;
storageSize = l.length();
next2fill = next2get = 0;
if (storageSize) {  // If it wasn't an empty string that we received...
	stg = new string[storageSize];		// Create the dynamic array
	while (next2fill < storageSize) { // For each char in string
		*(stg + next2fill) = l[next2fill];  // char into storage
		next2fill++;				// Advance to next slot AND next input char
	}
}
}

Vehicle::Vehicle(const Vehicle& original) //Copy Constructor
{
	storageSize = original.storageSize;
	if (storageSize)  // Did original HAVE any dynamic storage in use?
	{
		stg = new string[storageSize];  // If so, create a duplicate
		for (int ix = 0; ix < storageSize; ix++)
			stg[ix] = original.stg[ix];
	}
	else stg = NULL;
	next2fill = original.next2fill;
	next2get = original.next2get;
}

/**********************************************************************/
// Here are DESTRUCTOR definitions
Vehicle::~Vehicle()
{
	if (listPtr != NULL)
		delete[] listPtr;
	listPtr = NULL;
	listSize = 0;
	if (stg != NULL)	// If there WAS dynamic storage allocated...
		delete[] stg;	//   release it to freestore
	next2fill = next2get = 0;
	stg = NULL;
	cout << "-";
}

/********************* Private "helper" functions ***********************/
// Function to convert all letters in string to Upper Case.
void Vehicle::makeUC(string& s) // class function to make characters upper case
{
	char * cL = &s[0]; // Pointer to first char
	char * cR = &s[s.length()]; // Pointer BEYOND end of string

	for (; cL < cR; cL++) // Examine each char, in turn
		if (isalpha(*cL))
			*cL = toupper(*cL);
}

/**************  Regular functions, one to handle each menu choice ************/
// Displays a list of vehicles that are in the database
void viewDB(Vehicle V[], int size) // The (V)iew menu choice
{
	cout << "\nVin" << '\t' << "Price" << '\t' << "Weeks On Lot" << '\t' << "Options" << endl;
	for (int i = 0; i < size; i++)
		cout << "Vehicle " << i << endl << V[i] << endl;
}

// Lets the user input a vehicle to be added to the database
int add2DB(Vehicle dbVehicle[], int aSize) // The (N)ew menu choice.  Fetch data, build Vehicle, APPEND to DB file
{
	string bVin;
	double price;
	char y_n;

	cout << "\nEnter Vehicle VIN number: " << endl;
	cin >> bVin;
	cout << "Known vehicle price (Y/N)? ";
	cin >> y_n;

	if ((y_n != 'y') && (y_n != 'Y')) { // if 'y' or 'Y' are entered then ask for price
		dbVehicle[aSize].wrtVin(bVin);
		return(aSize + 1);
	}
	else {
		cout << "\nEnter Vehicle price: " << endl;
		cin >> price;
		dbVehicle[aSize].wrtVin(bVin);
		dbVehicle[aSize].wrtPrice(price);
		return(aSize + 1);
	}
}

// Increments the number of weeks a vehicle has been on the lot by 1, writes the updated database to a new file
void updateWeeksOnLot(Vehicle dbVehicle[], int aSize) // Mutator that updates weeksOnLot for all objects in an array
{
	for (int ix = 0; ix < aSize; ix++)
		++dbVehicle[ix];
}

// User can search the vehicle database by entering three uppercase letters that are in the vehicles vin
// If a vin with those letters is found information about that vehicle is displayed
void search4Vehicle(Vehicle V[], int size)	// Choice to (S)earch for vehicle in DB.
{
	string bVin;
	bool notFound = true;  // Assume it's NOT there...

	cout << "Enter the first 3 capital letters of the vehicle's vin: ";
	cin >> bVin;

	Vehicle search4(bVin);		// Local object to hold disk DB entries			


	for (int i = 0; i < size; i++)
	{
		if (V[i] == search4) // Loop until all entries read
		{
			cout << "Found! >> ";
			cout << V[i];
			notFound = false;  // Now will skip failure msg. below
		}
	}
	if (notFound)  // Failure message
		cout << "Vehicle \"" << bVin << "\" not in DB.";
}

void rearrange(Vehicle V[], int size)
{
	int veh1, veh2;
	cout << "\nRearrange two vehicles in the database" << "\nEnter the number for the first vehicle" << endl;
	cin >> veh1;
	cout << "\nEnter the number for the second vehicle" << endl;
	cin >> veh2;
		{
		
		}
}

ostream& operator << (ostream& outS, const Vehicle& v)
{
	outS << v.v1 + "-" + v.vin + "-" + v.v3 << '\t' << v.price << '\t' << v.weeksOnLot << '\t' << v.listSize << endl;
	if (v.listSize > 0)
	{
		string * sPtr = v.listPtr;
		for (int ix = 0; ix < v.listSize; ix++)
		{
			outS << *sPtr << endl;
			sPtr++;
		}
	
	}
	return(outS);
}

istream& operator >> (istream& inS, Vehicle& v)
{
	string bVin;
	v.vin[0] = '?';
	inS >> v.vin;
	if (v.vin[0] != '?')
	{
		v.vinBreaker(v.vin);
		inS >> v.price >> v.weeksOnLot >> v.listSize;
		inS.get();
		if (v.listSize > 0)
		{
			v.listPtr = new string[v.listSize];
			string * sPtr = v.listPtr;
			for (int ix = 0; ix < v.listSize; ix++)
			{
				getline(inS, *sPtr);
				sPtr++;
			}
		}
	}
	return(inS);
}