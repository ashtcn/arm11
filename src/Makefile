CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

# GCC sometimes complains about -D_BSD_SOURCE being deprecated
# CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic -O3

.SUFFIXES: .c .o

.PHONY: all clean

all: emulate assemble unit_tests

emulate: emulate.o decode.o execute.o print_compliant.o print.o toolbox.o
assemble: assemble.o assemble_toolbox.o 
unit_tests: unit_tests.o decode.o execute.o print_compliant.o print.o toolbox.o

# emulate
emulate.o: decode.h execute.h print_compliant.h
decode.o: decode.h instruction.h toolbox.h
execute.o: execute.h toolbox.h
print_compliant.o: print_compliant.h print.h
print.o: print.h toolbox.h
toolbox.o: toolbox.h global.h system_state.h value_carry.h

# assemble
assemble.o: global.h assemble_toolbox.h

# unit_tests
unit_tests.o: decode.h execute.h print_compliant.h

clean:
	rm -f $(wildcard *.o)
	rm -f $(wildcard *.gch)
	rm -f emulate
	rm -f assemble
	rm -f unit_tests
