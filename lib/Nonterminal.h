#ifndef NONTERMINAL_H
#define NONTERMINAL_H

#include <iostream>
#include <string>
#include <vector>
#include <set>
using namespace std;

class Nonterminal{

	public:
		string term;
		bool nullable;
		int production_nullable;
		set<string> first;
		set<string> follow;

		Nonterminal(string term);	
		void insertFollow(set<string> const set);
		void insertFirst(set<string> const set);

	private:
};

#endif
