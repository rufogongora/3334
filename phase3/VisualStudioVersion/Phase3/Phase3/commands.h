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
	cout << "help:            List of available commands              0 parameters" << endl;
	cout << "execute:         Not implemented yet                     0 parameters" << endl;
	cout << "debug:           Not implemented yet                     0 parameters" << endl;
	cout << "dump start end:  Not implemented yet                     2 parameters" << endl;
	cout << "load filename:   Not implemented yet                     1 parameter" << endl;
	cout << "assemble filename:Not implemented yet                    2 parameter" << endl;
	cout << "directory:       lists all files in curr directory       0 parameters" << endl;
	cout << "exit:            exits the SIC simulator                 0 parameters" << endl;
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

	if (par1 != "" && par2 != "")
		cout << "Dump command with parameter 1: " << par1 << ", parameter 2: " << par2 << endl;
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
		cout << "Load command with parameter 1: " << par1 << endl;
	else
		cout << "You need at least 1 parameter";
}

void execute()
{
	cout << "Execute command executed" << endl;
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







