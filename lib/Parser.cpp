#include "Parser.h"

/* constructor
 * initial garmmar input path
 * */
Parser:: Parser(char* input_path){
	inputGrammar(input_path);
}

/* destructor
 * */
Parser:: ~Parser(){
	vector<Nonterminal*>:: iterator itr;
	for(itr = non_terminals.begin(); itr != non_terminals.end(); ++itr ){
		delete *itr;
	}
}

/* load grammar file and save as map <string, vector<list<string> > > 
 * */
void Parser:: inputGrammar(char* input_path){

	/* inputGrammar save as map<string, vector<list<string> >> */
	Nonterminal* head;
	string head_symbol;
	string terminal;
	vector<list<string> > body;

	// open file
	ifstream input_file(input_path, ios::in);
	if( !input_file ){
		cout << "Grammar.txt could not be opend" << endl;
		exit(1);
	}

	// read input line by line
	for ( string line; getline(input_file, line); ){
	
		//spilit the '\n'
		line.substr(0, line.length()-1);

		// is Nonterminal
		if ( isalpha(line[0]) ){
			if( !body.empty() ){
				grammar.insert(make_pair(head_symbol, body));
			}
			
			head_symbol = line;
			head = new Nonterminal( head_symbol );
			
			// non_terminals vector
			non_terminals.push_back(head);
			// non_terminals map for search
			non_terminals_map.insert(make_pair(line, head));
			body.clear();

		}// is terminal
		else{	
			list<string> right_hand_side;

			// convert string to char* for split
			char* tokenPtr = new char[line.length() + 1];
			strcpy(tokenPtr, line.c_str() );
				
			// splite by white space
			tokenPtr = strtok(tokenPtr, " \t");
				
			// whike has next token
			while( tokenPtr != NULL ){
				terminal = tokenPtr;
				right_hand_side.push_back(terminal);
				// get next token
				tokenPtr = strtok(NULL, " \t");
			}
			delete [] tokenPtr;
			body.push_back(right_hand_side);	
		}
	}

	// insert last production to produce grammar
	grammar.insert(make_pair(head_symbol, body));
}

/* Parser parsing 
 * create first follow nullable and table then parsing token
 * finally output parsing tree
 * */
void Parser:: startParsing(vector<Token*> tokens){
	
	vector<Nonterminal*>::iterator itr;
	vector<list<string> > starting_Production;
	list<string> body;

	//find nullable
	for( itr = non_terminals.begin(); itr!= non_terminals.end(); ++itr ){
		(*itr) -> nullable = nullable( (*itr) -> term );
	}

	// add a production: S -> Program $
	Nonterminal* startState = new Nonterminal("S");
	startState -> nullable = non_terminals[0] -> nullable;
	non_terminals.push_back( startState );
	body.push_back( non_terminals[0]-> term );
	body.push_back( "$" );
	starting_Production.push_back(body);
	grammar.insert( make_pair("S", starting_Production) );

	//find first
	for( itr = non_terminals.begin(); itr!= non_terminals.end(); ++itr ){
		if ( (*itr) -> first.empty() ){
			(*itr) -> first = first( (*itr) -> term );		
		}
	}

	set<string>:: iterator itr2;
	
	//find follow
	follow();

	//output set.txt include nullable first and follow
	outputSet();
	
	// build Parsing table by if nullalbe is exist
	for(itr = non_terminals.begin(); itr != non_terminals.end(); ++itr){
		if( (*itr)->nullable ){
			for(set<string>::iterator follow_itr = (*itr) -> follow.begin(); \
							follow_itr != (*itr) -> follow.end(); ++follow_itr ){
				if( (*itr)->first.find( (*follow_itr) ) == (*itr)->first.end() ){
					map<string, map<string, int> >::iterator non_terminal_find_itr = parsingTable.find( (*itr)-> term );
					if ( non_terminal_find_itr == parsingTable.end() ){
						//parsingTable.insert( make_pair((*itr),   ) );	
					}
					else{
						non_terminal_find_itr -> second.insert( make_pair(*follow_itr, (*itr)-> production_nullable ) );
					}
				}
			}
		}
	}
	
	// output table
	outputParingTable();
	
	// create Parsing Tree and ouput it
	createParsingTree(tokens);
}

