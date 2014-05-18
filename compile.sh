#if test -e "compiler";
#then
#	make clean
#fi

if make; 
then
	./compiler input/main.c input/grammar.txt
fi
