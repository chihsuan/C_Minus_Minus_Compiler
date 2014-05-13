#include <iostream>
#include <string>
#include <cstring>
#include "Lexier.h"
#include <iomanip>
#include "Token.h"

/* define the DFA move state */
#define isOperators1 10
#define isOperators2 11
#define isOperators3 12
#define isOperators4 13
#define isComment 14
#define isChar 15
#define isSpecialChar 16
#define waitQuote 17
#define isSymbols 18
#define isNumber 19
#define isIdentifier 102
#define isKeyword 103
#define accept 104
#define notAccept 105

using namespace std;


/*constructor initial input_path*/
Lexier:: Lexier(string input_path)
	:input_path(input_path),
	 keyword_move(1)
{
}

Lexier:: ~Lexier(){
	for( vector<Token*>::iterator itr = token_list.begin(); itr != token_list.end(); ++itr){
		delete  *(itr);
	}
	token_list.clear();
}

bool Lexier:: startParseTokens(){
	
	int line_number = 1;
	string tokenCategory = " ";	
	ifstream& input = getInput();
	ofstream& output = getOfStream();


	/* loop find each token in each line for DFA check */	
	for (string line; getline(input, line); ){
		// output the line number
		output << right <<  "Line" <<  setw(4) << line_number << " :" << endl;
		
		// for string splite replace ' ' to '\s' ensure ' ' can be accept
		int char_space_pos = 0;
		while(  (char_space_pos = line.find('\'', char_space_pos)) != string::npos  ){
			if(line[char_space_pos+1] == ' ' && line[char_space_pos+2] == '\'' ){
				line.erase(char_space_pos+1, 1);
				line.insert(char_space_pos+1, "\\s");
				cout << line << endl;
			}
			char_space_pos += 1;
		}

		// remove last ' '
		if( line[line.length()-1] == 13 ){
			line = line.substr(0, line.length()-1);
		}
		// convert string to char* for split
		char* tokenPtr = new char[line.length() + 1];
		strcpy(tokenPtr, line.c_str() );
		
		// splite by tab and white space
		tokenPtr = strtok(tokenPtr, " \t");
		
		// whike has next token
		while( tokenPtr != NULL ){
				string token = tokenPtr;

				//enter DFA
				if(!enter_DFA(tokenPtr, tokenCategory)){
					cout << "Error! Line " << left << setw(3) << line_number \
							<< "token:" << "\""  << token << "\"" << endl;
				}
			
				//is comment (no nedd to print)
				if( tokenCategory.compare("<Comment>") == 0 ){
					tokenPtr = strtok(NULL, " \t");
					continue;
				}
		
				// reduce '\s' to ' '
				if (token.compare("\'\\s\'") == 0){
					token = "\' \'";
				}
				// output the lexier result
				output << left <<  "\t"     <<  setw(13) << tokenCategory << ": " << tokenPtr << endl; 
			
				//save result
				token_list.push_back( new Token(token, tokenCategory, line_number) );
			
				// get next token
				tokenPtr = strtok(NULL, " \t");
		}
		delete [] tokenPtr; 
		line_number += 1;
	}
	input.close();
	output.close();
}


vector<Token*> Lexier:: getTokenList(){
	return token_list;
}


bool Lexier:: enter_DFA(char* token, string& tokenCategory){

	//starting state
	int state = 0;
	/* implement a DFA */
	for( int i = 0; i < strlen(token); ++i ){
		move(state, token[i], tokenCategory);	
	}
	keyword_move = 1;

	// check state is accept or not accept
	if( state == notAccept || state == isChar || state == waitQuote){
		tokenCategory = "<Error>";
		return false;
	}
	return true;
}

