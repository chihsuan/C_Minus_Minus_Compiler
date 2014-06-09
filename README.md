C_Minus_Minus_Compiler
===============
2014 Compiler Project
Goal: Implementing a C_Minus_Minus compiler 
Environment: Ubuntu 12.04.4 LTS
Language: C++
Main program: compiler.cpp
Input file: main.c grammar.txt
Output file: token.txt set.txt LLtable.txt tree.txt symbol.txt quadruples.txt code.tm
*Compile & Run: "./compile.sh" or "make" "./compiler input/main.c input/grammar.txt"
---------------
C_Minus_Minus Token:
	Kerwords: int char return if else while break
	Operators: = ! + - * / == != < > <= >= && ||
	Special Symbols: [ ] ( ) { } ; ,
	Identifier: [a-zA-Z_][a-zA-Z0-9_]*
	Number: [0-9]+
	Char: ‘[.|\\n|\\t| ]’ Ex. ‘a’、’\n’、’ ’
	Comment(no need to print) //
program Process:
	1. Lexical Analysis
	2. Syntax Analysis
	3. Semantic Analysis
	4. IR Generation
	5. Code Generation
---------------
Directory & file description:
	lib: all custom class file
	input: include main.c & grammar.txt
	ouput: all output file could found in this directoy
	compile.sh: shell program for quick compile and run program
	Tiny_Machine.c: machine source code for run result code.tm
*note: compiler support char to int implicit conversion (no need to type checking)
