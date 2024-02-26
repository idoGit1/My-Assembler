#include <iostream>
#include <fstream> // For file handling
#include <string>
#include <vector>
#include <cstdlib> // For dynamic memory allocation
#include <cstring> // For string manipulation

//#include "functions.h"
#include "Data Structures.h" 

#define START_MACRO "%macro"
#define END_MACRO "%endmacro"

std::vector<Macro> macrosList;

void spreadMacros(const std::string& fileName);
void spreadLabels();
void removeBlankLines();
void preAssembler(const std::string& fileName) 
{
    spreadMacros(fileName);
    removeBlankLines();
    spreadLabels();
}


void removeBlankLines()
{
    std::ifstream srcPtr("D:/Assembler project/preAssembled.asm");
    std::ofstream destPtr("D:/Assembler project/preAssembled.asm");

    std::string line;

    while (std::getline(srcPtr, line))
    {
        if (line != "\n")
            destPtr << line << "\n";
        else
            continue;
    }
}

void spreadMacros(const std::string& fileName) 
{
    std::vector<std::string>sourceFile;
    std::string line;
    
    std::ifstream srcPtr("D:/Assembler project/sourceFile.asm");
    std::ofstream destPtr("D:/Assembler project/preAssembled.asm");

    std::pair<int, int> macroRange = { -1, -1 };
    bool flag;

    while (std::getline(srcPtr, line))
        sourceFile.push_back(line);

    for (int i = 0; i < sourceFile.size(); i++)
    {
        flag = false;

        std::string checkMacroDef = sourceFile[i].substr(0, 6);
        std::string checkEndMacroDef = sourceFile[i].substr(0, 9);

        if (checkMacroDef == START_MACRO)
        {
            macroRange.first = i;
            flag = true;
        }

        else if (checkEndMacroDef == END_MACRO)
        {
            // Needs to search for the number of params and add to array.
            int j = 6;
            while (!isdigit(sourceFile[macroRange.first][j]))
                j++;
            int params = sourceFile[macroRange.first][j] - '0';
            // VERY IMPORTANT: end every string with a \0.
            macroRange.second = i;
            Macro newDef;
            newDef.numberOfParameters = params;
            newDef.range.first = macroRange.first + 1;
            newDef.range.second = macroRange.second - 1;

            //char* name = (char*)malloc(sizeof(char) * 20);
            std::string name;
            
            // Name starts in 7 until the next space:
            name = sourceFile[macroRange.first].substr(7);
            size_t spacePos = name.find(" ");
            name = name.substr(0, spacePos);
            
            newDef.name = name;

            macrosList.push_back(newDef);
           
            flag = true;
            macroRange.first = -1;
            macroRange.second = -1;
        }
        else if (macroRange.first != -1 && i > macroRange.first && macroRange.second == -1)
            flag = true;
        else
        {
            std::string suspectedMacro;
            size_t pos = sourceFile[i].find(" ");

            if (pos == std::string::npos)
                goto print;

            suspectedMacro = sourceFile[i].substr(0, pos);

            size_t macroCalledIndex = -1;
            
            for (size_t i = 0; i < macrosList.size(); i++)
                if (macrosList[i].name == suspectedMacro)
                    macroCalledIndex = i;


            if (macroCalledIndex != -1)
            {
                flag = true; // Flagging that this line is a macro call.
                // Calling a macro
                // Creating an array of strings to contain the
                // registers names (ax, bx etc.)
                std::vector<std::vector<char>> registers
                (macrosList[macroCalledIndex].numberOfParameters, std::vector<char>(2));
                int registerIndex = 0; // Maybe needs to be fixed.
                for (int parameterIndex = 0;
                    parameterIndex < macrosList[macroCalledIndex].numberOfParameters;
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
                std::string argument;
                char numberOfParameter;
                argument = '%';
                size_t ptr;
                std::string cpyOfLineInMarcoDefinition;

                for (int lineNumberInMacroDefinition = macrosList[macroCalledIndex].range.first; lineNumberInMacroDefinition <=
                    macrosList[macroCalledIndex].range.second; lineNumberInMacroDefinition++)
                {
                    numberOfParameter = '0';
                    cpyOfLineInMarcoDefinition = sourceFile[lineNumberInMacroDefinition];
                    for (cntParameters = 0; cntParameters < macrosList[macroCalledIndex].numberOfParameters;
                        cntParameters++)
                    {
                        numberOfParameter = (char)((int)numberOfParameter + 1);
                        argument += numberOfParameter;
                        //argument[2] = '\0';
                        ptr = cpyOfLineInMarcoDefinition.find(argument);
                        while (ptr != std::string::npos)
                        {
                            cpyOfLineInMarcoDefinition[ptr] = registers[cntParameters][0];
                            ++ptr;
                            cpyOfLineInMarcoDefinition[ptr] = registers[cntParameters][1];
                            ptr = cpyOfLineInMarcoDefinition.find(argument);
                        }
                    }

                    destPtr << cpyOfLineInMarcoDefinition;
                }

            }
        }
print:
        if (!flag)
            destPtr << sourceFile[i];
    }
    // Cleaning processes
    srcPtr.close();
    destPtr.close();
}

void spreadLabels() 
   {
    std::ifstream file("preAssembled.asm"); // Open pre-assembled file for reading
    std::ofstream outputFile("preAssembled_tmp.asm"); // Open temporary file for writing

    std::vector<std::string> labelTable;
    std::vector<int> labelLineNumber;
    std::string line;
    int lineCounter = 0;

    // Process each line of the pre-assembled file
    while (std::getline(file, line)) 
    {
        size_t labelPos = line.find(":");

        if (labelPos != std::string::npos)
        {
            std::string labelName = line.substr(0, labelPos);

            labelTable.push_back(labelName);
            labelLineNumber.push_back(lineCounter);
        }
        lineCounter++;
    }

    // Reset file pointer to the beginning
    file.clear();
    file.seekg(0, std::ios::beg);

    // Process each line of the pre-assembled file again
    while (std::getline(file, line)) 
    {
        size_t labelPos = line.find(":");
        size_t jmpPos = line.find("jmp");
        if (labelPos == std::string::npos && jmpPos == std::string::npos) 
        {
            outputFile << line << "\n";
        }
        else if (jmpPos != std::string::npos) 
        {
            // If line contains a jmp to a label, find the corresponding label number
            std::string labelName = line.substr(4, std::string::npos);
            for (size_t i = 0; i < labelTable.size(); ++i) {
                if (labelTable[i] == labelName) {
                    // Write jump instruction to output file
                    outputFile << "jmp 0x" << std::hex << labelLineNumber[i] << "\n";
                    break;
                }
            }
        }
        else
            outputFile << "\n";
    }

    file.close(); // Close file
    outputFile.close(); // Close output file

    // Rename temporary file to original file
    //std::rename("preAssembled_tmp.asm", "preAssembled.asm");
}

