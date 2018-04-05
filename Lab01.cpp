// Lab01.cpp
//Author: Charles Mikkelborg, Joel Davis
//Description: Reads an input file to poplulate a structure that can be outputted to the console.
//Last modified: 04/05/2018

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Person
{
	int last4SSN;
	string firstN, lastN, emailAddr;

};

void rdPerson(istream&, Person&);
void makeUC(string& name);
void wrtPerson(Person&);

int main()
{
	Person per1 = { 1234, "Charles", "Mikkelborg", "cmikk@gmail.com" };
	cout << per1.firstN << " " << per1.lastN << "'s SSN is: " << per1.last4SSN << endl;

	ifstream rdStream;
	string diskFile = "C:\\Users\\cmikk\\OneDrive\\School\\Q3 - Spring 2018\\Computer Science 2\\assets\\persons.txt";
	rdStream.open(diskFile);
	if (rdStream.fail())
	{
		cout << "File open failed." << endl;
		system("pause");
		exit(1);
	}

	do
	{
		if (rdStream >> per1.last4SSN)
		{
			rdPerson(rdStream, per1);
			makeUC(per1.lastN);
			makeUC(per1.firstN);

			wrtPerson(per1);
		}
		else
			break;

	} while (true);

	system("pause");
	return 0;
}

void rdPerson(istream& rdS, Person& per)
{
	rdS >> per.firstN;
	rdS.get();
	getline(rdS, per.lastN, '\t');
	rdS >> per.emailAddr;
}

void makeUC(string& name)
{
	string upper(name);
	for (int i = 0; i < name.length(); i++)
		upper[i] = toupper(name[i]);
	name = upper;
}

void wrtPerson(Person& per1)
{
	cout << per1.lastN << ", " << per1.firstN << ":" << per1.last4SSN << ":" << per1.emailAddr << endl;
}