/* check is terminal or non_terminal
 * */
bool Parser:: isTerminal(string term){
	return islower(term[0]) || !isalpha(term[0]) ;
}

/* build nullable
 * */
bool Parser:: nullable(string term){

	map<string, vector<list<string> > >::iterator itr = grammar.find(term);
	int production_null_index = 0; // record which production produce nullable	
	bool isNullable = true;

	// iterator each prduction
	for( vector<list<string> >::iterator itr2 = (itr->second).begin(); itr2 != (itr-> second).end(); ++itr2 ){

		// iterator body term
		for( list<string>::iterator itr3 = (*itr2).begin(); itr3 != (*itr2).end(); ++itr3 ){
		
			// is nullable
			if( (*itr3).compare("epsilon") == 0 ){
				break;
			}
			// is a terminal or is a nullable non_terminal
			else if( isTerminal( (*itr3) )  || !nullable(*itr3) ){
				isNullable = false;
				break;
			}
		}

		if( isNullable ){
			// readcord which production produce nullable
			map<string, Nonterminal*>::iterator non_terminal = non_terminals_map.find(term);
			non_terminal->second->production_nullable = production_null_index;
			return true;
		}
		
		// reset and go to next production
		isNullable = true;
		production_null_index += 1;
	}
	return false;
}

/* build first
 * */
set<string> Parser:: first(string non_terminal){

	set<string> first_set;
	map<string, vector<list<string> > >::iterator itr = grammar.find(non_terminal);
	map<string, int> terminal_production; // for parsing table
	int productionIndex = -1;

	//iterator production
	for(vector<list<string> >::iterator itr2 = (itr->second).begin(); itr2 != (itr-> second).end(); ++itr2 ){
		productionIndex += 1;

		// epsilon skip
		if( (*itr2).front().compare("epsilon") == 0 ){
			continue;
		}
		// insert terminal and continue 
		if ( isTerminal( (*itr2).front() )  ){
			first_set.insert( (*itr2).front() );
			terminal_production.insert( make_pair( (*itr2).front(), productionIndex ) );			
			continue;
		}
		
		// front element is non_terminal, iterator body
		for(list<string>::iterator itr3 = (*itr2).begin(); itr3 != (*itr2).end(); ++itr3){
			
			// insert terminal
			if( isTerminal(*itr3) ){
				first_set.insert( *itr3 );
				terminal_production.insert( make_pair( (*itr3), productionIndex ) );			
			}
			else{
				// non_terminal
				map<string, Nonterminal*>::iterator bodyElement =  non_terminals_map.find(*itr3);
				
				// not Nullable then insert first of that nonterminal			
				bodyElement -> second -> first = first(*itr3);
				for(set<string>::iterator itr4 = bodyElement-> second -> first.begin(); \
								itr4 != bodyElement -> second -> first.end(); ++itr4){
					first_set.insert(*itr4);
					terminal_production.insert( make_pair( (*itr4), productionIndex) );			
				}
				if( (bodyElement->second)->nullable ){
					continue;
				}
			}
			break;
		}
	}

	// insert to parsing table 
	parsingTable.insert( make_pair(non_terminal, terminal_production) );
	return first_set;
}

