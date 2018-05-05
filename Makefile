all:
	gcc -g -Wall -Werror main.c -o main -std=gnu99
	gcc -g -Wall -Werror mem.c -o mem  -std=gnu99
clean:
	rm -rf main mem