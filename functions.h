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

char* findName(const char* line) // Finding name of the macro while declaring it
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

void inputFile(FILE* fptr, char*** sourceFile, int* numOfLines, char* fileName)
{
    char tmpLine[200];
    int num = 0;
    while (fgets(tmpLine, 200, fptr)) // Counting the number of lines
        // in the source file.
        num++;
    // Sizing the matrix to be in size [number of lines]x200 (chars per line)
    *sourceFile = (char**) malloc(num * sizeof(char*));
    for (int i = 0; i < num; ++i)
        *(*sourceFile + i) = (char*) malloc(200 * sizeof(char));


    fptr = fopen(fileName, "r");
    for (int i = 0; i < num; i++)
        fgets((*sourceFile)[i], 200, fptr);
    // Input file is done. The source file is located in sourceFile.
    *numOfLines = num;
}


void append(MacroList* list, Macro newElement)
{
    int size = list->size;

    list->list = (Macro*) realloc(list->list, (size + 1) * sizeof(Macro));
    if (list->list == NULL)
    {
        printf("Error, program will terminate");
        exit(1);
    }
    list->list[size].name = newElement.name;
    list->list[size].numberOfParameters = newElement.numberOfParameters;
    list->list[size].range = newElement.range;
    list->size++;
}

void findMacroName(const char* line, char** suspectedMacro) // Finding name of the macro while calling it
{
    int i = 0;
    char name[20];
    while (line[i] != ' ' && line[i] != '\0')
    {
        name[i] = line[i];
        i++;
    }
    name[i] = '\0';
    *suspectedMacro = (char*) malloc(i * sizeof(char));
    strncpy(*suspectedMacro, name, i + 1);
}

int locateMacroByName(MacroList list, char* name) // Finding the index of the named
// macro in the list of macros.
{
    for (int i = 0; i < list.size; i++)
    {
        if (strcmp(list.list[i].name, name) == 0)
            return i;
    }
    return -1;
}

void freeAllMemory(MacroList* list)
{
    for (int i = 0; i < list->size; i++)
    {
        free(list->list[i].name);
    }
    free(list->list);
}

char* findLabelNameBackwords(char* line, char* ptrToEndOfLabel)
{
    char* labelName = (char*) malloc(200 * sizeof(char));
    int lengthOfName = 1;
    char* ptrToBeginOfLabel = ptrToEndOfLabel - 1;
    while (line != ptrToBeginOfLabel)
    {
        lengthOfName++;
        --ptrToBeginOfLabel;
    }
    // Now ptrToEndOfLabel points to the beginning of the label, so 
    // changing the name:
    strncpy(labelName, ptrToBeginOfLabel, lengthOfName);
    labelName[lengthOfName] = '\0';
    return labelName;
}


#endif //ASSEMBLER_PROJECT_FUNCTIONS_H