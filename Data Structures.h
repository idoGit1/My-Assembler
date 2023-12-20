
#ifndef ASSEMBLER_PROJECT_DATA_STRUCTURES_H
#define ASSEMBLER_PROJECT_DATA_STRUCTURES_H

typedef enum {false ,true} Bool;

struct Pair
{
    int first;
    int second;
};
typedef struct Pair Pair;


struct Macro
{
    char* name;
    int number_of_parameters;
    Pair range;
};
typedef struct Macro Macro;

struct MacroList
{
    Macro* list;
    int size;
};
typedef struct MacroList MacroList;


#endif //ASSEMBLER_PROJECT_DATA_STRUCTURES_H
