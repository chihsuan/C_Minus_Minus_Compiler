#ifndef LEXIER_H 
#define LEXIER_H

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <iomanip>
#include "Token.h"


using namespace std;

class Lexier{

	public:
		Lexier(string input_path);
		~Lexier();
		bool startParseTokens();
		vector<Token*> getTokenList();

	private:
		string input_path;
	   	vector<Token*> token_list; 
		int keyword_move;
		int state;

	    bool enter_DFA(string token, string& tokenCategory);
		void startState(int& state, char next_char, string& tokenCategory);
		void move(int& state, char next_char, string& tokenCategory);
		ifstream& getInput();
		ofstream& getOfstream();
};

#endif
