# ARM11 project

## Source Code

See the `src` directory.

- `emulate.c` contains the main functionality for the emulator.
- `assemble.c` contains the main functionality for the assembler.
- `emulate_utils` contains helper functions for emulate.
- `assemble_utils` contains helper functions for assemble.

You can make emulate using `make emulate`, and assemble using `make assemble`.

## Tests

See the `src` directory.

- `unit_tests.c` contains the unit tests for emulator and assembler.
- `./run_tests` (`make full_tests`) runs all tests, including unit tests, memory tests and the test suite. This script can take some time to complete.
- `./run_quick_tests` (runs automatically by calling `make`) runs a subset of tests, including unit tests, basic memory tests, and the test suite.

See `test_suite` for our extended ruby test suite.

## Documentation

See the `doc` directory. Use `make` to generate the pdf files.

- `Checkpoint.pdf` is the checkpoint report.
- `Report.pdf` is the final report.
- `LaTeX Documentation/refman.pdf` is the documentation for emulate and assemble.
- `Style Guide.md` is our style guide, used throughout the project.

## GPIO

See `programs` for the Part III GPIO program. This can be assembled using `assemble` from `src`. It has been emulated and tested on the Raspberry Pi and works as intended.

## Extension: OpenCV Game Engine

See the submodule `open-cv-game-engine`. It has a seperate README that describes use. LaTeX documentation is available in the `LaTeX Documentation` directory.

To build the extension, simple use the script `./setup_engine`.
