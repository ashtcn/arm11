# ARM11 Project Style Guide and C Reference

When writing C code for this project, you should follow these guidelines in
order to ensure consistent formatting.

## Aims

The aims of this style guide are threefold:

- Ensure *consistent* styling. This allows for easy reading of the source
code and makes finding things easier.
- Allow for easier *documentation*. This style guide is written with the
documentation generation tool, Doxygen in mind. Following this style guide
will ensure documentation for the source code can be generated quickly.
- Require concise, well-thought out and correct code. Following these
guidelines for formatting and documentation makes it easier to spot mistakes
and layout code in a way which makes the most sense to readers.

This guide also discusses most of the C language required for the 120.3 course,
so can act as a quick reference for C.

Numbered points are enforceable guidelines that must be followed. These can
be checked by a script if required. Notes are other general points that
should be followed to improve code style.

## General Guidelines

### Line Breaks and Indentation

#### Line Breaks

1. Write no more than one statement per line.
2. Keep lines under 80 characters.
3. Use blank lines to break up your code:
  * After precompiler declarations,
  * After each function,
  * After variable declarations,
  * (Optional) In any other case where further seperation of code would aid
  readability.

#### Indentation

4. Use spaces instead of tabs. Indent 2 spaces at a time.
5. Indent each block of code (surrounded by braces) should be indented by 2
more spaces than the last.

### Commenting

6. Every file should be preceeded by a brief description (max 75 characters)
and most files should also have a detailed description.
7. Every top-level definition (e.g. global variables, functions) should be
preceeded by a brief description (max 75 characters) and most functions should
also have a detailed description.
  * The brief description should explain what the function does in general.
  * The detailed description should explain any edge cases, and should
  explain the choice of implementation if it is complex.

Note: standard single line comments should also be used in code to help
explain more complex functions.

Note: comments should be formatted using markdown.

Example: Comments before functions should be formatted as follows:
```
/**
 * @brief A brief description which ends with a period.
 *
 * (Optional) A detailed description which may span multiple lines.
 * @param p1 Description of first parameter.
 * ...
 * @see another_function()
 * @return What the function does to the parameters.
 */
returnType function_name(type1 p1, ...) {
```

### Language Features

This section acts as a quick reference, and gives examples which demonstrate
the style guidelines.

#### Basic Types

8. Don't use the `char`, `short`, `int` and `long` types.

In order to increase portability and readability, use `stdint.h`.

Included types are: `uint_8t`, `uint_16t`, `uint_32t`, `uint_64t`, `int_8t`,
`int_16t`, `int_32t`, `int_64t`.

#### Enumerated Types

Example:
```
/**
 * @brief An enum type that represents flags for rendering.
 *
 * Each bit represents a different flag. Use bitwise and to check if a flag is
 * set.
 */
enum render_flag {
  /** The ambient flag (bit 0). */
  AMBIENT = 1,
  /** The diffuse flag (bit 1). */
  DIFFUSE = 2,
  /** The specular flag (bit 2). */
  SPECULAR = 4
};
```

#### Functions

9. Use underscore\_seperation for function names.
10. Don't put a space between the function name and parameter list.
11. Never leve the parameter list for a function empty.
12. Leave a single space between the closing parenthesis of the parameter list
and the opening brace of the function body.

Note: Try to keep functions under 40 lines long.

Remember to include `void` in your parameter list for functions without
parameters.

#### Variables

Note: In general, you should initialise variables at point of definition.

Example: `double numbers[2][3] = {{1.0, 2.0, 3.0}, {2.0, 3.0, 4.0}}`.

A string can be represented as an array of type `char`, ending with `'\0'`.

Example: `char hello[] = "Hello!"`.

#### Flow of Control

13. Always use braces where it is possible to do so.
14. Leave a space and after the opening and closing parentheses for the
condition, respectively.

Note: You should usually break at the end of each case in a switch statement.

Examples:
```
if (condition) {
  statement;
  ...
} else if (condition) {
  ...
} else {
  ...
}
```
```
while (condition) {
  ...
}
```
```
for (initialiser; condition; increment / decrement) {
  ...
}
```
```
switch (expression) {
  case constant1:
    ...
    break;
  case constant2:
    ...
    break;
  default:
    assert(false);
}
```

#### Logical Expressions

15. Place constants before variables when using the equality operator.

Note: Use parentheses to clarify precedence in complex expressions.

Example: `3 == x`, NOT `x == 3`.

#### Input Output

Use `printf` to print to standard output. Sanitise using:
- `%c` for character.
- `%d` for signed integer.
- `%u` for unsigned integer.
- `%o` for octal.
- `%x` for hexadecimal.
- `%f` for floating point value.
- `%e` for floating point value using scientific notation.
- `%s` for string.
- `%p` for pointer value.

Use `scanf` to read from standard input. Beware of secuirity risks when
inputting, say, a string. You can use `fgets` instead.

Examples:
```
int i;
int ret = scanf("%i", &i);
assert(1 == ret);
```
```
char buffer[100];
int size = sizeof(buffer);
fgets(buffer, size, stdin);
```

