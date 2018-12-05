p1.x : prob1_2.cpp
	g++ -g -D PROB1 prob1_2.cpp -o p1.x

p2.x: prob1_2.cpp
	g++ -g prob1_2.cpp -o p2.x

p3.x : prob3.c
	gcc -g prob3.c -o p3.x

runProb3 : p3.x
	> prob3.test
	./p3.x prob3.test
	./fileexists.sh

clean:
	rm -f *.x *.o