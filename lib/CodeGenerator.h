#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class CodeGenerator{

	public:
		CodeGenerator();
		~CodeGenerator();
	
		void machineCode();

	private:
		void outputTMCode();
};
