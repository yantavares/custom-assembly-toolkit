#ifndef PRE_PROCESSING_HPP
#define PRE_PROCESSING_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cctype>
#include <algorithm>

class PRE_PROCESSING
{
private:
    std::vector<std::string> EQU_VALUE;
    std::vector<std::string> EQU_LABEL;

public:
    PRE_PROCESSING(){};
    ~PRE_PROCESSING(){};
    void ReadFile(std::string filename);
    bool FindIf(std::vector<std::string> linha);
    bool IfAnalizer(std::vector<std::string> linha);
    bool DefineEQU(std::vector<std::string> linha);
    std::string FindEQU(std::string word);
    std::string Uppercase(std::string word);
    bool Comments(std::string word);
};
#endif