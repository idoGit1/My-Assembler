#ifndef STDIO_H_
#define STDIO_H_
#include <stdio.h>
#endif
#ifndef STDLIB_H_
#define STDLIB_H_
#include <stdlib.h>
#endif
#include "pre assembler.h"
int main(int argc, char* argv[])
{
    strcat(argv[1], ".asm");
    preAssembler(argv[1])   ;  // Spreading macros.
    return 0;
}
