all: build

build: 
	gcc -g -Wall -o tema2 functiiLG.c tema2.c

clean:
	rm -rf tema2