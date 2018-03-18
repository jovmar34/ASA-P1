
proj: proj.c
	gcc -g -Wall -pedantic -o proj proj.c

run:
	mkdir Meus-Outputs;
	sh testes.txt

diff:
	sh diff.txt

clean:
	rm -f *.o proj;
	rm -r Meus-Outputs
