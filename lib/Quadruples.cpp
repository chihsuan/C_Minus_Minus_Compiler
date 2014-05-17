#include "Quadruples.h"

Quadruples:: Quadruples(string op, string arg1, string arg2, string result){
	this -> op = op;
	this -> arg1 = arg1;
	this -> arg2 = arg2;
	this -> result = result;
}

string Quadruples:: getOp(){
	return op;
}
string Quadruples:: getArg1(){
	return arg1;
}
string Quadruples:: getArg2(){
	return arg2;
}
string Quadruples:: getResult(){
	return result;
}


