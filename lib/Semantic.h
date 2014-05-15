#ifndef SEMANTIC_H 
#define SEMANTIC_H

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <cstdlib>
#include "Node.h"
#include "Symbol.h"
#include <iomanip>
#include <stack>

using namespace std;


class Semantic{

	public:
		Semantic();
		void analysis(multimap<int, Node> parsingTree);
	private:
		multimap<int, Symbol> symbolTable;

		void buildSymbolTable(multimap<int, Node> parsingTree);
		
		bool findSymbol(stack<string> input, string symbol);
		bool tableInsert(stack<string> input, string symbol);
		void outputSymbolTable();

};

#endif
