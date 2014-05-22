#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "Quadruples.h"
#include <map>
#include <set>
#include <queue>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <stack>
#include <cstring>

using namespace std;

class CodeGenerator{

	public:
		CodeGenerator();
		~CodeGenerator();
	
		void gernerateCode(vector<Quadruples> quadruples);

	private:
		int set_mem;
		int count;
		int reg;

		string getReg(string symbol);
		string getMem(string symbol);
		vector<string> machine_code;
		queue<string> reg_queue;
		map<string, int> str_reg;

		map<string, int> variable_mem;

		string fromatCode(int count, string op, string arg1, string arg2, string arg3);
		string toString(int integer);
		string getOpCode(string op);
		void outputFile();
};
