#include <iostream>
#include <string>
#include "Token.h"


Token:: Token(string token, string catergory, int line_number){
	this -> token = token;
	this -> catergory = catergory;
	this -> line_number = line_number;
}

string Token:: getToken(){
	
	return token;
}

string Token:: getCatergory(){
	
	return catergory;
}

int Token:: getLineNumber(){
	return line_number;
}


