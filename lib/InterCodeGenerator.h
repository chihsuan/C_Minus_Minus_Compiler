#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class InterCodeGenerator{

	public:
		InterCodeGenerator();
		~InterCodeGenerator();
	
		void threeAddressCode();
		void quadruples();

	private:
		void outputQuadruples();
};
