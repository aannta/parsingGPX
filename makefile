CC=gcc
CFLAGS= -std=c99 -Wall -pedantic -g3

all: ParseGPX

ParseGPX: ParseGPX.c
	${CC} ${CFLAGS} -o ParseGPX ParseGPX.c

clean:
	rm -r *.o ParseGPX