#### Bitwise Operations

- `&` for bitwise AND.
- `|` for bitwise OR.
- `>>` for RIGHT SHIFT.
- `<<` for LEFT SHIFT.
- `~` for bitwise NOT.
- `^` for bitwise XOR.

#### Pointers

16. When declaring a pointer, place the `*` adjacent to the variable name, not
the type.
17. When passing by reference, pointers must be declared as `const`.

Example: `int *pointer`, NOT `int* pointer` or `int * pointer`.

Note: Use the `const` keyword wherever possible.

Examples:
```
int val = 5;
// Value cannot be modified
const int *ptr1 = &val;
// Pointer cannot be modified
int const *ptr2 = &val;
// Neither value nor pointer can be modified
const int const *ptr3 = &val;
```

Think of `*` as an operator that takes an address and returns the value which
it points to. `&` is an operator that takes a value and returns its address.

We can also use function pointers to pass functions be reference.

Example:
```
/**
 * @brief A function which sums its two inputs.
 */
int sum(int a, int b) {
  return a + b
}

/**
 * @brief A main function which does nothing.
 */
int main(void) {
  int (*sum_pointer)(int, int) = &sum;

  return EXIT_SUCCESS;
}
```

#### Command Line Arguments

`main` can have a type signature where it receives arguments from the command
line:

- `argc`, the number of passed parameters.
- `argv`, an array of strings.

The first argument is the name of the file!

Example:
```
/**
 * @brief A main function which prints its command line arguments.
 */
int main(int argc, char **argv) {
  for (int i = 0; i < argc; i++) {
    printf("argv[%i] = %s\n", i, argv[i]);
  }
  return EXIT_SUCCESS;
}
```

#### Memory Management

18. `main` should only return `EXIT_SUCCESS` or `EXIT_FAILURE`.

- `stlib.h`'s `void *malloc(size_t size)` allocates a region of memory of
`size` bytes and returns a pointer to the allocated memory.
- `void free(void *ptr)` frees a previously allocated memory region.
- You need to check that these don't fail (not NULL).
- `string.h`'s `void *memset (void *s, int c, size_t n)` sets teh `n`-byte
region starting at `s` to `c`.
- `void *memcpy(void *dest, const void *src, size_t n)` copies `n` bytes from
`src` to `dst`, returning `dst`.
- You can use the `memcheck` tool provided by `valgrind` to check for memory
leaks.

#### Assertions

`assert.h` provides `assert(logical_expression)`.

#### Headers

19. All headers should be surrounded by include guards, `#ifndef THIS_H`,
`#define THIS_H`.

Note: In general, every `.c` file should have an associated `.h` file.

#### Makefile

Example:
```
# Set the flags for the C compiler
CFLAGS = -Wall -pedantic -std=c99

# Build rule for final executable
sum: add.o

# Build rules for the .o files
sum.o: add.h
add.o: add.h

# Rule to clean generated files
clean: rm -f sum sum.o add.o

# Tell make that 'clean' is not a real file
.PHONY: clean
```

#### More Variables

- `static` (local): value is retained.
- `static` (top level): only seen within this file.
- `extern` variables: defined in header, can be used in multiple files.
- Global variables: try to avoid them.

#### File Input

- Use `FILE fopen(const char *path, const char *mode)`.
  * Modes are `r` to read from start, `w` to write from start, `a` to append.
  * Need to check file could be opened (not NULL).
- Use `int feof(FILE *stream)` to check if end of file has been reached (NULL).
- Use `int fclose(FILE *fp)` to close file.
  * Need to check it doesn't fail.

Reading a file:

- `int fscanf(FILE *stream, const char *format, ...)`.
- `char *fgets(char *s, int size, FILE *stream)`.

Writing to a file:

- `int fprintf(FILE *stream, const char *format, ...)`.
- `int fputs(const char *s, FILE *stream)`.

Binary data:

- Use the mode `b`.
- `size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)`
- `size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)`
- `ptr` is the address to read or write, `nmemb` is the number of elements,
`size` is the number of bytes for each element.
- Need to check returned value is the number of elements.

Error checking:

You can also use `int ferror(FILE *stream)`.

#### Structs

Example:
```
/**
 * @brief A struct that hold the co-ordinates of a 2D vertex.
 */
typedef struct {
  /** The x co-ordinate */
  float x;
  /** The y co-ordinate */
  float y;
} vertex;
```

- Use `.` to refer to a struct component.
- Use `->` to refer to a struct component from a pointer to a struct.

Use `typedef` to declare an alias for an existing type.

Use a `union` where requirements to store components are mutually exclusive.

#### Constants

Three approaches:

1. Use a marco using `#define`.
2. Define a global `static const` variable (not a true constant).
3. Use an unnamed `enum`.

#### Strings

Use `string.h` to get:

- `size_t strlen(const char *s)`.
- `char *strncat(char *dest, const char *src, size_t n)`.
- `int strncmp(const char *s1, const char *s2, size_t n)`.
- `char *strncpy(char *dest, const char *src, size_t n)`.
