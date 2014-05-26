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
#include "lib/CodeGenerator.h"
using namespace std;

int main(int argc, char* argv[]){

	Lexier* lexier;
	Parser* parser;
	Semantic* semantic_analyzer;
	InterCodeGenerator* interCodeGenerator;
	CodeGenerator* codeGenerator;

	/*  file input path */
	if (argc < 2){
		cout << "ERROR:argv[1] should be the input souce code!" << endl;
		exit(1);
	}
	else if (argc < 3){
		cout << "ERROR:argv[2] should be the Grammar.txt" << endl;
		exit(1);
	}

	cout << "Start compile .c file = " << argv[1] << "grammar = " \
		 <<	argv[2] << endl;


	/* Start Lexial Analysis */
	cout << "1.Start Lexial Analysis" << endl;	
	lexier = new Lexier(argv[1]);
	lexier -> startParseTokens();
	cout << "--Lexial Analysis success!" << endl;

	/* Start Syntax Analysis */
	cout << "2.Start Syntax Analysis" << endl;
	parser = new Parser(argv[2]);
	parser -> startParsing( lexier-> getTokenList() );
	cout << "--parsering success!" << endl;
	delete lexier;
	
	/* Start Semantic Analysis */
	cout << "3.Start Semantic Analysis" << endl;
	semantic_analyzer = new Semantic();
	semantic_analyzer -> analysis( parser-> getParsingTree() );
	cout << "--Semantic success!" << endl;
	delete semantic_analyzer;
		
	cout << "4.Start InterCodeGenerator" << endl;
	/* IC Gerneration */
	interCodeGenerator = new InterCodeGenerator();
	interCodeGenerator -> startGenerate( parser -> getParsingTree() );
	cout << "--interCodeGenerator success!" << endl;

	cout << "5.Start produce machine code" << endl;
	/* Code Gerneration */
	codeGenerator = new CodeGenerator();
 	codeGenerator -> gernerateCode( parser -> getParsingTree(), interCodeGenerator-> getQuadruples());
	cout << "--compile success!" << endl;
	

	delete parser;
	delete interCodeGenerator;
	delete codeGenerator;
	return 0;
}
