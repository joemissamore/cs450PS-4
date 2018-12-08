SUBMISSION = prob1_2.cpp prob3.c Makefile README.md groupmembers.txt # Gabe add your pdf to this line

p1.x : prob1_2.cpp
	g++ -g -D PROB1 prob1_2.cpp -o p1.x

p2.x: prob1_2.cpp
	g++ -g prob1_2.cpp -o p2.x

p3.x : prob3.c
	gcc -g prob3.c -o p3.x

clean:
	rm -f *.x *.o *.out 

submission:
	tar cvf submission.tar ${SUBMISSION}