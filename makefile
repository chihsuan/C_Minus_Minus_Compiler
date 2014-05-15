compiler: compiler.o Lexier.o Token.o  Parser.o Nonterminal.o Node.o Semantic.o InterCodeGenerator.o Semantic.o Symbol.o
	g++ -g -o  compiler compiler.o Lexier.o Token.o Parser.o Nonterminal.o Node.o Semantic.o InterCodeGenerator.o Symbol.o
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
InterCodeGenerator.o: lib/InterCodeGenerator.cpp lib/InterCodeGenerator.h
	g++ -c lib/InterCodeGenerator.cpp
Semantic.o: lib/Semantic.cpp lib/Semantic.h lib/Node.h
	g++ -c lib/Semantic.cpp
Symbol.o: lib/Symbol.cpp lib/Symbol.h
	g++ -c lib/Symbol.cpp
clean:
	rm compiler *.o
