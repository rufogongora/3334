#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <list>
#include <sstream>
#include <iomanip>

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

		string at(int n)
		{
			int i = 0;
			for (auto x : symTable)
			{
				if (i == n)
					return x.sym ;
			}
			return "";
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

		int getAddress(string name)
		{
			for (auto x : symTable)
			{
				if (x.sym == name)
					return x.value;
			}
			return 0;
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
				
				y = y.substr(0, y.size());
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

		bool isInTableByValue(string opcodeIn)
		{
			for (auto x : opTable)
			{
				if (opcodeIn == x.OPcode)
				{
					return true;
				}
			}
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
string ZeroPadNumber(int num, int n)
{
	stringstream ss;

	// the number is converted to string with the help of stringstream
	ss << num;
	string ret;
	ss >> ret;

	// Append zero chars
	int str_length = ret.length();
	for (int i = 0; i < n - str_length; i++)
		ret = "0" + ret;
	return ret;
}

template< typename T >
string int_to_hex(T i)
{
	stringstream stream;
	stream << hex << i;
	string s = stream.str();
	return s;
}



string stringToHex(string input)
{
	string output = "";
	for (int i = 0; i < input.length(); i++)
	{
		stringstream s;
		char z = input[i];
		s << hex << (int)z << endl;
		string getsubstr = s.str();
		getsubstr = getsubstr.substr(0, getsubstr.length() - 1);
		output += getsubstr;
	}

	return output;
}

string getConstantValue(string operand)
{
	string input = operand.substr(2, operand.length() - 3);
	return stringToHex(input);
}
