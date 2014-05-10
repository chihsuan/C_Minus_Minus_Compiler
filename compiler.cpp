/* c-like compiler project 
 * main program
 *************************/
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include "lib/Lexier.h"
#include "lib/Token.h"

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
	
	cout << "--Lexial Analysis success!" << endl;

	//Start Syntax Analysis
	vector<Token*> token_list = lexier -> getTokenList();	

	for( vector<Token*>::iterator itr = token_list.begin(); itr != token_list.end(); ++itr){
		cout << "Line " << itr[0] -> getLineNumber() << "\n" << \
				right << setw(14) << itr[0] -> getCatergory() << " : "  << itr[0] -> getToken() << endl;
	}
	//Start Semantic Analysis
	
	//IR Gerneration
	
	//Code Gerneration
	delete lexier;
	return 0;
}
