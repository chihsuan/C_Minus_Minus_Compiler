#include "Token.h"

Token:: Token(string symbol, string catergory, int line_number)
	:symbol(symbol),
	 catergory(catergory),
 	 line_number(line_number)
{
	if( catergory.compare("Identifier") == 0 ){
	  		token = "id";
	}
	else{
		token = catergory;
	}

}

string Token:: getSymbol(){
	
	return symbol;
}

string Token:: getCatergory(){
	
	return catergory;
}


string Token:: getToken(){
	return token;
}

void Token:: setType(string type){
	this-> type = type;
}

string Token:: getType(){
	return type;
}

void Token:: setScope(int scope){
	this -> scope = scope;
}

int Token:: getScope(){
	return scope;
}

int Token:: getLineNumber(){
	return line_number;
}


