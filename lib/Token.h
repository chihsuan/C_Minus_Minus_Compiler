#include <iostream>
#include <string>

using namespace std;

#ifndef TOKEN_H 
#define TOKEN_H

class Token{
	public:
		Token(string token, string catergory, int line_number);
		string getSymbol();
		string getCatergory();
		int getLineNumber();
	private:
		string symbol;
		string catergory;
		int line_number;
		
};

#endif
