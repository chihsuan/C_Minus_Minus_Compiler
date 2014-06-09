#ifndef TOKEN_H 
#define TOKEN_H

#include <iostream>
#include <string>

using namespace std;
class Token{

	public:
		Token(string symbol, string catergory, int line_number);
		
		string getSymbol();
		string getCatergory();
		string getToken();
		string getType();
		int getLineNumber();
		int getScope();
		
		void setType(string type);
		void setScope(int scope);
		
	private:
		string symbol;
		string token;
		string type;
		string catergory;
		int scope;
		int line_number;
		
};

#endif
