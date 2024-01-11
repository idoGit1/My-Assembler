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

// Functions declarations:
void spreadMacros(char*);
void spreadLabels();


void preAssembler(char* fileName)
{
    spreadMacros(fileName);
    spreadLabels();
}

void spreadMacros(char* fileName)
{

    int numOfLines = 0; // The number of lines in the source file
    char** sourceFile = NULL; // The source file (.asm file)
    FILE* srcPtr; // File pointer to open the source file.
    FILE* destPtr;

    Pair macroRange = {-1, -1};
    Bool flag;
    srcPtr = fopen("D:/Assembler project/sourceFile.asm", "r");// Debugging line
    //srcPtr = fopen(fileName, "r");
    destPtr = fopen("D:/Assembler project/preAssembled.asm", "w");

    inputFile(srcPtr, &sourceFile, &numOfLines, fileName);

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
                int cntParameters;
                char argument[3];
                char numberOfParameter;
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
                        while (ptr != NULL)
                        { 
                            *ptr = registers[cntParameters][0];
                            ++ptr;
                            *ptr = registers[cntParameters][1];
                            ptr = strstr(cpyOfLineInMarcoDefinition, argument);
                        }
                    }

                    fprintf(destPtr, "%s", cpyOfLineInMarcoDefinition);
                }

            }
        }
        if (!flag)
            fprintf(destPtr, "%s", sourceFile[i]);
    }


    freeAllMemory(&macrosList);
}

void spreadLabels()
{
    FILE* fptr = fopen("D:/Assembler project/preAssembled.asm", "r");
    char currentLine[200];
    char* ptrToEndOfLabel;
    char** labelTable = (char**) malloc(1 * sizeof(char[200]));
    int* labelLineNumber = (int*) malloc(1 * sizeof(int));
    int labelCounter = 0;
    int lineCounter = 0;
    while (fgets(currentLine, 200, fptr))
    {
        ptrToEndOfLabel = strstr(currentLine, ":");
        if (ptrToEndOfLabel != NULL) 
        {
            // Find the name of the label, labels looks like
            // "nameOfLabel": so first we look after the ':' and then we find 
            // the name of the label and add it to the table.
            labelTable[labelCounter] = findLabelNameBackwords(currentLine, ptrToEndOfLabel);
            labelCounter++;
            labelTable = (char**) realloc(labelTable, (labelCounter + 1) * sizeof(char[200]));
            labelLineNumber[labelCounter] = lineCounter;
            labelLineNumber = (int*) realloc(labelLineNumber, (labelCounter + 1) * sizeof(int));
        }
        lineCounter++;
    }
    fptr = fopen("D:/Assembler project/preAssembled.asm", "w");
    while (fgets(currentLine, 200, fptr))
    {
        for (int i = 0; i < labelCounter; i++)
        {
            if (strncmp(currentLine + 4, labelTable[i], strlen(labelTable[i]))
            == 0)
            {
                fprintf(fptr, "jmp %#02x", labelLineNumber[i]);
            }
            else if (strstr(currentLine, ":") == NULL)
                fprintf(fptr, "");
            else
            fprintf(fptr, currentLine);
        }
    }
    // Need to go over the file again and write jmp lableLineNumber[i]
    // instead of "lableName".

}

#endif //ASSEMBLER_PROJECT_PRE_ASSEMBLER_H