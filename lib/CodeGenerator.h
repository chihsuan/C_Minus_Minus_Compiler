#ifndef CODEGENERATOR_H 
#define CODEGENERATOR_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <stack>
#include <cstring>
#include "Node.h"
#include "Quadruples.h"

using namespace std;

class CodeGenerator{

	public:
		CodeGenerator();
		~CodeGenerator();
	
		void gernerateCode(multimap<int, Node> parsingTree, vector<Quadruples> quadruples);

	private:
		int set_mem;
		int count;
		int reg;
		int last_reg;
		queue<string> reg_queue;
		map<string, int> str_reg;
		map<string, int> variable_mem;
		vector<string> machine_code;

		void initMemory(multimap<int, Node> parsingTree);
		string getReg(string symbol);
		string getMem(string symbol);
		string fromatCode(int count, string op, string arg1, string arg2, string arg3);
		string toString(int integer);
		string getOpCode(string op);
		void outputFile();
};

#endif
