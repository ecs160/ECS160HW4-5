
output: maxTweeter.o
	gcc -Wall -Werror -o output maxTweeter.o

maxTweeter.o: maxTweeter.c
	gcc -Wall -Werror -c -o maxTweeter.o maxTweeter.c

clean: 
	rm -f output maxTweeter.o

