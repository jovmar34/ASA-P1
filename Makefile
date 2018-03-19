
proj: proj_quick.o
	gcc -g -Wall -pedantic -o proj proj_quick.o

proj_quick.o: proj_quick.c
	gcc -g -Wall -pedantic -c proj_quick.c

run:
	mkdir Meus-Outputs;
	sh testes.txt

diff:
	sh diff.txt

clean:
	rm -f *.o proj;
	rm -r Meus-Outputs
