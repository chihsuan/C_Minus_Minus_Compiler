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

void CodeGenerator:: gernerateCode(multimap<int, Node> parsingTree, vector<Quadruples> quadruples){
		
	int quadruples_count = 0;
	bool array_op = false;
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
	stack<string> array_stack;
	string array_name1;
	string array_name2;
	string offset1;
	string offset2;

	initMemory(parsingTree);

	// iterator quadruples to convert to machine code
	for(vector<Quadruples>::iterator itr = quadruples.begin(); itr != quadruples.end(); ++itr ){
		
		quadruples_count += 1;
		op = itr -> getOp();
		arg1 = itr-> getArg1();
		arg2 = itr-> getArg2();
		result = itr -> getResult();	

		// add JEQ
		if( !until_jf_stack.empty() && quadruples_count == until_jf_stack.top() ){
			machine_code.insert( machine_code.begin() + jf_pos_stack.top(), fromatCode( jf_count_stack.top(), \
									"JEQ",  jf_tmp_stack.top(), toString(count) + "(0)", "" ) );	
			jf_tmp_stack.pop();
			jf_pos_stack.pop();
			jf_count_stack.pop();
			until_jf_stack.pop();
		}
		
		// add LDA
		if( !until_jp_stack.empty() && quadruples_count == until_jp_stack.top() ){
			machine_code.insert( machine_code.begin() + jp_pos_stack.top(), fromatCode( jp_count_stack.top(), "LDA", "7", toString(count) + "(0)", "" ) );	
			jp_pos_stack.pop();
			jp_count_stack.pop();
			until_jp_stack.pop();
		}
		
		if( op.compare("[]=") == 0 ){
			array_stack.push( arg1  );
			array_stack.push( result );
			continue;
		}
		else if( !array_stack.empty() && result.compare( array_stack.top() ) == 0){
			array_stack.pop();
			machine_code.push_back( fromatCode(count++, "LDC", "0", "0(0)", "" ) );
			machine_code.push_back( fromatCode(count++, "LD",  getReg(arg1), getMem(arg1) + "(0)" , "" ) );
			machine_code.push_back( fromatCode(count++, "LD",  getReg( array_stack.top() ), getMem( array_stack.top() ) + "(0)" , "" ) );
			machine_code.push_back( fromatCode(count++, "LDC",  getReg(result), getMem(result) + "(0)", "" ) );
			machine_code.push_back( fromatCode(count++, "ADD",  getReg( result ),  getReg(result),  getReg( array_stack.top() ) ) );
			machine_code.push_back( fromatCode(count++, "ST",  getReg(arg1),    "0(" + getReg(result) + ")", "" ) );
			array_stack.pop();
			continue;
		}		
		
		if( !array_stack.empty() && arg1.compare( array_stack.top() ) == 0  ){
			array_name1 = array_stack.top();
			array_stack.pop();
			machine_code.push_back( fromatCode(count++, "LDC", "0", "0(0)", "" ) );
			machine_code.push_back( fromatCode(count++, "LD",  getReg( array_name1 + array_stack.top() ),\
									getMem( array_stack.top() ) + "(0)" , "" ) );
			machine_code.push_back( fromatCode(count++, "LDC",  getReg( array_name1 ), getMem( array_name1 ) + "(0)", "" ) );
			machine_code.push_back( fromatCode(count++, "ADD",  getReg( array_name1 + array_stack.top() ),  getReg( array_name1 ),\
									getReg( array_name1 + array_stack.top() ) ) );
			machine_code.push_back( fromatCode(count++, "LD",  getReg( array_name1 + array_stack.top() ), \
									"0(" + getReg( array_name1 + array_stack.top() ) + ")", "" ) );
			array_op = true;
			offset1 = array_stack.top();
			array_stack.pop();
		}
		
		if( !array_stack.empty() && arg2.compare( array_stack.top() ) == 0){
			array_name2 = array_stack.top();
			array_stack.pop();
			machine_code.push_back( fromatCode(count++, "LDC", "0", "0(0)", "" ) );
			machine_code.push_back( fromatCode(count++, "LD",  getReg( array_name1 + array_stack.top() ),\
									getMem( array_stack.top() ) + "(0)" , "" ) );
			machine_code.push_back( fromatCode(count++, "LDC",  getReg( array_name2 ), getMem( array_name2 ) + "(0)", "" ) );
			machine_code.push_back( fromatCode(count++, "ADD",  getReg( array_name2 + array_stack.top() ),  getReg( array_name2 ),\
									getReg( array_name2 + array_stack.top() ) ) );
			machine_code.push_back( fromatCode(count++, "LD",  getReg( array_name2 + array_stack.top() ), \
									"0(" + getReg( array_name2 + array_stack.top() ) + ")", "" ) );
			array_op = true;
			offset2 = array_stack.top();
			array_stack.pop();
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
		//  jmp if false 
		else if( op.compare("jf") == 0){
			machine_code.push_back( fromatCode(count++, "LDC", "0", "0(0)", "") );
			jf_count_stack.push( count++ );
			jf_tmp_stack.push( getReg(arg2) );
			jf_pos_stack.push( machine_code.size()  );
			until_jf_stack.push( atoi( arg1.c_str() ) );
		} // jmp
		else if( op.compare("jp") == 0){
			int go_to = atoi( arg1.c_str() );
		
			machine_code.push_back( fromatCode(count++, "LDC", "0", "0(0)", "") );
			if( go_to < quadruples_count ){
				machine_code.push_back( fromatCode( count++, "LDA", "7", toString( jf_count_stack.top() - 6 ) + "(0)", "" ) );	
			}
			else{
				jp_count_stack.push( count++ );
				jp_pos_stack.push( machine_code.size() + 1 );
				until_jp_stack.push( atoi( arg1.c_str() ) );
			}
		}	
		// is operator and ALU operate
		else{

			// lw arg1
			if( isdigit( itr-> getArg1()[0] ) ){
				machine_code.push_back( fromatCode(count++, "LDC", getReg(arg1), arg1 + "(0)", "" ) );
			}
			else if ( arg1.compare(array_name1) != 0  && arg1.compare(array_name2) != 0 ){
				machine_code.push_back( fromatCode(count++, "LDC", "0", "0(0)", "" ) );
				machine_code.push_back( fromatCode(count++, "LD",  getReg(arg1), getMem(arg1) + "(0)" , "" ) );
			}
			else if ( arg1.compare(array_name1) == 0 ){
				arg1 = arg1 + offset1;
			}
			else{
				arg1 = arg1 + offset2; 
			}
	
			// arg2 lw digit
			if( isdigit( itr-> getArg2()[0] ) ){
				machine_code.push_back( fromatCode(count++, "LDC", getReg(arg2), arg2 + "(0)", "" ) );
			}
			// arg2 lw identifier
			else if ( arg2.compare(array_name1) != 0 && arg2.compare(array_name2) != 0 ){
				machine_code.push_back( fromatCode(count++, "LDC", "0", "0(0)", "" ) );
				machine_code.push_back( fromatCode(count++, "LD",  getReg(arg2), getMem(arg2) + "(0)" , "" ) );
			}
			else if ( arg1.compare(array_name1) == 0 ){
				arg2 = arg2 + offset1;
			}
			else{
				arg2 = arg2 + offset2; 
			}
	
			// alu operater
			if ( !array_op ){
				machine_code.push_back( fromatCode(count++, getOpCode(op),  getReg(result), getReg(arg1) , getReg(arg2)) );
			}
			else{
				machine_code.push_back( fromatCode(count++, getOpCode(op),  getReg(result), getReg(arg1) , getReg(arg2)) );
				array_stack.pop();
				array_name1 = "";
				array_name2 = "";
				array_op = false;
			}
			machine_code.push_back( fromatCode(count++, "ST",  getReg(result), getMem(result) + "(0)", "" ) );
			finalString = result;
		}
	}

	while( !until_jf_stack.empty() ){
			machine_code.insert( machine_code.begin() + jf_pos_stack.top(), fromatCode( jf_count_stack.top() , \
									"JEQ", jf_tmp_stack.top(), toString(count) + "(0)", "" ) );	
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

void CodeGenerator:: initMemory(multimap<int, Node> parsingTree){

	bool varDecl = false;
	string var_name;

	for(multimap<int, Node>::iterator itr = parsingTree.begin(); itr != parsingTree.end(); ++ itr){

		// is decl
		if ( itr -> second.token.compare("Type") == 0 ){
			varDecl = true;
		}
		else if ( varDecl && ( itr-> second.catergory.compare("Identifier") == 0) ){
			
			var_name = (itr++) -> second.symbol;

			// array initailize
			if ( (++itr) -> second.symbol.compare("[") == 0 ){
				int array_size = atoi( (++itr) -> second.symbol.c_str() ) - 1;	
				getMem( var_name );
				while( array_size > 0 ){
					getMem( var_name + "[" + toString(array_size) + "]" );
					-- array_size;
				}
			}// normal variable initailize
			else{
				getMem(var_name);
			}
			varDecl = false;
		}
	}
}


// get available reg or it already in reg
string CodeGenerator:: getReg(string symbol){
	
	map<string, int>::iterator itr =  str_reg.find(symbol);

	if( itr != str_reg.end() ){
		if( reg_queue.front().compare( symbol ) == 0 ){
			reg_queue.pop();
			reg_queue.push( symbol );
		}	
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


/* convet op symbol to machine code*/ 
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
		if(op.compare("&&") == 0){
				return "OR"
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
					case '&':
							return "AND";
					case '|':
							return "OR";
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
