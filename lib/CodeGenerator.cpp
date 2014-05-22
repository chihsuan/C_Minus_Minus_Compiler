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
		
	int quadruples_count = 0;
	string op;
	string arg1;
	string arg2;
	string result;
	string finalString;
	stack<int> jf_count_stack;
	stack<int> jp_count_stack;
	stack<int> jp_pos_stack;
	stack<int> jf_pos_stack;
	stack<int> until_jf_stack;
	stack<int> until_jp_stack;
	stack<string> jf_tmp_stack;

	// iterator quadruples to convert to machine code
	for(vector<Quadruples>::iterator itr = quadruples.begin(); itr != quadruples.end(); ++itr ){
		
		quadruples_count += 1;
		op = itr -> getOp();
		arg1 = itr-> getArg1();
		arg2 = itr-> getArg2();
		result = itr -> getResult();	
		
		// add if jmp
		if( quadruples_count == until_jf_stack.top() ){
			machine_code.insert( machine_code.begin() + jf_pos_stack.top(), fromatCode( jf_count_stack.top(), \
									"JEQ", getReg( jf_tmp_stack.top() ), toString(count) + "(0)", "" ) );	
			jf_tmp_stack.pop();
			jf_pos_stack.pop();
			jf_count_stack.pop();
			until_jf_stack.pop();
		}
		
		// add end if jmp
		if( quadruples_count == until_jp_stack.top() ){
			machine_code.insert( machine_code.begin() + jp_pos_stack.top(), fromatCode( jp_count_stack.top(), "LDA", "7", toString(count) + "(0)", "" ) );	
			jp_pos_stack.pop();
			jp_count_stack.pop();
			until_jp_stack.pop();
		}
		
		// is assign digit
		if( op.compare("=") == 0  && isdigit( itr-> getArg1()[0] ) ){
			machine_code.push_back( fromatCode(count++, "LDC", getReg(arg1), arg1 + "(0)", "" ) );
			machine_code.push_back( fromatCode(count++, "LDC", "0", "0(0)", "") );
			machine_code.push_back( fromatCode(count++, "ST", getReg(arg1), getMem(result) + "(0)", "") ) ;
			finalString = arg1;
		}
		// is assign identifier
		else if( op.compare("=") == 0 ){
			machine_code.push_back( fromatCode(count++, "LD",  getReg(arg1), getMem(arg1) + "(0)" , "" ) );
			machine_code.push_back( fromatCode(count++, "LDC", "0", "0(0)", "" ) );
			machine_code.push_back( fromatCode(count++, "ST",  getReg(arg1), getMem(result) + "(0)", "" ) );
			finalString = arg1;
		}
		//  jmp if 
		else if( op.compare("jf") == 0){
			machine_code.push_back( fromatCode(count++, "LDC", "0", "0(0)", "") );
			jf_count_stack.push( count++ );
			jf_tmp_stack.push( arg2 );
			jf_pos_stack.push( machine_code.size()  );
			until_jf_stack.push( atoi( arg1.c_str() ) );
		} // jmp
		else if( op.compare("jp") == 0){
			machine_code.push_back( fromatCode(count++, "LDC", "0", "0(0)", "") );
			jp_count_stack.push( count++ );
			jp_pos_stack.push( machine_code.size()  );
			until_jp_stack.push( atoi( arg1.c_str() ) );
		}	
		// is operator and ALU operate
		else{
		
			// lw arg1
			if( isdigit( itr-> getArg1()[0] ) ){
				machine_code.push_back( fromatCode(count++, "LDC", getReg(arg1), arg1 + "(0)", "" ) );
			}
			else{
				machine_code.push_back( fromatCode(count++, "LDC", "0", "0(0)", "" ) );
				machine_code.push_back( fromatCode(count++, "LD",  getReg(arg1), getMem(arg1) + "(0)" , "" ) );
			}

			// arg2 lw digit
			if( isdigit( itr-> getArg2()[0] ) ){
				machine_code.push_back( fromatCode(count++, "LDC", getReg(arg2), arg2 + "(0)", "" ) );
			}
			// arg2 lw identifier
			else{
				machine_code.push_back( fromatCode(count++, "LDC", "0", "0(0)", "" ) );
				machine_code.push_back( fromatCode(count++, "LD",  getReg(arg2), getMem(arg2) + "(0)" , "" ) );
			}

			// alu operater
			machine_code.push_back( fromatCode(count++, getOpCode(op),  getReg(result), getReg(arg1) , getReg(arg2)) );
			machine_code.push_back( fromatCode(count++, "ST",  getReg(result), getMem(result) + "(0)", "" ) );
			
			finalString = result;
		}
	}

	while( !until_jf_stack.empty() ){
			machine_code.insert( machine_code.begin() + jf_pos_stack.top(), fromatCode( jf_count_stack.top() , \
									"JEQ", getReg( jf_tmp_stack.top()  ), toString(count) + "(0)", "" ) );	
			jf_tmp_stack.pop();
			jf_pos_stack.pop();
			jf_count_stack.pop();
			until_jf_stack.pop();
	}
	while ( !until_jp_stack.empty() ){
			machine_code.insert( machine_code.begin() + jp_pos_stack.top(), fromatCode( jp_count_stack.top(), "LDA", "7", toString(count) + "(0)", "" ) );	
			jp_pos_stack.pop();
			jp_count_stack.pop();
			until_jp_stack.pop();
	} 


	// print the last result and stop machine
	machine_code.push_back( fromatCode(count++, "OUT",  getReg(finalString), "0", "0" ) );
	machine_code.push_back( fromatCode(count++, "HALT",  "1", "0", "0" ) );
	
	outputFile();
}


// get available reg or it already in reg
string CodeGenerator:: getReg(string symbol){
	
	map<string, int>::iterator itr =  str_reg.find(symbol);

	if( itr != str_reg.end() ){
		return toString(itr -> second);
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

	return toString(reg);
}



// get available mem space
string CodeGenerator:: getMem(string symbol){
		

	map<string, int>::iterator itr =  variable_mem.find(symbol);

	if( itr != variable_mem.end() ){
		return toString( itr -> second );
	}
	else{
		variable_mem.insert( make_pair(symbol, set_mem) );
		return toString( set_mem++ );
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


void CodeGenerator:: outputFile(){

	/*  output stream for write the result */
    ofstream output( "output/code.tm", ios::out);

	if (!output){
		cerr << "File could not be opened" << endl;
		exit(1);
	}

	for( vector<string>:: iterator itr = machine_code.begin(); itr != machine_code.end(); ++itr){
	
		//cout << *itr << endl;
		output <<  *itr << endl;
	}

	output.close();
}

string CodeGenerator:: toString(int integer){
	stringstream ss;
	ss << integer;
	return ss.str();
}
string CodeGenerator:: fromatCode(int count, string op, string arg1, string arg2, string arg3){
	
	if( arg3.size() > 0 ){
		return toString(count) + ": " + op + " " + arg1 + "," + arg2 + "," + arg3;
	}
	else{
		return toString(count) + ": " + op + " " + arg1 + "," + arg2;
	}
}
