/* c-like compiler project 
 * main program
 *************************/
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include "lib/Lexier.h"
#include "lib/Token.h"
#include "lib/Parser.h"
#include "lib/Nonterminal.h"
#include "lib/Semantic.h"
#include "lib/InterCodeGenerator.h"

using namespace std;

int main(int argc, char* argv[]){

	Lexier* lexier;
	Parser* parser;
	Semantic* semantic_analyzer;

	if (argc < 2){
		cout << "ERROR:argv[1] should be the input souce code!" << endl;
		exit(1);
	}
	else if (argc < 3){
		cout << "ERROR:argv[2] should be the Grammar.txt" << endl;
		exit(1);
	}

	cout << "Start compile input_file = " << argv[1] << endl;
	
	/* Start Lexial Analysis */
	cout << "Start Lexial Analysis" << endl;	

	lexier = new Lexier(argv[1]);
	lexier -> startParseTokens();
	
	cout << "--Lexial Analysis success!" << endl;

	
	//*********************************/
	vector<Token*> token_list = lexier -> getTokenList();	

	/*int line = 0;
	for( vector<Token*>::iterator itr = token_list.begin(); itr != token_list.end(); ++itr){
		if( line != itr[0] -> getLineNumber() ){
			line = itr[0] -> getLineNumber();
			cout << "Line " << line << endl;
		}
		cout << left << "\t" << setw(13) << itr[0] -> getCatergory() << " : "  << itr[0] -> getToken() << endl;
	}*/

	/* Start Syntax Analysis */
	cout << "Start Syntax Analysis" << endl;
	parser = new Parser(argv[2]);
	parser -> startParsing( lexier-> getTokenList() );

	cout << "--Parsing success!" << endl;
	/* Start Semantic Analysis */
	semantic_analyzer = new Semantic();
	semantic_analyzer -> analysis( parser-> getParsingTree() );

	/* IC Gerneration */
	
	/* Code Gerneration */


	delete lexier;
	delete parser;
	delete semantic_analyzer;
	return 0;
}
