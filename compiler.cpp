/* c-like compiler project 
 * main program
 *************************/
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "lib/Lexier.h"

using namespace std;

int main(int argc, char* argv[]){

	Lexier* lexier;

	if (argc < 2){
		cout << "ERROR:argv[1] should be the input souce code!" << endl;
		exit(1);
	}

	cout << "Start compile input_file = " << argv[1] << endl;
	
	//Start Lexial Analysis  
	cout << "Start Lexial Analysis" << endl;	

	
	lexier = new Lexier(argv[1]);

	lexier -> startParseTokens();
	delete lexier;
	cout << "--Lexial Analysis success!" << endl;
	
	//Start Syntax Analysis
	
	//Start Semantic Analysis
	
	//IR Gerneration
	
	//Code Gerneration
	return 0;
}
