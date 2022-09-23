all:
	gcc -Wall -Wextra -c *.c
	gcc *.o -o main -lSDL2 -lSDL2_image

clean:
	rm -rf *[.do] main *~
