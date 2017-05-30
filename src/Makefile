CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o
emulate: emulate.o toolbox.o

emulate.o: global.h system_state.h instruction.h toolbox.h
toolbox.o: toolbox.h global.h system_state.h

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