void Parser:: follow(){

	map<string, vector<list<string> > >:: iterator itr;	
	bool backNull = false;
	set<string> follow_set;
					
	non_terminals[0] -> follow.insert("$");
	// iterator each Nonterminal production
	for(itr = grammar.begin(); itr != grammar.end(); ++itr){
		//iterator each production in that Nonterminal
		follow_set.clear();
		for(vector<list<string> >::iterator itr2 = (itr->second).begin(); itr2 != (itr-> second).end(); ++itr2 ){
			//iterator body by reverse
			for(list<string>::reverse_iterator itr3 = (*itr2).rbegin(); itr3 != (*itr2).rend(); ++itr3){
						
				if( (*itr3).compare("epsilon") == 0 ){
						break;
				}//terminal
				else if( isTerminal(*itr3) ){
					follow_set.clear();
					follow_set.insert( *itr3 );
				}//nonterminal
				else{
					map<string, Nonterminal*>::iterator non_terminal = non_terminals_map.find(*itr3);
						
					if( !follow_set.empty()  ){
						non_terminal -> second -> insertFollow(follow_set);
						follow_set.clear();
					}
					follow_set = non_terminal->second-> first;
				}
			}		
		}
	}
	// iterator each Nonterminal production
	for(vector<Nonterminal*>::iterator non_itr = non_terminals.begin();  \
					non_itr != non_terminals.end(); ++non_itr){
		itr = grammar.find( (*non_itr)->term);
		//iterator each production in that Nonterminal
		backNull = false;
		for(vector<list<string> >::iterator itr2 = (itr->second).begin(); itr2 != (itr-> second).end(); ++itr2 ){
			//iterator body by reverse
			for(list<string>::reverse_iterator itr3 = (*itr2).rbegin(); itr3 != (*itr2).rend(); ++itr3){
				if( (*itr3).compare("epsilon") == 0 ){
					break;
				}
				else if( isTerminal(*itr3) ){
					backNull = false;
				}
				else{
					map<string, Nonterminal*>::iterator non_terminal =  non_terminals_map.find(*itr3);
					// next element is nullable or is last one 
					if( itr3 == (*itr2).rbegin() || backNull ){
						non_terminal -> second -> insertFollow( (*non_itr)->follow );	
					}
					if( non_terminal -> second -> nullable ){
						backNull = true;
					}
				}				
			}
		}
	}
}
/* create parsing tree

 * */
void Parser:: createParsingTree(vector<Token*> tokens){

	stack<string> stateStack;	
	stack<Token*> inputStack;
	stateStack.push("S");
	int treeIndex = 0;
	string treeValue;
	int order = 0;
	int find_return;

	// push input to a stack 
	inputStack.push( new Token("$", "SP", 0) );
	for(vector<Token*>::reverse_iterator itr = tokens.rbegin(); itr != tokens.rend(); ++itr ){
		inputStack.push( (*itr) );
	}

	// loop while state stack  not empty
	while( !stateStack.empty() ){

		// preprocessing
		if( stateStack.top().compare("@") == 0 ){
			--treeIndex;
			stateStack.pop();
			continue;
		}
		// skip epsilon
		if( stateStack.top().compare("epsilon") == 0){
			stateStack.pop();
			continue;
		}
		
		treeValue = stateStack.top();
	
		// terminal insert to tree
		if( isTerminal( stateStack.top() ) ){
			parsingTree.insert( make_pair( order++ ,Node(treeIndex, treeValue, inputStack.top() -> getSymbol(), inputStack.top() -> getCatergory() ) ) );
		}
		// non_terminal insert to tree
		else{
			parsingTree.insert( make_pair( order++ ,Node(treeIndex, treeValue, inputStack.top() -> getSymbol(), " " ) ) );
		}

		// stack Pop and Push
		if( isTerminal(stateStack.top())){
			stateStack.pop();
			inputStack.pop();
		}
		// not found in parsin table
		else if( (find_return = findTable(stateStack.top(),  (*inputStack.top()) ) ) == -1  ){
			cout << "Line: " << inputStack.top() -> getLineNumber() << " Parsing Error!" << endl;
			exit(1);
		}
		else{
			// reduce
			map<string, vector<list<string> > > :: iterator itr =  grammar.find( stateStack.top() );
			stateStack.pop();	
		
			treeIndex += 1;
			// for corret tree index
			stateStack.push("@");
			for(list<string>:: reverse_iterator itr2 = itr->second[find_return].rbegin(); \
							itr2 != itr->second[find_return].rend(); ++itr2 ){
				stateStack.push( (*itr2) );
			}
		}
	}
	outputParingTree();
}

/* output file: set.txt
 * 
 * */
