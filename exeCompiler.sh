if test -e "compiler";
then
	make clean
fi

if make; 
then
	./compiler main.c
fi
