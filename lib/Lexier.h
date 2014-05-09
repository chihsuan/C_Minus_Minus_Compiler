#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>


#ifndef LEXIER_H 
#define LEXIER_H

using namespace std;

class Lexier{

	public:
		Lexier(string input_path);
		bool startParseTokens();
				
	private:
		string input_path;
	    
	    bool enter_DFA(ofstream& output, char* token);
		void startState(int& state, char next_char, string&tokenCategory);
		void move(int&state, char next_char, string& tokenCategory);
		ifstream& getInput();
		ofstream& getOfStream();
};

#endif
