#include <iostream>
#include <string>
#include <vector>
#include <set>
using namespace std;

#ifndef NONTERMINAL_H
#define NONTERMINAL_H

class Nonterminal{

	public:
		string non_terminal;
		bool nullable;
		set<string> first;
		set<string> follow;

		Nonterminal(string non_terminal);	

	private:
};

#endif
