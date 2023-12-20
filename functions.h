#include <stdlib.h>
#include <string.h>

#ifndef DS_H_
#define DS_H_
#include "Data Structures.h"
#endif
#ifndef ASSEMBLER_PROJECT_FUNCTIONS_H
#define ASSEMBLER_PROJECT_FUNCTIONS_H

Bool isDigit(char c)
{
    return (c >= '0') && (c <= '9') ? true : false;
}

char* findName(char* line) // Finding name of the macro while declaring it
{
    int i = 7;
    int cnt = 1;
    while (line[i] != ' ')
    {
        i++;
        cnt++;
    }
    char* name = (char*) malloc(cnt * sizeof(char));
    for (int j = 0; j < cnt; j++)
    {
        name[j] = line[7 + j];
    }
    name[cnt - 1] = '\0';
    return name;
}

void inputFile(FILE* fptr, char** sourceFile, int* numOfLines)
{
    char tmpLine[200];
    int num = 0;
    while (fgets(tmpLine, 200, fptr)) // Counting the number of lines
        // in the source file.
        num++;
    // Sizing the matrix to be in size [number of lines]x200 (chars per line)
    sourceFile = malloc(num * sizeof(char*));
    for (int i = 0; i < num; ++i)
        *(sourceFile + i) = malloc(200 * sizeof(char));


    fptr = fopen("D:/Assembler project/sourceFile.asm", "r");
    for (int i = 0; i < num; i++)
        fgets(sourceFile[i], 200, fptr);
    // Input file is done. The source file is located in sourceFile.
    *numOfLines = num;
}


void append(MacroList* list, Macro newElement)
{
    int size = list->size;

    list->list = (char*) realloc(list->list, (size + 1) * sizeof(char));

    list->list[size].name = newElement.name;
    list->list[size].numberOfParameters = newElement.numberOfParameters;
    list->list[size].range = newElement.range;
    list->size++;
}

char* findMacroName(char* line) // Finding name of the macro while calling it
{
    int i = 0;
    char name[20];
    while (line[i] != ' ')
    {
        name[i] = line[i];
        i++;
    }
    name[i] = '\0';
    return name;
}

int locateMacroByName(MacroList list, char* name)
{
    for (int i = 0; i < list.size; i++)
    {
        if (strcmp(list.list[i].name, name) == 0)
            return i;
    }
    return -1;
}

#endif //ASSEMBLER_PROJECT_FUNCTIONS_H