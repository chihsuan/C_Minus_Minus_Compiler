#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "Quadruples.h"

using namespace std;

class CodeGenerator{

	public:
		CodeGenerator();
		~CodeGenerator();
	
		void gernerateCode(vector<Quadruples> quadruples);

	private:
		void outputTMCode();
};
