compiler: compiler.o Lexier.o Token.o  Parser.o Nonterminal.o Node.o CodeGenerator.o
	g++ -g -o  compiler compiler.o Lexier.o Token.o Parser.o Nonterminal.o Node.o
compiler.o: compiler.cpp lib/Lexier.h lib/Parser.h
	g++ -c  compiler.cpp
Lexier.o: lib/Lexier.cpp lib/Lexier.h lib/Token.h 
	g++ -c lib/Lexier.cpp
Token.o: lib/Token.cpp lib/Token.h
	g++ -c lib/Token.cpp
Parser.o: lib/Parser.cpp lib/Parser.h lib/Nonterminal.h lib/Node.h
	g++ -c lib/Parser.cpp
Nonterminal.o: lib/Nonterminal.cpp lib/Nonterminal.h
	g++ -c lib/Nonterminal.cpp
Node.o: lib/Node.cpp lib/Node.h
	g++ -c lib/Node.cpp
CodeGenerator.o: lib/CodeGenerator.cpp lib/CodeGenerator.h
	g++ -c lib/CodeGenerator.cpp
clean:
	rm compiler *.o
