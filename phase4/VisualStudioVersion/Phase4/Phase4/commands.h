#include <iostream>
#include <cstdlib>
#include "dataStructures.h"
#include "assemble.h"

/*
NAME : RODOLFO GONGORA
SID  : 20214481
CLASS: CSCI 3334
TERM : FALL 2014
***PHASE 2 ***/


using namespace std;

//this file contains all the functions for every single command, even if they are not implemetned

void help()
{
	cout << "List of available commands: " << endl;
	cout << "help:            List of available commands				0 parameters" << endl;
	cout << "execute:         executes the program						0 parameters" << endl;
	cout << "debug:           Not implemented yet						0 parameters" << endl;
	cout << "dump start end:  Dumps the specified memory				2 parameters" << endl;
	cout << "load filename:   Loads the specified object file to memory	1 parameter" << endl;
	cout << "assemble filename: assembles the specified source file		2 parameter" << endl;
	cout << "directory:       lists all files in curr directory			0 parameters" << endl;
	cout << "exit:            exits the SIC simulator					0 parameters" << endl;
}

void directory(string OS)
{
	if (OS == "Windows")
		system("DIR");
	else
		system("ls");
}

void dump(optQ * optionQueue)
{
	string par1 = optionQueue->pop();
	string par2 = optionQueue->pop();


	int x;
	stringstream ss;
	ss << hex << par1;
	ss >> x;


	int y;
	ss.clear();
	ss << hex << par2;
	ss >> y;

	int start = x;
	int end = y;

	if (par1 != "" && par2 != "")
	{
		ADDRESS startAddr = start;
		ADDRESS endAddr = end;
		BYTE currentByte = 0;
		cout << "Dumping memory from: " << hex << startAddr;
		cout << " to " << hex << endAddr << endl;
		int columns = 1;
		for (ADDRESS i = startAddr; i < endAddr; i++)
		{
			GetMem(i, &currentByte, 0);
			cout << setfill('0') << setw(2) << (int)currentByte;

			if (columns > 10)
			{
				cout << endl;
				columns = 0;
			}
			else
				cout << " - ";

			columns++;
		}
		cout << endl;
	}
	else
		cout << "You need at least 2 parameters";
}
void clear(string OS)
{
	if (OS == "Windows")
		system("CLS");
	else
		system("clear");
}
void load(optQ * optionQueue)
{
	string par1 = optionQueue->pop();

	if (par1 != "")
		load(par1);
	else
		cout << "You need at least 1 parameter";
}

void execute()
{
	ADDRESS location = PROGSTART;
	SICRun(&location, 0);
}

void debug()
{
	cout << "Debug command executed" << endl;
}



void assemble(optQ * optionQueue)
{
	string par1 = optionQueue->pop();
	string par2 = optionQueue->pop();

	if (par1 != "" && par2 != "")
	{
		assemble(par1, par2);
	}
	else
		cout << "Please specify the source and output intermediate file"<<endl;
}







