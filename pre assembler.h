#include <stdio.h>
#include <string.h>
#include "functions.h"
#ifndef DS_H_
#define DS_H_
#include "Data Structures.h"
#endif
#ifndef ASSEMBLER_PROJECT_PRE_ASSEMBLER_H
#define ASSEMBLER_PROJECT_PRE_ASSEMBLER_H
#define START_MACRO "%macro"
#define END_MACRO "%endmacro"


void pre_assembler()
{

    int num_of_lines = 0; // The number of lines in the source file
    char** source_file; // The source file (.asm file)
    FILE* src_ptr; // File pointer to open the source file.
    FILE* dest_ptr;

    Pair macro_range;
    MacroList macros_list;
    int size_of_list = 0;
    Bool flag = false;

    src_ptr = fopen("D:/Assembler project/sourceFile.asm", "r");
    dest_ptr = fopen("D:/Assembler project/destFile.asm", "w");

    input_file(src_ptr, source_file, &num_of_lines);

    for (int i = 0; i < num_of_lines; i++)
    {
        char checkMacroDef[7];
        char checkEndMacroDef[10];

        strncpy(checkMacroDef, source_file[i], 6);
        strncpy(checkEndMacroDef, source_file[i], 9);

        checkMacroDef[6] = '\0';
        checkEndMacroDef[9] = '\0';

        if (strcmp(checkMacroDef, START_MACRO) == 0)
        {
            //flag = true;
            macro_range.first = i;
        }

        else if (strcmp(checkEndMacroDef, END_MACRO) == 0)
        {
            // Needs to search for the number of params and add to map.
            int j = 6;
            while (isDigit(*(source_file[macro_range.first] + j)) == false)
                j++;
            int params = *(source_file[macro_range.first] + j) - '0';
            // VERY IMPORTANT: end every string with a \0.
            macro_range.second = i;
            Macro newDef;
            newDef.number_of_parameters = params;
            newDef.range.first = macro_range.first + 1;
            newDef.range.second = macro_range.second - 1;

            char* name = (char*) malloc(sizeof(char) * 20);

            strcpy(name, find_name(source_file[macro_range.first]));
            newDef.name = name;

            append(&macros_list, newDef);
        }
        else
        {
            char* suspected_macro = find_macro_name(source_file[i]);
            int macro_called_index = locate_macro_by_name(macros_list, suspected_macro);
            if (macro_called_index != -1)
            {
                // Calling a macro
                //char** registers = (char**) malloc(macros_list.list[i].number_of_parameters * sizeof(char*));
                char registers[macros_list.list[macro_called_index].number_of_parameters][2];
                for (int j = 0; j < macros_list.list[macro_called_index].number_of_parameters; j++)
                {
                    static int k = 0; // Maybe needs to be fixed.
                    while (j==0 && source_file[i][k] != ' ') k++;
                    k++;
                    registers[j][0] = source_file[i][k];
                    registers[j][1] = source_file[i][k++];
                    k += 3;
                }
                int cnt_parameters = 1;

                char arg[2];
                arg[0] = '%';
                for (; cnt_parameters <= macros_list.list[macro_called_index].number_of_parameters;
                cnt_parameters++)
                {
                    for (int j = macros_list.list[macro_called_index].range.first; j <
                                                                                   macros_list.list[macro_called_index].range.second; j++)
                    {
                        char num = (char) (cnt_parameters + '0');
                        char* ptr = strstr(source_file[j], strcat(arg, &num));
                        if (ptr != NULL)
                        {
                            *ptr = registers[j][0];
                            ++ptr;
                            *ptr = registers[j][0];
                        }

                    }
                }
            }
            else
                continue;
        }
        fprintf(dest_ptr, "%s", source_file[i]);

    }


    
    free(macros_list.list); // Need to write free all memory function.
}


#endif //ASSEMBLER_PROJECT_PRE_ASSEMBLER_H
