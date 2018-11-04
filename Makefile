main: main.o mem.o
	gcc -o main main.o mem.o

main.o: main.c
	gcc -O -Wall -Werror -c main.c

mem.o: mem.c
	gcc -c -fpic mem.c -Wall -Werror

create-shared:
	gcc -shared -o libmem.so mem.o

clean:
	rm -f main.o mem.o libmem.so
