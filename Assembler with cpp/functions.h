#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Data Structures.h" // Assuming this header contains necessary declarations

#ifndef ASSEMBLER_PROJECT_FUNCTIONS_H
#define ASSEMBLER_PROJECT_FUNCTIONS_H

// Define bool if not already defined


std::string findName(const std::string& line) {
    size_t start = 7;
    size_t end = line.find(' ', start);
    if (end == std::string::npos) {
        end = line.size();
    }
    return line.substr(start, end - start);
}

void inputFile(const std::string& fileName, std::vector<std::string>& sourceFile, int& numOfLines) {
    std::ifstream fptr(fileName);
    std::string line;
    while (std::getline(fptr, line)) {
        sourceFile.push_back(line);
    }
    numOfLines = sourceFile.size();
}

void append(MacroList& list, const Macro& newElement) {
    list.list.push_back(newElement);
}

void findMacroName(const std::string& line, std::string& suspectedMacro) {
    size_t end = line.find(' ');
    if (end == std::string::npos) {
        end = line.size();
    }
    suspectedMacro = line.substr(0, end);
}

int locateMacroByName(const MacroList& list, const std::string& name) {
    for (size_t i = 0; i < list.size; ++i) {
        if (list.list[i].name == name) {
            return i;
        }
    }
    return -1;
}

std::string findLabelNameBackwords(const std::string& line, const std::string::iterator& ptrToEndOfLabel) {
    std::string::iterator ptrToBeginOfLabel = ptrToEndOfLabel - 1;
    while (ptrToBeginOfLabel != line.begin() && *ptrToBeginOfLabel != ' ') {
        --ptrToBeginOfLabel;
    }
    if (*ptrToBeginOfLabel == ' ') ++ptrToBeginOfLabel;
    return std::string(ptrToBeginOfLabel, ptrToEndOfLabel);
}

#endif // ASSEMBLER_PROJECT_FUNCTIONS_H
