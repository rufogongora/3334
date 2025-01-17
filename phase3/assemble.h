#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include "assembler_classes.h"

class tokenList;
tokenList * tokenize(string inputStream);
class optQ;

struct errorFlag
{
	string errorType;
	bool error;
};
void writeToIntermediateFile(ofstream& interfile, string line, int locctr, string mnemonic, string operand, bool error)
{
	interfile << line << endl;
	interfile << hex << locctr << '\t' << mnemonic << '\t' << operand;
	if (error)
	{
		interfile << "Error" << endl;
	}
}

bool isComment(string line)
{
	return (line[0] == '.' || line.empty());
}

int byteSize(string operand)
{
	int i;
	if (operand[0] == 'C')
	{

		//IT'S A STRING
		if (operand.length() < 33)
		{

			i = operand.length() - 1;
			return (i - 2) ;
		}
	}
	else
	if (operand[0] == 'X')
	{
		//it's a hex
		if (operand.length() % 2 == 0)
			return 0;
		else if (operand.length() < 35)
		{
			i = operand.length() - 1;
			return  ( (i - 2) / 2 );
		}

	}
	else
		//error
		return 0;
}

errorFlag handle(string line, opTab * opcodeTable, symbolTable * symboltable, int & locctr, string & label, string & opcode, string & operand)
{

	errorFlag anError;
	//tokenizing the line
	optQ * tokens = getTokens(line);

	//there's a label
	if (line[0] != ' ' && line[0] != '\t')
	{
		label = tokens->pop();
		if (symboltable->isInTable(label))
		{
			//set error flag
			anError.errorType = "Duplicate Label";
			anError.error = true;
			return anError;
		}
		else
			//insert to symbol table
			symboltable->insert(label, locctr);
	}
	opcode = tokens->pop();
	operand = tokens->pop();
	if (opcodeTable->isInTable(opcode))
	{
		//translating mnemonic into opcode
		opcode = opcodeTable->getOpcode(opcode);
		locctr += 3;
	}
	else if (opcode == "WORD")
		locctr += 3;
	else if (opcode == "RESB")
		locctr += stoi(operand);
	else if (opcode == "RESW")
		locctr += 3 * stoi(operand);
	else if (opcode == "BYTE")
	{
		int size = byteSize(operand);
		if (size != 0)
		{
			locctr += size;
		}
		else
		{
			anError.errorType = "Size is 0";
			anError.error = true;
			return anError;
		}
	}
	else if (opcode == "END")
	{
		anError.error = false;
		return anError;
	}
	else
	{
		anError.errorType = "Invalid Opcode";
		anError.error = true;
		return anError;
	}
	anError.error = false;
	return anError;
}

void pass1(ifstream & file, string interFileName)
{
	//classes needed
	opTab * opcodeTable = new opTab();
	symbolTable * symboltable = new symbolTable();

	//file stream stuff
	ofstream interFile;
	interFile.open(interFileName);
	string line;
	string firstline;

	//counters and stuff
	int locctr;
	int starting;
	int progLength;
	errorFlag errorflag;

	//get first line and write it to the intermediate file
	getline(file, line);

	interFile << line << endl;
	optQ * tokens = getTokens(line);
	string label = tokens->pop();
	string opcode = tokens->pop();
	string operand = tokens->pop();

	//if OPCODE = 'START' then
	if (opcode == "START")
	{
		//convert the operand to hex
		starting = strtol(operand.c_str(), NULL, 16);
		locctr = starting;

		interFile << hex << locctr << '\t' << opcode << '\t' << operand << endl;
		//read next line and write it to the intermediate file
		getline(file, line);

		interFile << line << endl;
	}
	else
		locctr = 0;
	while (opcode != "END")
	{
		if (!isComment(line))
		{
			//handle line
			errorflag = handle(line, opcodeTable, symboltable, locctr, label, opcode, operand);

			interFile << hex << locctr << '\t' << opcode << '\t' << operand;

			if (errorflag.error)
			{
				interFile << '\t' << "!!!!ERROR!!! " << errorflag.errorType;
			}
			interFile << endl;
		}
		getline(file, line);
		interFile << line << endl;

	}
	progLength = locctr - starting;

	//TEST STUFF
	cout << "symbol table" << endl;
	symboltable->print();
	//print Smbol Table to file
	interFile << "Symbol Table: " << endl;
	symboltable->printToFile(interFile);
	cout << endl << "Program Length: ";
	cout << hex << progLength << endl;
	cout << endl << "Opcode Table: " << endl;
	opcodeTable->print();

	//close the file
	interFile.close();

}

void pass2(ifstream & interFile)
{
	string sourceLine;
	string line;
	
	//Read source line
	getline(interFile, sourceLine);
	
	//Read Actual code line
	getline(interFile, line);
	
	//getting all the tokens
	optQ * tokens = getTokens(line);
	string label = tokens->pop();
	string opcode = tokens->pop();
	string operand = tokens->pop();

	if (opcode == "START")
	{
		
	}
}

void assemble(string filename, string interFileName)
{
	ifstream thefile;
	thefile.open(filename);
	if (thefile.good())
	{
		pass1(thefile, interFileName);
		ifstream interFile; 
		interFile.open(interFileName);
		pass2(interFile);
	}
	else
		cout << "Invalid filename" << endl;
	thefile.close();

}
