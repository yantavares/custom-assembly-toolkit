#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cctype>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>

class Assembler
{
public:
    Assembler();
    void ReadFile(const std::string &filename);
    void WriteFile(const std::string &filename);
    int line_counter;

private:
    int PC;
    std::string modname;
    bool should_be_linked;
    std::vector<std::string> labels;
    std::vector<std::string> label_addresses;
    std::vector<std::string> program;
    std::vector<std::string> public_symbols;
    std::vector<std::string> symbols;
    std::vector<std::string> symbol_addresses;
    std::vector<bool> symbols_notD;
    std::vector<std::string> opcodes;
    std::vector<std::string> opcode_values;
    std::vector<std::string> undefined_labels;
    std::vector<std::string> label_notD;
    bool second_label;
    std::unordered_map<std::string, std::vector<int>> usage_table;
    std::unordered_map<std::string, int> definition_table;
    std::vector<int> relocation_table;
    bool is_hex(std::string num);
    std::string parse_hex_to_dec(std::string num);

    void UpdateDefinitionTable(std::string label, int value);
    void ParseDefinitionTable(const std::vector<std::string> &line);
    std::vector<std::string> FindLabel(std::vector<std::string> line);
    void WriteProgram(const std::vector<std::string> &line, bool shouldBeLinked = false);
    void WriteSymbols(const std::vector<std::string> &line);
    std::vector<std::string> ProcessCopyInstruction(std::vector<std::string> line);

    void ErrorNotDefined();
    void LexicERROR(std::string word);
    void InstructionConfig(std::vector<std::string> line);
};
