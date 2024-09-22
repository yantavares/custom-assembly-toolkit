#ifndef SECONDPASSANALYZER_HPP
#define SECONDPASSANALYZER_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <bitset>
#include <memory>

class SecondPassAnalyzer
{
public:
    SecondPassAnalyzer(
        std::unordered_map<std::string, u_int16_t> &labels);

    int analyse(std::ifstream &file);

private:
    std::unordered_map<std::string, u_int16_t> &labels;

    void parseInstruction(std::string &instr, std::istringstream &iss,
                          std::string &label, u_int16_t &address);

    int16_t parseValue(std::string &value);

    u_int16_t getOpcode(std::string &instr);

    void writeSignedConstantToMemory(u_int16_t index, int16_t value);
    void writeValueToFile(u_int16_t type, u_int16_t index, u_int16_t value);
    void writeTokenToMEMtext(std::string &instr, u_int16_t &address, std::string &label);

    std::unordered_map<std::string, u_int16_t> opcodes{
        {"ADD", 0x01},
        {"SUB", 0x02},
        {"MUL", 0x03},
        {"DIV", 0x04},
        {"JMP", 0x05},
        {"JMPN", 0x06},
        {"JMPP", 0x07},
        {"JMPZ", 0x08},
        {"COPY", 0x09},
        {"LOAD", 0x0A},
        {"STORE", 0x0B},
        {"INPUT", 0x0C},
        {"OUTPUT", 0x0D},
        {"STOP", 0x0E},
        {"THROW", 0x0F},
        {"NOP", 0x10},
    };

    u_int16_t elementSize;
    std::unordered_map<std::string, u_int16_t> usedLabels;

protected:
    std::unique_ptr<std::fstream> MEMdata;
    std::unique_ptr<std::fstream> MEMtext;
};

#endif // SECONDPASSANALYZER_HPP