void Lexier:: startState(int& state, char next_char, string& tokenCategory){

		switch(next_char){
			
			//operators type1
			case '+': case '-': case '*': case '/':
				state = isOperators1;
				tokenCategory = "<Operator>";
				break;
			//operators type2
			case '=': case '!': case '>': case '<':
				state = isOperators2;
				tokenCategory = "<Operator>";
				break;
			//operators type3
			case '&':
				state = isOperators3;
				tokenCategory = "<Operator>";
				break;
			//operators type4
			case '|':
				state = isOperators4;
				tokenCategory = "<Operator>";
				break;
			//char state
			case '\'':
				state = isChar;
				tokenCategory = "<Char>";
				break;
			//special symbols
			case '[': case ']': case '(': case ')': case '{': case '}': case ';': case ',':
				state = isSymbols;
				tokenCategory = "<Special>";
				break;
			case '_':
				state = isIdentifier;
				tokenCategory = "<identifier>";
				break;
			default:
				// Number state
				if ( isdigit(next_char) ){
					state = isNumber;
					tokenCategory = "<Number>";
				}// identifier
				else if( isalpha(next_char) ){
					state = isIdentifier;
					tokenCategory = "<identifier>";
				}//
				else{
					state = notAccept;
				}
		}
}


void Lexier:: move(int& state, char next_char, string& tokenCategory){

		const string keyword[]  = {" ","if", "int", "char", "return", "else", "while", "break"};

		switch(state){
			case 0:
				startState(state, next_char, tokenCategory);
				switch(next_char){
					case 'i':
						state = 1;
						break;
					case 'c':
						state = 3;
						break;
					case 'r':
						state = 4;
						break;
					case 'e':
						state = 5;
						break;
					case 'w':
						state = 6;
						break;
					case 'b':
						state = 7;
						break;
				}
				break;
			case 1: case 2: case 3:  case 4: case 5: case 6: case 7:
				if(keyword_move == 1 && state == 1 && next_char == 'n'){
					state = 2;
				}
				if( keyword_move >= keyword[state].length() || next_char != keyword[state][keyword_move] ){
					state = isIdentifier;			
				}
				if ( keyword_move == keyword[state].length()-1 && next_char == keyword[state][keyword_move] ){
					state = isKeyword;
					tokenCategory = "<Keyword>";
				}
				keyword_move += 1;
				break;
			// '+' , '-' , '*', '/'
			case isOperators1:
				//comment state
				if (next_char == '/'){
					state = isComment;
					tokenCategory = "<Comment>";
				}
				else{
					state = notAccept;
				}
				break;
			// '=', '!',  '<', '>'
			case isOperators2:
				state = next_char == '=' ? accept : notAccept;
				break;
			// '&'
			case isOperators3:
				state = next_char == '&' ? accept : notAccept;
				break;
			// '|'
			case isOperators4:
				state = next_char == '|' ? accept : notAccept;
				break;

			// '/'
			case isComment:
				state = next_char == '/' ? accept : notAccept;
				break;

			// first '\''
			case isChar:
				if ( next_char == '\\'  ){
					state = isSpecialChar;
				}
				else{
					state = waitQuote;
				}
				break;
			
			// '\t' '\n' ' '
			case isSpecialChar:
				state = next_char == 't' || next_char == 'n' || next_char == 's'? waitQuote : notAccept;
				break;

			// second '\''
			case waitQuote:
				state = next_char == '\'' ? accept : notAccept;
				break;
		
			// '[', ']', '{', '}', '(', ')', ';', ','
			case isSymbols:
				state = notAccept;
				break;
		
			//0-9
			case isNumber:
				state = isdigit(next_char) ? isNumber : notAccept;
				break;
			
			//[a-z A-z '_'][a-z A-Z 0-9 _]
			case isIdentifier:
				if(next_char == '_' || isdigit(next_char)\
								|| isalpha(next_char) ){
					state = isIdentifier;
				}
				else{
					state = notAccept;
				}
				break;
			case accept:
				state = notAccept;
				break;
			case notAccept:
				break;
		}
}


ifstream& Lexier:: getInput(){
	
	/* input souce code and reutrn ifstream object */
	static ifstream inputFile( input_path.c_str() ,ios::in);
	if ( !inputFile ){
		cerr << "File could not be opened" << endl;
		exit(1);
	}

	return inputFile;
}

ofstream& Lexier:: getOfStream(){

	/* return the output stream for write the result */
    static ofstream outputFile( "output/token.txt", ios::out);

	if (!outputFile){
		cerr << "File could not be opened" << endl;
		exit(1);
	}
	return outputFile;
}
