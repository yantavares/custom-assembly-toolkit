#ifndef LINKER_HPP
#define LINKER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>

using UsageTable = std::map<std::string, std::vector<int>>;
using DefinitionTable = std::map<std::string, int>;
using RelocationTable = std::vector<int>;
using Code = std::vector<int>;

class Linker
{
public:
    Linker();
    void linkFiles(const std::string &file1, const std::string &file2);

private:
    void applyCorrectionFactor(Code &c, RelocationTable &rt);
    void generateTables(std::ifstream &file, UsageTable &ut, DefinitionTable &dt, RelocationTable &rt, Code &c);

    UsageTable usageTable1;
    DefinitionTable definitionTable1;
    RelocationTable relocationTable1;
    Code code1;

    UsageTable usageTable2;
    DefinitionTable definitionTable2;
    RelocationTable relocationTable2;
    Code code2;

    int correctionFactor;
};

#endif
