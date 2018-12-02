prob1.o : prob1.cpp
	g++ -g prob1.cpp -o prob1.o

prob3.o : prob3.c
	gcc -g prob3.c -o prob3.o

runProb3 : prob3.o
	> prob3.test
	./prob3.o prob3.test
	./fileexists.sh