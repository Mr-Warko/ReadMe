## Simple SDL mini code
 
CC=gcc
 
CPPFLAGS= `pkg-config --cflags sdl` `pkg-config gtk+-2.0 --cflags`

CFLAGS= -Wall -Wextra  -std=c99 -O2
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image `pkg-config gtk+-2.0 --libs`

 
SRC= 
OBJ= ${SRC:.c=.o}
 
all: main
 
main: ${OBJ}
 
clean:
	rm -f *~ *.o
	rm -f main
 
# END
