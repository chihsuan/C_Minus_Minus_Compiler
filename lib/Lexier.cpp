#include "Lexier.h"

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
#define accept 104
#define notAccept 105

/*Lexier constructor 
 * */
Lexier:: Lexier(string input_path)
	:input_path(input_path)
{
}

/* Lexier descontuctor 
 * delete dynamic memory
 * */
Lexier:: ~Lexier(){
	for( vector<Token*>::iterator itr = token_list.begin(); itr != token_list.end(); ++itr){
		delete  *(itr);
	}
	token_list.clear();
}

/* Lexier main program parse input symbol
 *
 * */
bool Lexier:: startParseTokens(){
	
	int line_number = 1;
	bool parseResult = true;
	string tokenCategory = " ";	
	string token;	
	// input & ouput stream
	ifstream& input = getInput();
	ofstream& output = getOfstream();

	/* loop find each token in each line for DFA check */	
	for (string line; getline(input, line); ){
		
		// output the line number
		output << right <<  "Line" <<  setw(4) << line_number << " :" << endl;
		
		// special case: for string splite replace ' ' to '\s' ensure ' ' can be accept
		int char_space_pos = 0;
		while(  (char_space_pos = line.find('\'', char_space_pos)) != string::npos  ){
			if(line[char_space_pos+1] == ' ' && line[char_space_pos+2] == '\'' ){
				line.erase(char_space_pos+1, 1);
				line.insert(char_space_pos+1, "\\s");
			}
			char_space_pos += 2;
			if ( char_space_pos >= line.length() ){
				break;
			}
		}

		// convert input string to char* for split
		char* tokenPtr = new char[line.length() + 1];
		char* token_list_ptr = tokenPtr;
		strcpy(tokenPtr, line.c_str() );
		
		// splite by tab and white space
		tokenPtr = strtok(tokenPtr, " \t");
		
		// whike has next token
		while( tokenPtr != NULL ){
				
				token = tokenPtr;
				//enter DFA  if error
				if( !enter_DFA(token, tokenCategory )){
					cout << "Error! Line " << left << setw(3) << line_number \
							<< "token:" << "\""  << token << "\"" << endl;
					parseResult = false;
				}
				
				//is comment (no nedd to print)
				if( tokenCategory.compare("Comment") == 0 ){
					break;
				}
				// reduce '\s' to ' '
				if (token.compare("\'\\s\'") == 0){
					token = "\' \'";
				}
				// output the lexier result
				output << left <<  "\t"     << setw(16) << "<" + tokenCategory + ">" << ":"  << token << endl; 
			
				//save result
				token_list.push_back( new Token(token, tokenCategory, line_number) );
			
				// get next token
				tokenPtr = strtok(NULL, " \t");
		}
		delete [] token_list_ptr; 
		line_number += 1;
	}
	
	input.close();
	output.close();
	return parseResult;
}

bool Lexier:: enter_DFA(string token, string& tokenCategory){
	
	const int keyword_size = 8;
	const static string keyword[keyword_size]  = {" ","if", "int", "char", "return", "else", "while", "break"};
	
	for(int i = 0; i < keyword_size; ++i){
		if ( token.compare(keyword[i]) == 0){
			tokenCategory = "<Kerword>";
			return true;
		}
	}
	//starting state
	int state = 0;
	/* implement a DFA */
	for( int i = 0; i < token.length(); ++i ){
		move(state, token[i], tokenCategory);	
	}
	// check state is accept or not accept
	if( state == notAccept || state == isChar || state == waitQuote){
		tokenCategory = "Error";
		return false;
	}
	return true;
}

/*
 *  initial state
 * */
void Lexier:: startState(int& state, char next_char, string& tokenCategory){

		switch(next_char){
			
			//operators type1
			case '+': case '-': case '*': case '/':
				state = isOperators1;
				tokenCategory = "Operator";
				break;
			//operators type2
			case '=': case '!': case '>': case '<':
				state = isOperators2;
				tokenCategory = "Operator";
				break;
			//operators type3
			case '&':
				state = isOperators3;
				tokenCategory = "Operator";
				break;
			//operators type4
			case '|':
				state = isOperators4;
				tokenCategory = "Operator";
				break;
			//char state
			case '\'':
				state = isChar;
				tokenCategory = "Char";
				break;
			//special symbols
			case '[': case ']': case '(': case ')': case '{': case '}': case ';': case ',':
				state = isSymbols;
				tokenCategory = "Special";
				break;
			case '_':
				tokenCategory = "Identifier";
				break;
			default:
				// Number state
				if ( isdigit(next_char) ){
					state = isNumber;
					tokenCategory = "Number";
				}// identifier
				else if( isalpha(next_char) ){
					state = isIdentifier;
					tokenCategory = "Identifier";
				}//
				else{
					state = notAccept;
				}
		}
}

/*
 * DFA state move
 * */
void Lexier:: move(int& state, char next_char, string& tokenCategory){

		switch(state){
			// start state
			case 0:
				startState(state, next_char, tokenCategory);
				break;	
			// '+' , '-' , '*', '/'
			case isOperators1:
				//comment state
				if (next_char == '/'){
					state = isComment;
					tokenCategory = "Comment";
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
			
			// accept
			case accept:
				state = next_char == ' ' ? accept : notAccept;
				break;

			// notAccept
			case notAccept:
				break;
		}
}

/* open file and return input stream
 * */
ifstream& Lexier:: getInput(){
	
	/* input souce code and reutrn ifstream object */
	static ifstream inputFile( input_path.c_str() ,ios::in);
	if ( !inputFile ){
		cerr << "File could not be opened" << endl;
		exit(1);
	}

	return inputFile;
}

/* open file and return output stream
 * */
ofstream& Lexier:: getOfstream(){

	/* return the output stream for write the result */
    static ofstream outputFile( "output/token.txt", ios::out);
	if (!outputFile){
		cerr << "File could not be opened" << endl;
		exit(1);
	}
	return outputFile;
}

/* for other class get parse result 
 * */
vector<Token*> Lexier:: getTokenList(){
	return token_list;
}
