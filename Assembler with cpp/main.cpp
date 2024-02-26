#include <iostream>
#include "pre assembler.h"
int main(int argc, char* argv[])
{
    std::string fileName = argv[1];
    fileName += ".asm";

    preAssembler(fileName);
    return 0;
}