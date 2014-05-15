#include <iostream>
#include <string>

using namespace std;

#ifndef TOKEN_H 
#define TOKEN_H

class Token{
	public:
		Token(string symbol, string catergory, int line_number);
		string getSymbol();
		string getCatergory();
		int getLineNumber();
		
		string getToken();
		
		void setType(string type);
		string getType();
		
		void setScope(int scope);
		int getScope();
		
	private:
		string symbol;
		string token;
		string type;
		int scope;
		string catergory;
		int line_number;
		
};

#endif
