#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class CodeGenerator{

	public:
		CodeGenerator();
		~CodeGenerator();
	
		void semanticRules();
		void symbolTable();
		void threeAddressCode();
		void quadruples();
		void machineCode();

	private:
		void outputSymbol();
		void outputQuadruples();
		void outputTMCode();

};
