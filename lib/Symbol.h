#include <iostream>
#include <string>

using namespace std;

#ifndef SYMBOL_H 
#define SYMBOL_H

class Symbol{

	public:
		Symbol(string symbol, string token, string type, int scope);

		string getSymbol();
		string getToken();
		string getType();
		int getScope();

	private:
		string symbol;
		string token;
		string type;
		int scope;

};

#endif
