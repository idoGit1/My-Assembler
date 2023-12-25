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

MacroList macrosList;

void preAssembler(char* fileName)
{

    int numOfLines = 0; // The number of lines in the source file
    char** sourceFile = NULL; // The source file (.asm file)
    FILE* srcPtr; // File pointer to open the source file.
    FILE* destPtr;

    Pair macroRange = {-1, -1};
    int sizeOfList = 0;
    Bool flag = false;

    srcPtr = fopen(fileName, "r");
    destPtr = fopen("D:/Assembler project/preAssembled.asm", "w");

    inputFile(srcPtr, &sourceFile, &numOfLines);

    for (int i = 0; i < numOfLines; i++)
    {
        flag = false;
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
            flag = true;
        }

        else if (strcmp(checkEndMacroDef, END_MACRO) == 0)
        {
            // Needs to search for the number of params and add to array.
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
            flag = true;
            macroRange.first = -1;
            macroRange.second = -1;
        }
        else if (macroRange.first != -1 && i > macroRange.first && macroRange.second == -1)
            flag = true;
        else
        {
            char* suspectedMacro = NULL;
            findMacroName(sourceFile[i], &suspectedMacro);
            int macroCalledIndex = locateMacroByName(macrosList, suspectedMacro);
            if (macroCalledIndex != -1)
            {
                flag = true; // Flagging that this line is a macro call.
                // Calling a macro
                // Creating an array of strings to contain the
                // registers names (ax, bx etc.)
                char registers[macrosList.list[macroCalledIndex].numberOfParameters][2];
                int registerIndex = 0; // Maybe needs to be fixed.
                for (int parameterIndex = 0;
                parameterIndex < macrosList.list[macroCalledIndex].numberOfParameters;
                parameterIndex++)
                {
                    // Finding the first register index in the i(th) line in sourceFile.
                    while (parameterIndex == 0 && sourceFile[i][registerIndex] != ' ')
                        registerIndex++;
                    // Now sourceFile[i][registerIndex] == ' ' so I'll increase it again by 1
                    if (sourceFile[i][registerIndex] == ' ')
                        registerIndex++;
                    // Found register. Now storing the name of it in the array
                    registers[parameterIndex][0] = sourceFile[i][registerIndex];
                    registerIndex++;
                    registers[parameterIndex][1] = sourceFile[i][registerIndex];
                    // List of registers looks like:
                    // ax, bl, ch, ... Now registerIndex is on the last letter of the register
                    // so registerIndex + 1 == ',', registersIndex + 2 == ' '
                    // and registersIndex + 3 = first letter of next register.

                    registerIndex += 3;
                }
                // Replacing the %1, %2, ... with the registers names.
                int cntParameters = 0;
                char argument[3];
                char numberOfParameter = '1';
                argument[0] = '%';
                char* ptr;
                char cpyOfLineInMarcoDefinition[200];

                for (int lineNumberInMacroDefinition = macrosList.list[macroCalledIndex].range.first; lineNumberInMacroDefinition <=
                                                                                                      macrosList.list[macroCalledIndex].range.second; lineNumberInMacroDefinition++)
                {
                    numberOfParameter = '0';
                    strcpy(cpyOfLineInMarcoDefinition, sourceFile[lineNumberInMacroDefinition]);
                    for (cntParameters = 0; cntParameters < macrosList.list[macroCalledIndex].numberOfParameters;
                           cntParameters++)
                    {
                        numberOfParameter = (char)((int)numberOfParameter + 1) ;
                        argument[1] = numberOfParameter;
                        argument[2] = '\0';
                        ptr = strstr(cpyOfLineInMarcoDefinition, argument);
                        if (ptr != NULL)
                        {
                            *ptr = registers[cntParameters][0];
                            ++ptr;
                            *ptr = registers[cntParameters][1];
                        }
                    }

                    fprintf(destPtr, "%s", cpyOfLineInMarcoDefinition);
                }

            }
        }
        if (!flag)
            fprintf(destPtr, "%s", sourceFile[i]);
    }



    free(macrosList.list); // Need to write free all memory function.
}


#endif //ASSEMBLER_PROJECT_PRE_ASSEMBLER_H