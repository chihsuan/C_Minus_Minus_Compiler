#include <iostream>
#include <string>
#include "InterCodeGenerator.h"
#include <sstream>
#include <fstream>
using namespace std;


InterCodeGenerator:: InterCodeGenerator()
	:name_index(0) // temperary name index for quadruples
{

}
InterCodeGenerator:: ~InterCodeGenerator(){

}

void InterCodeGenerator:: startGenerate(multimap<int, Node> parsingTree){


	bool in_Expr = false;  // is in Expr block ?
	int Expr_index = -1;   // record the Expr index
	bool in_while = false;
	int while_index = -1;

	stringstream ss;
	queue<Node> tmp;      // tmp stack push expr symbol    
	queue<Node> postfix_expr;
	stack<int> while_jmp_stack;
	stack<int> if_jmp_stack;
	stack<string> if_tmp_stack;
	stack<int> else_jmp_stack;
	stack<int> else_index;

	/* ParsingTree ->  produce three address code by postfix */
	for(multimap<int, Node>::iterator itr = parsingTree.begin(); itr != parsingTree.end(); ++ itr){
		
		string const & token =  itr -> second.token;
		string symbol =  itr -> second.symbol;
		string catergory = itr -> second.catergory;
		int index = itr -> second.index;
		
		if( !in_Expr && token.compare("Expr") == 0 ){

			// set flag and record index
			in_Expr = true;                     
			Expr_index = itr -> second.index;
		}
		else if( index == Expr_index ){

			// exit expr block unset flag and index
			in_Expr = false;
			Expr_index = -1;

			// converts to  postfix  format
			postfix_expr = postfix(tmp);
			
			// clear
			while( !tmp.empty() ){
				tmp.pop();
			}

			//create quadruples
			createQuadruples(postfix_expr);
		
		}/*
		else if( token.compare("while") == 0 ){
			while_index = itr -> second.index;
			in_while = true;
		}
		else if( itr-> second.index == while_index && in_while ){
			// push for insert jmp pos
			while_jmp_stack.push( quadruples.size() - 1 );
		}*/
		else if( token.compare("if") == 0 ){
			// push for insert jmp pos
			ss.str("");
			ss.clear();
			ss <<  name_index;
			if_jmp_stack.push( quadruples.size() + 1 );
			if_tmp_stack.push( "t" + ss.str() );
		}
		else if( token.compare("else") == 0 ){
			ss.str("");
			ss.clear();
			ss <<  quadruples.size() + 2 + if_jmp_stack.size() ;
			
			quadruples.insert( quadruples.begin() + if_jmp_stack.top() , Quadruples("jf", ss.str(), if_tmp_stack.top(), "" )  );
			if_jmp_stack.pop();
			if_tmp_stack.pop();
			else_jmp_stack.push( quadruples.size() );
			else_index.push( index - 1 );
		}
		else if( !else_index.empty() && index == else_index.top() ){
			ss.str("");
			ss.clear();
			ss <<  quadruples.size() + 2 + if_jmp_stack.size() ;
			
			quadruples.insert( quadruples.begin() + else_jmp_stack.top() , Quadruples("jp", ss.str(), "", "" )  );
			else_jmp_stack.pop();
			else_index.pop();
		} 

		// in expr block, push symbol 
		if( ( catergory.compare("Identifier") == 0 || catergory.compare("Operator")  == 0 \
								|| catergory.compare("Number")  == 0) || catergory.compare("ch") == 0   && in_Expr ){
			tmp.push( itr -> second );
		}
	}

	outputQuadruples();
}

queue<Node> InterCodeGenerator:: postfix(queue<Node> expr){

	stack<Node> operator_stack;
	queue<Node> result;

    while( !expr.empty() ) { 
        
        // operator, priority push
		if ( expr.front().catergory.compare("Operator") == 0 ) {
			while (!operator_stack.empty() && priority( operator_stack.top() ) \
							<= priority( expr.front() )) {
                result.push( operator_stack.top() ); 
                operator_stack.pop();
            }
            operator_stack.push( expr.front() );
        }
        // push and push until get ')' 
        else if ( expr.front().symbol.compare("(") == 0 ) {

            operator_stack.push( expr.front() );
        } 
        // pop until get '('
        else if ( expr.front().symbol.compare(")") == 0 ) {

            while ( operator_stack.top().symbol.compare("(") != 0 ) {
				result.push( operator_stack.top() );
				operator_stack.pop();
			}
			operator_stack.pop();
		}
		else{
			// push Identifier, char, num
			result.push( expr.front() );
		}

		// next element
		expr.pop();
	}

	while(!operator_stack.empty()){
		result.push( operator_stack.top() );
		operator_stack.pop();
	}

	return result;
}
int InterCodeGenerator:: priority(Node node){


	// return postfix priority 
	string op = node.symbol;

	if( op.size() == 1){
		switch( op[0] ){
			case '+': case '-':
					return 6;
			case '!': case '*': case '/':
					return 3;
			case '>': case '<':
					return 8;
			case '&':
					return 10;
			case '|': 
					return 11;
			case '=':
					return 16;
			default:
					break;
		}
	}
	else if( op.compare("<=") == 0 || op.compare(">=") == 0 )
		return 8;
	else if( op.compare("==") == 0 || op.compare("!=") == 0){
		return 9;
	}
	else if( op.compare("&&") ){
		return 13;
	}
	else if( op.compare("||") ){
		return 13;
	}

	exit(1);
}



void InterCodeGenerator:: createQuadruples(queue<Node> post_expr){

	stack<Node> pr_stack;
	stringstream ss;

	while( !post_expr.empty() ){
		
		// operator get two element in pr_stack and  push to quadruples
		if ( post_expr.front().catergory.compare("Operator") == 0 ) {

			// int to str clear buffer
			ss.str("");
			ss.clear();
			ss <<  name_index++;

			// get first element
			Node tmp = pr_stack.top();
			pr_stack.pop();
		
			// is assign ( arg2 empty )  
			if( post_expr.front().symbol.compare("=") == 0 ){
				quadruples.push_back( Quadruples(post_expr.front().symbol, tmp.symbol, "",  "t" + ss.str() ) );
				quadruples.push_back( Quadruples(post_expr.front().symbol, "t" +  ss.str(), "",  pr_stack.top().symbol ) );
			
			}
			// is ALU operate
			else{
				quadruples.push_back( Quadruples(post_expr.front().symbol,  pr_stack.top().symbol, tmp.symbol,  "t" + ss.str() ) );
			}
			pr_stack.pop( );
			// push temperary name
			pr_stack.push(Node(0, "t" + ss.str(), "t" + ss.str(), " "));
		}
		else{
			// push identifiy, char, num
			pr_stack.push( post_expr.front() );
		}

		// next element
		post_expr.pop();
	}
}

void InterCodeGenerator:: outputQuadruples(){
	

	// output the InterCodeGenerator Quadruples result 
	ofstream outputfile("output/quadruples.txt", ios:: out);

	if(!outputfile){
		cout << "could not open outputfile set.txt" << endl;
		exit(1);
	}
	
	for(vector<Quadruples>::iterator itr = quadruples.begin(); itr != quadruples.end(); ++itr ){
		outputfile << left << setw(4) <<  itr-> getOp()  << setw(5) << itr-> getArg1() << " " \
				<< setw(5) << itr -> getArg2() << setw(5) <<  itr -> getResult()   << endl;
	}
}

// let other class can get quadruples
vector<Quadruples> InterCodeGenerator:: getQuadruples(){
	return quadruples;	
}		

string  InterCodeGenerator:: toString(int integer){


}
