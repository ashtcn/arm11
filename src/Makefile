CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: emulate assemble unit_tests

emulate: emulate.o toolbox.o
assemble: assemble.o
unit_tests: unit_tests.o toolbox.o

# emulate
emulate.o: instruction.h toolbox.h
toolbox.o: toolbox.h global.h system_state.h value_carry.h

# assemble
assemble.o:

# unit_tests
unit_tests.o: toolbox.h

clean:
	rm -f $(wildcard *.o)
	rm -f $(wildcard *.gch)
	rm -f emulate
	rm -f assemble
	rm -f unit_tests
