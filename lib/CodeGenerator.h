#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class CodeGenerator{

	public:
		CodeGenerator();
		~CodeGenerator();
	
		void semanticRules();
		void threeAddressCode();
		void quadruples();
		void machineCode();

	private:
		void outputQuadruples();
		void outputTMCode();
};
