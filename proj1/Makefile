#SOPE proj1
all: main

main: main.o getters.o
	gcc -o main main.o getters.o

main.o: main.c
	gcc -o main.o main.c -c -Wall -Wextra -Werror

getters.o: getters.c
	gcc -o getters.o getters.c -c -Wall -Wextra -Werror

clean:
	rm -rf *.o *~ main
