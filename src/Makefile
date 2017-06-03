CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic -O3

.SUFFIXES: .c .o

.PHONY: all clean

all: emulate assemble unit_tests

emulate: emulate.o emulate_utils/decode.o emulate_utils/execute.o emulate_utils/print_compliant.o emulate_utils/print.o toolbox.o
assemble: assemble.o assemble_utils/assemble_toolbox.o assemble_utils/string_array_array.o assemble_utils/symbol_table.o assemble_utils/assembler.o assemble_utils/encode.o toolbox.o assemble_utils/word_array.o emulate_utils/print.o
unit_tests: unit_tests.o emulate_utils/decode.o emulate_utils/execute.o emulate_utils/print_compliant.o emulate_utils/print.o toolbox.o

# emulate
emulate.o: emulate_utils/decode.h emulate_utils/execute.h emulate_utils/print_compliant.h
emulate_utils/decode.o: emulate_utils/decode.h instruction.h toolbox.h
emulate_utils/execute.o: emulate_utils/execute.h toolbox.h
emulate_utils/print_compliant.o: emulate_utils/print_compliant.h emulate_utils/print.h
emulate_utils/print.o: emulate_utils/print.h toolbox.h
toolbox.o: toolbox.h global.h emulate_utils/system_state.h emulate_utils/value_carry.h

# assemble
assemble.o: global.h assemble_utils/assemble_toolbox.h assemble_utils/word_array.h
assemble_utils/assembler.o: instruction.h assemble_utils/string_array.h assemble_utils/symbol_table.h assemble_utils/encode.h assemble_utils/word_array.h global.h toolbox.h emulate_utils/print.h
assemble_utils/word_array.o: global.h

# unit_tests
unit_tests.o: emulate_utils/decode.h emulate_utils/execute.h emulate_utils/print_compliant.h

clean:
	rm -rf $(wildcard *.o)
	rm -rf $(wildcard *.gch)
	rm -f emulate
	rm -f assemble
	rm -f unit_tests
