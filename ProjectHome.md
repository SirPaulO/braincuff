Relies on the C preprocessor to generate brainfuck programs based on C macros (#define).

  * Create a .bc and #include it in bf.c
  * In the .bc, include bclib.c and create a "#define MAIN ..." with BF code that may use the "functions" defined in bclib.bc
  * Compile

Then

  * Run with '-c' to get the BF source code

Or

  * Get your brainfuck program from the intermediate, preprocessed source file (visual studio generates it in a .i file)

Or

  * Run with '-d' to debug