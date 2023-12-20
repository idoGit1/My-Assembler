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

char* find_name(char* line) // Finding name of the macro while declaring it
{
    int i = 7;
    int cnt = 1;
    while (line[i] != ' ')
    {
        i++;
        cnt++;
    }
    char* name = malloc(cnt * sizeof(char));
    for (int j = 0; j < cnt; j++)
    {
        name[j] = line[7 + j];
    }
    name[cnt - 1] = '\0';
    return name;
}

void input_file(FILE* fptr, char** source_file, int* num_of_lines)
{
    char tmp_line[200];
    int num = 0;
    while (fgets(tmp_line, 200, fptr)) // Counting the number of lines
        // in the source file.
        num++;
    // Sizing the matrix to be in size [number of lines]x200 (chars per line)
    source_file = malloc(num * sizeof(char*));
    for (int i = 0; i < num; ++i)
        *(source_file + i) = malloc(200 * sizeof(char));


    fptr = fopen("D:/Assembler project/sourceFile.asm", "r");
    for (int i = 0; i < num; i++)
        fgets(source_file[i], 200, fptr);
    // Input file is done. The source file is located in source_file.
    *num_of_lines = num;
}


void append(MacroList* list, Macro new_element)
{
    int size = list->size;

    list->list = (char*) realloc(list->list, (size + 1) * sizeof(char));

    list->list[size].name = new_element.name;
    list->list[size].number_of_parameters = new_element.number_of_parameters;
    list->list[size].range = new_element.range;
    list->size++;
}

char* find_macro_name(char* line) // Finding name of the macro while calling it
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

int locate_macro_by_name(MacroList list, char* name)
{
    for (int i = 0; i < list.size; i++)
    {
        if (strcmp(list.list[i].name, name) == 0)
        return i;
    }
    return -1;
}

#endif //ASSEMBLER_PROJECT_FUNCTIONS_H
