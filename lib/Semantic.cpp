#include <iostream>
#include "Semantic.h"

using namespace std;

Semantic:: Semantic(){
	
}


// build Semantic rule and analysis
void Semantic:: analysis(multimap<int, Node> parsingTree){


	//build Semantic rule

	// build symbol table check the decl vaild
	buildSymbolTable(parsingTree);
}

void Semantic:: buildSymbolTable(multimap<int, Node> parsingTree){

	string type = "";
	int order = 0;
	int scope = 0;
	int scopeNumber = 0;
	bool isdecl = false;
	bool isFuncDecl = false;
	stack<string> stream_stack;

	//iterator parsingTree to check what scope is and if decl vaild
	for(multimap<int, Node>::iterator itr = parsingTree.begin(); itr != parsingTree.end(); ++ itr){
		
		string const & token =  itr -> second.token;
		string symbol =  itr -> second.symbol;
		
		if( token.compare("id") == 0 && isdecl ){
			isdecl = false;	
			if( !tableInsert(stream_stack, symbol) ){
				cout << "Error variable has already declared" << endl; 
				exit(1);
			}
			if( isFuncDecl ){
				scope = scopeNumber + 1;
			}
			symbolTable.insert( make_pair(order, Symbol(symbol, token, type, scope)) );
			stream_stack.push( symbol );
			
		}
		else if( itr -> second.catergory.compare("Identifier") == 0 ){
			if( !findSymbol(stream_stack, symbol) ){
				cout << "Error variable use before declare" << endl; 
				exit(1);
			}
		}
		else if( token.compare("Type") == 0 ){
			isdecl = true;
			type = symbol;
		}
		else if( token.compare("{") == 0 ){
			isFuncDecl = false;
			scope = ++scopeNumber;
			stream_stack.push( symbol );
		}
		else if( token.compare("}") == 0 ){
			
			while( stream_stack.top().compare("{") != 0 ){
				stream_stack.pop();
				scope--;
			}
			stream_stack.pop();
		
			if(  !findSymbol(stream_stack, "{") ){
				scope = 0;
			}
		} 
		else if( token.compare("FunDecl") == 0){
			isFuncDecl = true;
		}
	}
	outputSymbolTable();
}

bool Semantic:: findSymbol(stack<string> input, string symbol){
	

	while( !input.empty() ){
		if( input.top().compare(symbol) == 0 ){
			return true;
		}
		input.pop();
	}
	return false;
}

bool Semantic:: tableInsert(stack<string> input, string symbol ){

	while( !input.empty() && input.top().compare("{") != 0   ){
		
		if(input.top().compare(symbol) == 0){
			return false;
		}
		input.pop();	
	}
	return true;
}

void Semantic:: outputSymbolTable(){

	ofstream outputfile("output/symbol.txt", ios:: out);

	if(!outputfile){
		cout << "could not open outputfile set.txt" << endl;
		exit(1);
	}
	
	for(multimap<int, Symbol>::iterator itr = symbolTable.begin(); itr != symbolTable.end(); ++ itr){
		outputfile << left << setw(10) << itr -> second.getSymbol() << setw(3) << itr -> second.getToken() \
				<< setw(5) <<  itr-> second.getType() << setw(3) << itr -> second.getScope() << endl;	
	}
	

	outputfile.close();
}
