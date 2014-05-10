compiler: compiler.o Lexier.o Token.o
	g++ -g -o compiler compiler.o Lexier.o Token.o
compiler.o: compiler.cpp lib/Lexier.h 
	g++ -c compiler.cpp
Lexier.o: lib/Lexier.cpp lib/Lexier.h lib/Token.h 
	g++ -c lib/Lexier.cpp
Token.o: lib/Token.cpp lib/Token.h
	g++ -c lib/Token.cpp
clean:
	rm program *.o
