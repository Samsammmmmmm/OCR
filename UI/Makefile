# Makefile

CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0 sdl2 SDL2_image` -Wall -O3
LDLIBS = `pkg-config --libs gtk+-3.0 sdl2 SDL2_image`

EXE = ui

all: $(EXE)

$(foreach f, $(EXE), $(eval $(f):))

.PHONY: clean

clean:
	${RM} $(EXE)
#END
