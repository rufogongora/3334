#include <iostream>
#include "stdlib.h"
#include <cstdlib>
#include "commands.h"

/*
	NAME : RODOLFO GONGORA
	SID  : 20214481
	CLASS: CSCI 3334
	TERM : FALL 2014
	***PHASE 1***
*/


using namespace std;


//this file contains the most complex functions like tokenizing etc. "the backend stuff"
string getOsName()
{
    #ifdef _WIN32
    return "Windows";
    #elif _WIN64
    return "Windows";
    #elif __unix || __unix__
    return "Unix";
    #elif __APPLE__ || __MACH__
    return "Mac_OSX";
    #elif __linux__
    reutnr "Linux";
    #elif __FreeBSD__
    return "FreeBSD";
    #else
    return "Other";
    #endif
}

tokenList * tokenize(string inputStream)
{
	
	tokenList * aTokenList = new tokenList();
	int insideCounter = 0;
	char * theString = new char();
	string str;
	for(int i = 0; i < inputStream.length(); i++)
	{

		if (inputStream[i] != ' ')
		{
			theString[insideCounter] = inputStream[i];
			insideCounter++;
		}
		//found a space
		else
		{
			str = theString;
			
			if (str.length() != 0)
			{
				aTokenList->addToken(str);	
			}
			insideCounter = 0;
			theString = new char();
		}
	}
	str = theString;
	if (str.length() != 0)
	aTokenList->addToken(str);

	//this line is to print-test the tokens just in case
	//aTokenList->print();
	return aTokenList;
}

void selectCommand(string command, optQ * optionQueue)
{

	//after getting the string select the appropiate command and call the function
	if (command == "help")
	{
		help();
	}
	else if (command == "directory")
	{	
		directory(getOsName());
	}
	else if (command == "quit" || command == "exit")
	{
		exit(0);
	}
	else if (command == "dump")
	{
		dump(optionQueue);
	}
	else if (command == "clear")
	{
		clear(getOsName());
	}
	else if (command == "load")
	{	
		load(optionQueue);
	}
	else if (command == "execute")
	{
		execute();	
	}
	else if (command == "debug")
	{
		debug();	
	}
	else if (command == "assemble")
	{
		assemble(optionQueue);	
	}
	
	//if everything fails, hehe
	else
	{
		cout<<"invalid command --use the command ''help'' for a list of available commands"<<endl;
	}
	
}


void func(string option)
{
	//initialize string queue to select proper option
	optQ * optionQueue;

	//tokenize all the commands	
	tokenList * aTokenList = tokenize(option);
	
	//create a queue with all the tokens
	optionQueue = aTokenList->createQueue();
	
	//select the first command
	if (!optionQueue->empty())
	{
		string frstCommand = optionQueue->pop();
		selectCommand(frstCommand, optionQueue);
	}
	else
		cout<<"please write a command"<<endl;
	

	
}


void terminal()
{
	//to read user input
	string option;
	cout<<"username@"<<getOsName()<<": ";
	getline(cin, option);

	//select function according to user input
	func(option);	
	
}



