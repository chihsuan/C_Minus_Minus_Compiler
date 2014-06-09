#include "Symbol.h"

using namespace std;

Symbol:: Symbol(string symbol, string token, string type, int scope){
	this -> symbol = symbol;
	this -> token = token;
	this -> type = type;
	this -> scope = scope;

}

string Symbol:: getSymbol(){
	return symbol;
}
string Symbol:: getToken(){
	return token;
}
string Symbol:: getType(){
	return type;
}
int Symbol:: getScope(){
	return scope;
}
