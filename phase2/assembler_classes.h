#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <list>

class tokenList;
class optQ;

optQ * getTokens(string line);

class symbolTable
{
	private:
		class symbol
		{
		public:
			string sym;
			int value;
			symbol(string x, int y)
			{
				sym = x;
				value = y;
			}
		};
		list<symbol> symTable;
	public:
		symbolTable()
		{

		}
		bool isInTable(string label)
		{
			for (auto x : symTable)
			{
				if (x.sym == label)
					return true;
			}
			return false;
		}
		void insert(string label, int locctr)
		{
			symbol x(label, locctr);
			symTable.push_back(x);
		}
		void print()
		{
			for (auto x : symTable)
			{
				cout << x.sym << '\t';
				cout << hex << x.value << endl;
			}
		}
		void printToFile(ofstream &  file)
		{
			for (auto x : symTable)
			{
				file << x.sym << '\t';
				file << hex << x.value <<endl;
			}
		}
};


class opTab
{
	private:
		class opcode
		{
			public:
				string mnemonic;
				string OPcode;

				opcode(string x, string y)
				{
					mnemonic = x;
					OPcode = y;
				}

		};
		list<opcode> opTable;

	public:
		opTab()
		{
			string line;
			ifstream opcodeTableFile;
			
			
			opcodeTableFile.open("opcodes.sic");
			while (getline(opcodeTableFile, line))
			{				
				optQ * tokens = getTokens(line);
				string x = tokens->pop();
				string y = tokens->pop();
				
				y = y.substr(0, y.size()-1);
				opcode z(x, y);
				opTable.push_back(z);

			}
			
		}	

		//REMEMBER: CHANGE SEARCH FUNCTION TO SOMETHING MORE OPTIMAL, LINEAR SEARCH SUCKS
		bool isInTable(string opcodeIn)
		{
			for (auto x : opTable)
			{
				if (opcodeIn == x.mnemonic)
					return true;
			}
			//if not found
			return false;
		}

		string getOpcode(string opcodeIn)
		{
			for (auto x : opTable)
			{
				if (opcodeIn == x.mnemonic)
					return x.OPcode;
			}
		}

		void print()
		{
			for (auto x : opTable)
			{
				cout << x.mnemonic << '\t';
				cout << hex << x.OPcode << endl;
			}
		}
};
