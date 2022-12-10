"""rota:
	gcc -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`   -c -o rota.o rota.c
	gcc   rota.o  `pkg-config --libs sdl2 SDL2_image` -o rota"""
	"""gcc -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` test.c block_op.h
	gcc   main.o  `pkg-config --libs sdl2 SDL2_image` -o block"""

hough:
	gcc -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`   -c -o seg.o seg.c
	gcc   seg.o  `pkg-config --libs sdl2 SDL2_image` -o hough

block:
	gcc -c -Wall -Wextra test.c		
	gcc -c -Wall -Wextra block_op.c
	gcc -Wall -Wextra `pkg-config --cflags sdl2 SDL2_image` -o block test.o block_op.o

all:
	gcc -Wall -Wextra -c *.c
	gcc *.o -o main -lSDL -lSDL2_image

clean:
	rm -rf *[.do] main hough *~ *.bmp ../BMP/*.bmp ../BMP/*.jpg
