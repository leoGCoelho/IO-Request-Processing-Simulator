CC=gcc

CFLAGS=-Wall -Wextra -Werror -std=c11 -O0 -g

all: grade

sim.o: sim.c

test:
	$(CC) $(CFLAGS) -o test sim.c test.c

grade: test
	./test

run_aluno:
	$(CC) $(CFLAGS) -o aluno sim.c aluno.c

aluno: run_aluno
	./aluno

clean:
	rm -rf *.o sim
