#include "InterCodeGenerator.h"
using namespace std;


InterCodeGenerator:: InterCodeGenerator()
	:t_index(0) // temperary name index for quadruples
{

}
InterCodeGenerator:: ~InterCodeGenerator(){

}

void InterCodeGenerator:: startGenerate(multimap<int, Node> parsingTree){


	bool in_Expr = false;  // is in Expr block
	bool unaryOp = false;
	int Expr_index = -1;   // record the Expr index
	string go_to;
	in_while = false;
	queue<Node> tmp;      // tmp stack push expr symbol    
	queue<Node> postfix_expr;

	/* ParsingTree ->  produce three address code by postfix */
	for(multimap<int, Node>::iterator itr = parsingTree.begin(); itr != parsingTree.end(); ++ itr){
		
		string token =  itr -> second.token;
		string symbol =  itr -> second.symbol;
		string catergory = itr -> second.catergory;
		int index = itr -> second.index;

		// Start ALU expression
		if( !in_Expr && token.compare("Expr") == 0 ){
			// set flag and record index
			in_Expr = true;                     
			Expr_index = index;
		}
		// End ALU expression
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
		}
		// in exprssion block push all Identifier operator Number char
		else if( ( catergory.compare("Identifier") == 0 || catergory.compare("Operator")  == 0 \
					|| catergory.compare("Number")  == 0 || catergory.compare("Char") == 0)   && in_Expr ){
		
			// char to int
			if( catergory.compare("Char") == 0){
				char ch = itr -> second.symbol[1];
				itr -> second.symbol = 	toString( int(ch) );
			}

			Node push_item = (itr) -> second;
			
			// unaryOP
			++itr;
			if( unaryOp ){
				tmp.push(Node(0, "0", "0", "Number"));
				unaryOp = false;
			}
			
			// array 
			if ( (itr++) -> second.symbol.compare("[") == 0 ){
				queue<Node> array_queue;
				array_queue.push( Node(0, "0", "0", "Number") );
				array_queue.push( Node(0, "+", "+", "Operator") );
				while( (++itr) -> second.symbol.compare("]") != 0 ){
					if ( islower( itr-> second.token[0] ) || itr-> second.catergory.compare("Operator") == 0 ){
						array_queue.push( itr-> second );		
					}
				}
				postfix_expr = postfix(array_queue);
				createQuadruples(postfix_expr);
				quadruples.push_back( Quadruples("[]=", "t" + toString(t_index-1), "",  push_item.symbol)  );
				tmp.push( push_item );
			}
			else{
				tmp.push( push_item );
				--itr;
			}
		}		
		else if( whileStmt(index, token, symbol) ){
		}
		// if statement
		else if( ifStmt(index, token, symbol) ){
		}
		else if ( token.compare("UnaryOp") == 0){
			unaryOp = true;
		}
	}

	outputQuadruples();
}


bool InterCodeGenerator:: ifStmt( int index, string token, string symbol ){

	string go_to;
	
	// start if
	if( token.compare("if") == 0 ){
		// pos where if should be insert
		if_jmp_stack.push( quadruples.size() + 1 );
		if_tmp_stack.push( "t" + toString( t_index ) );
	}
	// end if and start else
	else if( token.compare("else") == 0 ){
		
		// jmp to where
		go_to = toString( quadruples.size() + 2 + if_jmp_stack.size() + while_ret_stack.size() + break_jmp_stack.size() );
		
		// insert to if condition, if false jmp to else
		quadruples.insert( quadruples.begin() + if_jmp_stack.top() , Quadruples("jf", go_to, if_tmp_stack.top(), "" )  );	
		if_jmp_stack.pop();
		if_tmp_stack.pop();

		// pos where else should be insert
		else_jmp_stack.push( quadruples.size() );
		
		// record pos else end
		else_index.push( index - 1 );
	}
	// end else
	else if( !else_index.empty() && index == else_index.top() ){
		
		// insert to end if jmp out of else
		go_to = toString( quadruples.size() + 2 + if_jmp_stack.size() + while_ret_stack.size() + break_jmp_stack.size() );
		quadruples.insert( quadruples.begin() + else_jmp_stack.top() , Quadruples("jp", go_to, "", "" )  );
		else_jmp_stack.pop();
		else_index.pop();
	}
	else{
		return false;
	}

	return true;
}

bool InterCodeGenerator:: whileStmt(int index, string token, string symbol){


	string go_to;

	// start while statement
	if( token.compare("while") == 0 ){
		
		// stack where while loop sholud return 
		while_ret_stack.push( quadruples.size() + 1 );
		while_index_stack.push( index - 1 );
		while_tmp_stack.push( "t" + toString(t_index) );
	}	
	// end of while statement
	else if( !while_index_stack.empty() &&  index == while_index_stack.top() ){
		
		// while condition if false
		go_to = toString( quadruples.size()  + 2 + if_tmp_stack.size() + while_tmp_stack.size() + break_jmp_stack.size() );
		quadruples.insert( quadruples.begin() +  while_ret_stack.top() , Quadruples("jf",  go_to, while_tmp_stack.top(), "" )  );

		// insert break stmt out of while
		while ( !break_jmp_stack.empty() ){

			quadruples.insert( quadruples.begin() +  break_jmp_stack.top() , Quadruples("jp",  go_to, "", "" )  );
			break_jmp_stack.pop();
		}

		// loop return 
		go_to = toString( while_ret_stack.top() + while_tmp_stack.size() + if_tmp_stack.size() -1 );
		quadruples.push_back( Quadruples("jp", go_to, "", "" )  );
	
		while_ret_stack.pop();
		while_tmp_stack.pop();
		while_index_stack.pop();
	}
	// break statement;
	else if( token.compare("break") == 0 ){
			break_jmp_stack.push( quadruples.size() + 1 + if_jmp_stack.size() );
	}
	else{
		return false;
	}
	return true;
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
			case '[': case ']':
					return 2;
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
			ss <<  t_index++;

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
	stringstream ss;
	ss << integer;
	return ss.str();
}
