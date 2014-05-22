#include <iostream>
#include <string>
#include "CodeGenerator.h"
using namespace std;


CodeGenerator:: CodeGenerator()
	:set_mem(0),
	 count(0),
	 reg(0)
{

}
CodeGenerator:: ~CodeGenerator(){

}

void CodeGenerator:: gernerateCode(vector<Quadruples> quadruples){
		
	string op;
	string arg1;
	string arg2;
	string result;
	string finalString;
	ofstream& output = getOfStream();

	// iterator quadruples to convert to machine code
	for(vector<Quadruples>::iterator itr = quadruples.begin(); itr != quadruples.end(); ++itr ){
		
		op = itr -> getOp();
		arg1 = itr-> getArg1();
		arg2 = itr-> getArg2();
		result = itr-> getResult();
		
		// is assign digit
		if( itr-> getArg2().size() == 0  && isdigit( itr-> getArg1()[0] ) ){
			output << count++ << ": " << "LDC " <<  getReg(arg1) << "," << arg1 + "(0)" << endl;
			output << count++ << ": "  << "LDC 0,0(0)" << endl;
			output << count++ << ": " << "ST " << getReg(arg1) << "," << getMem( result )  << "(0)" << endl;
			finalString = arg1;
		}
		// is assign identifier
		else if( itr -> getArg2().size() == 0 ){
			output << count++ << ": " << "LD " << getReg(arg1) << ","  << getMem( arg1 ) << "(0)" << endl;
			output << count++ << ": "  << "LDC 0,0(0)" << endl;
			output << count++ << ": " << "ST " << getReg(arg1) << "," << getMem( result )  << "(0)" << endl;
			finalString = arg1;
		}
		// is operator and ALU operate
		else{
			
			// lw arg1
			if( isdigit( itr-> getArg1()[0] ) ){
				output << count++ << ": " << "LDC " << getReg(arg1) << "," << arg1 + "(0)" << endl;
			}
			else{
				output << count++ << ": "  << "LDC 0,0(0)" << endl;
				output << count++ << ": " << "LD " << getReg(arg1) << ","  << getMem( arg1 ) << "(0)" << endl;
			}

			// lw arg2
			if( isdigit( itr-> getArg2()[0] ) ){
				output << count++ << ": " << "LDC " << getReg( arg2 ) << "," << arg1 + "(0)" << endl;
			}
			else{
				output << count++ << ": "  << "LDC 0,0(0)" << endl;
				output << count++ << ": " << "LD " << getReg(arg2) << "," << getMem( arg2 ) << "(0)" << endl;
			}

			// alu operater
			output << count++ << ": " << getOpCode( op ) + " "  << getReg(result) << ","  << getReg(arg1)  << "," << getReg(arg2) << endl;
			output << count++ << ": " << "ST " << getReg(result) << "," << getMem( result ) << "(0)" << endl; 
			finalString = result;
		}	
	}

	// print the last result and stop machine
	output << count++ << ": " << "OUT " << getReg(finalString) << ",0,0"<< endl; 
	output << count++ << ": HALT 1,0,0"<< endl; 
	
	output.close();
}


// get available reg or it already in reg
int CodeGenerator:: getReg(string symbol){
	
	map<string, int>::iterator itr =  str_reg.find(symbol);

	if( itr != str_reg.end() ){
		return itr -> second;
	}
	else{
		reg += 1;
		if( reg_queue.size() >= 6 ){
			itr = str_reg.find( reg_queue.front() );
			reg = itr -> second;
			str_reg.erase( itr );
			reg_queue.pop();
		}
		str_reg.insert( make_pair(symbol, reg) );
		reg_queue.push( symbol );
	}

	return reg;
}



// get available mem space
int CodeGenerator:: getMem(string symbol){
		

	map<string, int>::iterator itr =  variable_mem.find(symbol);

	if( itr != variable_mem.end() ){
		return itr -> second;
	}
	else{
		variable_mem.insert( make_pair(symbol, set_mem) );
		return set_mem++;
	}


}


/* convet op symobl to machine code*/ 
string CodeGenerator:: getOpCode(string op){

	if( op.size() > 1 ){
		if(op.compare("<=") == 0){
				return "LE";

		}
		if(op.compare(">=") == 0){
				return "GE";

		}
		if(op.compare("==") == 0){
				return "EQ";

		}
		if(op.compare("!=") == 0){
				return "NE";

		}
		if(op.compare("&&") == 0){
				return "AND";

		}
	
	}
	else{
			switch(op[0]){
			
					case '+':
							return "ADD";
					case '-':
							return "SUB";
					case '*':
							return "MUL";
					case '/':
							return "DIV";
					case '!':
							return "NOT";
					case '>':
							return "GT";
					case '<':
							return "LT";
					default:
							return "Error";
			}
	}
}


ofstream& CodeGenerator:: getOfStream(){

	/*  output stream for write the result */
    static ofstream outputFile( "output/code.tm", ios::out);

	if (!outputFile){
		cerr << "File could not be opened" << endl;
		exit(1);
	}

	return outputFile;
}
