#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include "Nonterminal.h"

using namespace std;

#ifndef PARSER_H 
#define PARSER_H

class Parser{
	
	public:
		Parser(char* input_path);
		~Parser();
		void inputGrammar(char* input_path);
		void startParsing();
					

	private:
		map <Nonterminal*, vector<list<string> > > grammar;
		map <string, Nonterminal*> non_terminals_map;
		vector<Nonterminal*> non_terminals;

		bool isTerminal(string term);
		bool nullable(string non_terminal);
		set<string> first(string non_terminal);
		void follow();
		
		void outputSet();
		void outputParingTable();
		void outputParingTree();
		map<Nonterminal*, vector<list<string> > >::iterator findGrammar(string non_terminal);
};

#endif
