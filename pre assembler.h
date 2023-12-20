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


void preAssembler()
{

    int numOfLines = 0; // The number of lines in the source file
    char** sourceFile; // The source file (.asm file)
    FILE* srcPtr; // File pointer to open the source file.
    FILE* destPtr;

    Pair macroRange;
    MacroList macrosList;
    int sizeOfList = 0;
    Bool flag = false;

    srcPtr = fopen("D:/Assembler project/sourceFile.asm", "r");
    destPtr = fopen("D:/Assembler project/destFile.asm", "w");

    inputFile(srcPtr, sourceFile, &numOfLines);

    for (int i = 0; i < numOfLines; i++)
    {
        char checkMacroDef[7];
        char checkEndMacroDef[10];

        strncpy(checkMacroDef, sourceFile[i], 6);
        strncpy(checkEndMacroDef, sourceFile[i], 9);

        checkMacroDef[6] = '\0';
        checkEndMacroDef[9] = '\0';

        if (strcmp(checkMacroDef, START_MACRO) == 0)
        {
            //flag = true;
            macroRange.first = i;
        }

        else if (strcmp(checkEndMacroDef, END_MACRO) == 0)
        {
            // Needs to search for the number of params and add to map.
            int j = 6;
            while (isDigit(*(sourceFile[macroRange.first] + j)) == false)
                j++;
            int params = *(sourceFile[macroRange.first] + j) - '0';
            // VERY IMPORTANT: end every string with a \0.
            macroRange.second = i;
            Macro newDef;
            newDef.numberOfParameters = params;
            newDef.range.first = macroRange.first + 1;
            newDef.range.second = macroRange.second - 1;

            char* name = (char*) malloc(sizeof(char) * 20);

            strcpy(name, findName(sourceFile[macroRange.first]));
            newDef.name = name;

            append(&macrosList, newDef);
        }
        else
        {
            char* suspectedMacro = findMacroName(sourceFile[i]);
            int macroCalledIndex = locateMacroByName(macrosList, suspectedMacro);
            if (macroCalledIndex != -1)
            {
                // Calling a macro
                //char** registers = (char**) malloc(macrosList.list[i].numberOfParameters * sizeof(char*));
                char registers[macrosList.list[macroCalledIndex].numberOfParameters][2];
                for (int j = 0; j < macrosList.list[macroCalledIndex].numberOfParameters; j++)
                {
                    static int k = 0; // Maybe needs to be fixed.
                    while (j==0 && sourceFile[i][k] != ' ') k++;
                    k++;
                    registers[j][0] = sourceFile[i][k];
                    registers[j][1] = sourceFile[i][k++];
                    k += 3;
                }
                int cntParameters = 1;

                char arg[2];
                arg[0] = '%';
                for (; cntParameters <= macrosList.list[macroCalledIndex].numberOfParameters;
                       cntParameters++)
                {
                    for (int j = macrosList.list[macroCalledIndex].range.first; j <
                                                                                macrosList.list[macroCalledIndex].range.second; j++)
                    {
                        char num = (char) (cntParameters + '0');
                        char* ptr = strstr(sourceFile[j], strcat(arg, &num));
                        if (ptr != NULL)
                        {
                            *ptr = registers[cntParameters - 1][0];
                            ++ptr;
                            *ptr = registers[cntParameters - 1][0];
                        }

                    }
                }
            }
            else
                continue;
        }
        fprintf(destPtr, "%s", sourceFile[i]);

    }



    free(macrosList.list); // Need to write free all memory function.
}


#endif //ASSEMBLER_PROJECT_PRE_ASSEMBLER_H