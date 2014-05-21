#include <iostream>
#include <string>
#include "InterCodeGenerator.h"
#include <sstream>
#include <fstream>
using namespace std;


InterCodeGenerator:: InterCodeGenerator()
	:name_index(0)
{

}
InterCodeGenerator:: ~InterCodeGenerator(){

}

void InterCodeGenerator:: startGenerate(multimap<int, Node> parsingTree){

	bool in_Expr = false;
	int Expr_index = -1;
	queue<Node> tmp;
	queue<Node> postfix_return;

	/* ParsingTree ->  produce three address code by postfix */
	for(multimap<int, Node>::iterator itr = parsingTree.begin(); itr != parsingTree.end(); ++ itr){
	
		string const & token =  itr -> second.token;
		string symbol =  itr -> second.symbol;
		string catergory = itr -> second.catergory;
		
		if( !in_Expr && token.compare("Expr") == 0 ){
			in_Expr = true;
			Expr_index = itr -> second.index;
		}
		else if( itr -> second.index == Expr_index ){
			in_Expr = false;
			Expr_index = -1;
			postfix_return = postfix(tmp);
			
			while( !tmp.empty() ){
				tmp.pop();
			}
			createQuadruples(postfix_return);
		
		}

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
        
		if ( expr.front().catergory.compare("Operator") == 0 ) {
			while (!operator_stack.empty() && priority( operator_stack.top() ) \
							<= priority( expr.front() )) {
                result.push( operator_stack.top() ); 
                operator_stack.pop();
            }
            operator_stack.push( expr.front() );
        } 
        else if ( expr.front().symbol.compare("(") == 0 ) {

            operator_stack.push( expr.front() );
        } 
        else if ( expr.front().symbol.compare(")") == 0 ) {

            while ( operator_stack.top().symbol.compare("(") != 0 ) {
				result.push( operator_stack.top() );
				operator_stack.pop();
			}
			operator_stack.pop();
		}
		else{
			result.push( expr.front() );
		}
			expr.pop();
	}
	while(!operator_stack.empty()){
		result.push( operator_stack.top() );
		operator_stack.pop();
	}

	return result;
}
int InterCodeGenerator:: priority(Node node){

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
		if ( post_expr.front().catergory.compare("Operator") == 0 ) {
			ss.str("");
			ss.clear();
			Node tmp = pr_stack.top();
			pr_stack.pop();
			ss <<  name_index++;
			
			if( post_expr.front().symbol.compare("=") == 0 ){
				quadruples.push_back( Quadruples(post_expr.front().symbol, tmp.symbol, "",  "t" + ss.str() ) );
				quadruples.push_back( Quadruples(post_expr.front().symbol, "t" +  ss.str(), "",  pr_stack.top().symbol ) );
			}
			else{
				quadruples.push_back( Quadruples(post_expr.front().symbol,  pr_stack.top().symbol, tmp.symbol,  "t" + ss.str() ) );
			}
			pr_stack.pop( );
			pr_stack.push(Node(0, "t" + ss.str(), "t" + ss.str(), " "));
		}
		else{
			pr_stack.push( post_expr.front() );
		}

		post_expr.pop();
	}
}

void InterCodeGenerator:: outputQuadruples(){
	
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


vector<Quadruples> InterCodeGenerator:: getQuadruples(){
	return quadruples;	
}
