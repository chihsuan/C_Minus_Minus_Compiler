#ifndef QUADRUPLES_H 
#define QUADRUPLES_H

#include <iostream>
#include <string>

using namespace std;


class Quadruples{

	public:
		Quadruples(string op, string arg1, string arg2, string result);

		string getOp();
		string getArg1();
		string getArg2();
		string getResult();

	private:
		string op;
		string arg1;
		string arg2;
		string result;
};

#endif
