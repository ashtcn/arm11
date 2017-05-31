CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

# GCC sometimes complains about -D_BSD_SOURCE being deprecated
# CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: emulate assemble unit_tests

emulate: emulate.o decode.o execute.o toolbox.o
assemble: assemble.o
unit_tests: unit_tests.o toolbox.o

# emulate
emulate.o: decode.h execute.h
decode.o: decode.h instruction.h toolbox.h
execute.o: execute.h
toolbox.o: toolbox.h global.h system_state.h value_carry.h

# assemble
assemble.o:

# unit_tests
unit_tests.o: decode.h execute.h

clean:
	rm -f $(wildcard *.o)
	rm -f $(wildcard *.gch)
	rm -f emulate
	rm -f assemble
	rm -f unit_tests
