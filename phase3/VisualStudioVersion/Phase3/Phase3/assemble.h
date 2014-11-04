#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
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

bool handle2(string line, opTab * opcodeTable, symbolTable * symboltable, string & prevaddress, string & address, string & opcode, string & operand, string & textrecord, ofstream & objectfile, bool & written, string objectcode)
{

	string BYTE = "BYTE";
	string WORD = "WORD";
	string RESW = "RESW";
	string RESB = "RESB";
	objectcode = "";
	string operandAddress;

	if (opcodeTable->isInTableByValue(opcode))
	{
		objectcode += opcode;

		if (operand != "")
		{
			if (symboltable->isInTable(operand))
			{
				// store symbol value as operand address
				operandAddress = to_string(symboltable->getAddress(operand));
				operandAddress = int_to_hex(stoi(operandAddress)); //convert to hex

				
			}
			else
			{
				//store 0 as operand address
				operandAddress = "0";
				
				
				//set error flag (undefined symbol)
				
				if (opcode != "4C")
					return true;
			}
		}
		objectcode += operandAddress;
	}
	else if (opcode == BYTE || opcode == WORD)
	{
		if (opcode == WORD)
		{
			//convert consant to object code
			//string constant = int_to_hex(stoi(operand));
			string constant = ZeroPadNumber(stoi(operand), 6);
			constant = int_to_hex(constant);
			objectcode += constant;
		}
		else
		{
			//string constant = int_to_hex(byteSize(operand));
			if (operand[0] == 'C')
			{
				string constant = getConstantValue(operand);
				objectcode += constant;
			}
			else if (operand[0] == 'X')
			{
				string constant = operand.substr(2, operand.length() - 3);
				objectcode += constant;
			}
		}
	}


	int checkSize = textrecord.length() + objectcode.length();
	//if object code will nt fit into current text record then



	if (checkSize > 69 || opcode == RESW || opcode == RESB)
	{


		int length = (textrecord.length() - 9) / 2;
		//set the size of the text record
		string sizeInHex = int_to_hex(length);

		//textrecord.replace(7, 8, sizeInHex);
		if (textrecord != "")
		{
			textrecord[7] = sizeInHex[0];
			textrecord[8] = sizeInHex[1];
		}
		//write text record to object file

		if (textrecord.length() > 18)
		objectfile << textrecord << endl;
		textrecord = "";

		if (opcode == RESW || opcode == RESB)
			textrecord = "T00" + address + "__";
		else
		//initialize new text record
		textrecord = "T00" + prevaddress + "__";
	}
	textrecord += objectcode;
	return false;
}

	
void pass1(ifstream & file, string interFileName, int & programLength, opTab * opcodeTable, symbolTable * symboltable, string & progName)
{

	//file stream stuff
	ofstream interFile;
	interFile.open(interFileName);
	string line;
	string firstline;

	//counters and stuff
	int locctr;
	int starting;
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
		progName = label;
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

			string x = opcode.substr(0, opcode.length());
			interFile << hex << locctr << '\t' << opcode.substr(0, opcode.length()) << '\t' << operand;

			if (errorflag.error)
			{
				interFile << '\t' << "!!!!ERROR!!! " << errorflag.errorType;
			}
			interFile << endl;
		}
		getline(file, line);
		if (!isComment(line))
			interFile << line << endl;

	}
	programLength = locctr - starting;

	//TEST STUFF
	cout << "symbol table" << endl;
	symboltable->print();
	//print Smbol Table to file
	interFile << "Symbol Table: " << endl;
	symboltable->printToFile(interFile);
	cout << endl << "Program Length: ";
	cout << hex << programLength << endl;
	cout << endl << "Opcode Table: " << endl;
	opcodeTable->print();

	//close the file
	interFile.close();

}

void pass2(ifstream & interFile, int & programLength, opTab * opcodeTable, symbolTable * symboltable, string & progName)
{
	string sourceLine;
	string line;
	
	bool written = false;

	ofstream objectFile;
	objectFile.open("objfile.sic");

	ofstream listingFile;
	listingFile.open("listing.sic");

	//Read source line
	getline(interFile, sourceLine);
	
	//Read Actual code line
	getline(interFile, line);
	
	//getting all the tokens
	optQ * tokens = getTokens(line);
	string address = tokens->pop();
	string opcode = tokens->pop();
	string operand = tokens->pop();
	string prevaddress;
	string objectcode = "";

	string startingAddress;
	string progLength;
	string textrecord = "";

	if (opcode == "START")
	{
		//write listing line
		listingFile << "ADDRESS		OBJECTCODE		SOURCE \t\t\t\t ERRORS" << endl;
		listingFile << "---------------------------------------------------" << endl;
		//listingFile << address << "\t\t" << opcode + operand << "\t\t" << line <<  "\t 0";

		//read next inpu line
		//Read source line
		getline(interFile, sourceLine);

		//Read Actual code line
		getline(interFile, line);

		startingAddress = ZeroPadNumber(atoi(operand.c_str()), 6);
		progLength = ZeroPadNumber(programLength, 6);

	}




	//write header record to object file
	objectFile << "H" <<uppercase<< progName << "__" << startingAddress << hex << atoi(progLength.c_str()) <<   endl;

	//initialize first text record
	textrecord += "T" + ZeroPadNumber(atoi(address.c_str()), 6) + "__";




	tokens = getTokens(line);
	prevaddress = address;
	address = tokens->pop();
	opcode = tokens->pop();
	operand = tokens->pop();



	while (opcode != "END")
	{
		if (!isComment(line))
		{
			
			bool error = handle2(line, opcodeTable, symboltable, prevaddress, address, opcode, operand, textrecord, objectFile, written, objectcode);
			//write listing line
			listingFile << address << "\t\t" << objectcode << "\t\t" << line << "\t";
			//read next input line
			getline(interFile, sourceLine);
			getline(interFile, line);

			tokens = getTokens(line);
			prevaddress = address;
			address = tokens->pop();
			opcode = tokens->pop();
			operand = tokens->pop();



			if (error)
				listingFile << "ERROR, SYMBOL NOT FOUND";
			else
				listingFile << "0";
			listingFile << endl;
		}
	}

	//write last text record to object file
	//set the size of the text record
	int length = (textrecord.length() - 9) / 2;
	string sizeInHex = int_to_hex(length);
	int numberInInt = stoul(sizeInHex, NULL, 16);
	string number = ZeroPadNumber(numberInInt, 2);
	sizeInHex = int_to_hex(number);

	textrecord[7] = sizeInHex[0];
	textrecord[8] = sizeInHex[1];

	objectFile << textrecord<<endl;

	//write end record to object file
	objectFile << "E" << startingAddress;

	//write last listing ine
	listingFile << "END LISTING FILE" << endl;

	objectFile.close();

}

void assemble(string filename, string interFileName)
{
	//classes needed
	opTab * opcodeTable = new opTab();
	symbolTable * symboltable = new symbolTable();
	int programLength;
	string progName;

	ifstream thefile;
	thefile.open(filename);
	if (thefile.good())
	{
		pass1(thefile, interFileName, programLength, opcodeTable, symboltable, progName);
		ifstream interFile; 
		interFile.open(interFileName);
		pass2(interFile, programLength, opcodeTable, symboltable, progName);
	}
	else
		cout << "Invalid filename" << endl;
	thefile.close();

}


