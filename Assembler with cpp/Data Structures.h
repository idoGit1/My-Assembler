#ifndef ASSEMBLER_PROJECT_DATA_STRUCTURES_H
#define ASSEMBLER_PROJECT_DATA_STRUCTURES_H

#include <string>
#include <vector>

struct Macro 
{
    std::string name;
    int numberOfParameters;
    std::pair<int,int> range;
};

struct MacroList 
{
    std::vector<Macro> list;
    size_t size;
};

#endif // ASSEMBLER_PROJECT_DATA_STRUCTURES_H