void Parser:: outputSet(){
	
	vector<Nonterminal*>::iterator itr;
	ofstream& outputfile = *openFile("set.txt");

	outputfile << "Nullable" << endl;
	for( itr = non_terminals.begin(); itr!= non_terminals.end(); ++itr ){
		outputfile << left << setw(24) << (*itr) -> term << ": "; 
		outputfile << ((*itr) -> nullable ? "true" : "false" ) << endl;
	}
		
	outputfile << "First" << endl;
	for( itr = non_terminals.begin(); itr!= non_terminals.end(); ++itr ){
		itrSetOutput( (*itr) -> term, (*itr) -> first, outputfile);
	}

	outputfile << "Follow" << endl;
	for( itr = non_terminals.begin(); itr!= non_terminals.end(); ++itr ){
		itrSetOutput( (*itr) -> term, (*itr) -> follow, outputfile);
	}
	outputfile.close();
}

/* a function for same structue output
 * */
void Parser:: itrSetOutput(string symbol, set<string> output_set, ofstream& outputfile){
	
	outputfile << left << setw(24) << symbol << ": "; 
	for(set<string>:: iterator itr2 = output_set.begin(); itr2 != output_set.end(); ++itr2){
		outputfile << *itr2 << " ";
	}
	outputfile << endl;
}

/* output file :: LLtable.txt
 *
 * */
void Parser:: outputParingTable(){
	
	ofstream& outputfile = *openFile("LLtable.txt");
	
	if(!outputfile){
		cout << "could not open outputfile set.txt" << endl;
		exit(1);
	}

	outputfile << "S" << endl;

	map<string, vector<list<string> > >:: iterator grammar_itr;
	for ( map<string, map<string, int> >::iterator itr3 = parsingTable.begin(); itr3 != parsingTable.end(); ++itr3){
		for(map<string, int>::iterator itr4 = itr3->second.begin(); itr4 != itr3->second.end(); ++itr4 ){
			grammar_itr = grammar.find( itr3->first );
			vector<list<string> > body = grammar_itr-> second; 
			int& index =  itr4 -> second;
			outputfile << left << setw(16) << itr3->first << setw(16) << itr4 -> first;
			for( list<string>::iterator body_itr = body[index].begin(); \
							  body_itr != body[index].end(); ++body_itr){
				outputfile << *body_itr << " ";
			}
			outputfile << endl;
		}
	}
	outputfile.close();
}

/* ouput file:: tree.txt
 * */
void Parser:: outputParingTree(){
	
	ofstream& outputfile = *openFile("tree.txt");
	
	for(multimap<int, Node>::iterator itr = parsingTree.begin(); itr != parsingTree.end(); ++ itr){
		
		string value =  itr -> second.token;
		if( value.compare("num") == 0 || value.compare("ch") == 0 || value.compare("id") == 0 ){
			value = itr -> second.symbol;
		}
		outputfile << right << setw( itr-> second.index ) << itr -> second.index << " " << value << endl;	
	}
	outputfile.close();
}

/*
 * find token in table
 * */
int Parser:: findTable(string& non_terminal, Token& token){

	map<string, map<string, int> >::iterator itr = parsingTable.find(non_terminal);
	map<string, int>::iterator itr2;
	
	if( token.getCatergory().compare("Number") == 0){
		itr2 =  itr -> second.find( "num" );
	}
	else if( token.getCatergory().compare("Identifier") == 0 ){
		itr2 =  itr -> second.find( "id" );
	}
	else if( token.getCatergory().compare("Char") == 0){
		itr2 =  itr -> second.find( "ch" );
	}
	else{
		itr2 =  itr -> second.find( token.getSymbol() );
	}

	if( itr2 == itr -> second.end() ){
		return -1;	
	}
	
	return itr2 -> second;

}


/*
 * for other class or program get parsing tree
 * */
multimap<int, Node> Parser:: getParsingTree(){
	return parsingTree;
}
/* 
 * new output file
 * */
ofstream* Parser:: openFile(string file_name){
	
	static ofstream* outputfile;
	outputfile = new ofstream(  ("output/" + file_name).c_str(), ios:: out);

	if(!outputfile){
		cout << "could not open outputfile set.txt" << endl;
		exit(1);
	}
	return outputfile;
}
