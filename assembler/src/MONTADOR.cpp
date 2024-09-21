#include "MONTADOR.hpp"

Assembler::Assembler::Assembler()
    : line_counter(1), PC(0), should_be_linked(false), second_label(false),
      opcodes{"ADD", "SUB", "MUL", "DIV", "JMP", "JMPN", "JMPP", "JMPZ", "COPY", "LOAD", "STORE", "INPUT", "OUTPUT", "STOP"},
      opcode_values{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14"}
{
}

void Assembler::ReadFile(const std::string &filename)
{
    std::string line;
    std::string word;
    std::vector<std::string> parsed_line;

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "PRE file not found" << std::endl;
        return;
    }

    while (getline(file, line))
    {
        std::istringstream iss(line);
        parsed_line.clear();

        while (iss >> word)
        {
            parsed_line.push_back(word);
        }

        ParseDefinitionTable(parsed_line);

        parsed_line = FindLabel(parsed_line);
        if (parsed_line.size() != 0)
        {
            FindLabel(parsed_line);
        }

        if (parsed_line[0] == "BEGIN")
        {
            modname = labels.back();
            should_be_linked = true;
            line_counter++;
            continue;
        }

        if (parsed_line[0] == "END")
        {
            line_counter++;
            continue;
        }

        if (parsed_line[0] == "EXTERN")
        {
            line_counter++;
            usage_table[labels.back()].push_back(-1);
            continue;
        }

        if (parsed_line[0] == "PUBLIC")
        {
            line_counter++;
            public_symbols.push_back(parsed_line[1]);
            if (parsed_line[1] == modname)
            {
                UpdateDefinitionTable(parsed_line[1], 0);
            }
            else
            {
                UpdateDefinitionTable(parsed_line[1], -1);
            }
            continue;
        }

        if (parsed_line[0] == "COPY")
        {
            parsed_line = ProcessCopyInstruction(parsed_line);
        }

        WriteProgram(parsed_line, should_be_linked);

        if (parsed_line.size() > 1 || (parsed_line.size() == 1 && parsed_line[0].back() != ':'))
        {
            if (parsed_line.size() != 1 && parsed_line[0].back() != ':')
            {
                PC += parsed_line.size();
            }
            else
                PC++;
        }
        line_counter++;
    }
    ErrorNotDefined();
}

void Assembler::UpdateDefinitionTable(std::string label, int value)
{

    if (definition_table.find(label) != definition_table.end() && definition_table[label] != -1)
    {
        if (value != -1)
        {

            std::cerr << "Key " + label + " already exists in definition table";
            throw std::invalid_argument("Semantic Error: LABEL REDEFINED");
        }
    }
    else
    {
        definition_table[label] = value;
    }
}

void Assembler::ParseDefinitionTable(const std::vector<std::string> &line)
{
    if (line[0].back() == ':')
    {
        std::string temp = line[0];
        temp.pop_back();

        if (definition_table.find(temp) != definition_table.end())
        {
            std::vector<std::string> temp_line = line;

            while (temp_line[0].back() == ':')
            {
                temp_line.erase(temp_line.begin());
            }

            UpdateDefinitionTable(temp, PC);
        }
    }
}

void Assembler::WriteFile(const std::string &filename)
{
    std::string output_filename;
    if (should_be_linked)
        output_filename = filename.substr(0, filename.size() - 3) + "e";
    else
        output_filename = filename.substr(0, filename.size() - 3) + "obj";

    std::ofstream file(output_filename);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file for writing" << std::endl;
        return;
    }

    if (should_be_linked)
    {
        file << "USO\n";
        for (const auto &[key, value] : usage_table)
        {
            for (int v : value)
            {
                if (v != -1)
                {
                    file << key << " " << v << '\n';
                }
            }
        }

        file << "\nDEF\n";
        for (const auto &[key, value] : definition_table)
        {

            file << key << " " << value << '\n';
        }

        file << "\nREAL\n";
        for (int val : relocation_table)
        {
            file << val;
        }
        file << '\n';
    }

    file << '\n';

    for (const std::string &i : program)
    {
        file << i << ' ';
    }
    file.close();
}

std::vector<std::string> Assembler::FindLabel(std::vector<std::string> line)
{
    if (line[0].back() == ':')
    {
        if (second_label)
        {
            throw std::invalid_argument("Two labels");
        }
        second_label = true;
        std::string label = line[0];
        label.pop_back();
        LexicERROR(label);
        auto it = std::find(labels.begin(), labels.end(), label);
        if (it != labels.end())
        {
            throw std::invalid_argument("Semantic Error: LABEL REDEFINED");
        }
        else if (line.size() != 1)
        {
            if (line[1] == "SPACE" || line[1] == "CONST")
            {
                second_label = false;
                return line;
            }
            else
            {
                labels.push_back(label);
                label_addresses.push_back(std::to_string(PC));
                line.erase(line.begin());
            }
        }
        else
        {
            labels.push_back(label);
            label_addresses.push_back(std::to_string(PC));
            line.erase(line.begin());
        }
        auto undef_it = std::find(undefined_labels.begin(), undefined_labels.end(), label);
        if (undef_it != undefined_labels.end())
        {
            int pos;
            while (undef_it != undefined_labels.end())
            {
                pos = std::distance(undefined_labels.begin(), undef_it);
                program[std::stoi(label_notD[pos])] = std::to_string(PC);
                undefined_labels[pos] = "0";
                undef_it = std::find(undefined_labels.begin(), undefined_labels.end(), label);
            }
        }
    }
    else
    {
        second_label = false;
    }
    return line;
}

void Assembler::WriteProgram(const std::vector<std::string> &line, bool shouldBeLinked)
{
    int position;
    for (size_t i = 0; i < line.size(); ++i)
    {
        if (shouldBeLinked)
        {
            auto it = usage_table.find(line[i]);
            if (it != usage_table.end())
            {
                relocation_table.push_back(1);
                usage_table[line[i]].push_back(PC + static_cast<int>(i) + 1);
                program.push_back("0");
                continue;
            }
        }

        auto opcode_it = std::find(opcodes.begin(), opcodes.end(), line[i]);
        if (opcode_it != opcodes.end())
        {
            if (i == 0)
            {
                InstructionConfig(line);
                if (shouldBeLinked)
                {
                    relocation_table.push_back(0);
                }
                position = std::distance(opcodes.begin(), opcode_it);
                program.push_back(opcode_values[position]);
                continue;
            }
            else
            {
                throw std::invalid_argument("Syntax Error");
            }
        }

        auto label_it = std::find(labels.begin(), labels.end(), line[i]);
        if (label_it != labels.end())
        {
            if (shouldBeLinked)
            {
                relocation_table.push_back(1);
            }
            LexicERROR(line[i]);
            position = std::distance(labels.begin(), label_it);
            program.push_back(label_addresses[position]);
            continue;
        }

        auto symbol_it = std::find(symbols.begin(), symbols.end(), line[i]);
        if (symbol_it != symbols.end())
        {
            relocation_table.push_back(1);
            position = std::distance(symbols.begin(), symbol_it);
            program.push_back(symbol_addresses[position]);
            symbol_addresses[position] = std::to_string(PC + static_cast<int>(i));
            continue;
        }

        if (line[0].back() == ':')
        {
            if (shouldBeLinked)
            {
                relocation_table.push_back(0);
            }
            WriteSymbols(line);
            break;
        }
        else
        {
            if (line[0] != "JMP" && line[0] != "JMPP" && line[0] != "JMPZ" && line[0] != "JMPN")
            {
                LexicERROR(line[i]);
                if (shouldBeLinked)
                {
                    relocation_table.push_back(1);
                }
                symbols.push_back(line[i]);
                symbol_addresses.push_back(std::to_string(PC + static_cast<int>(i)));
                symbols_notD.push_back(true);
                program.push_back("-1");
            }
            else
            {
                LexicERROR(line[i]);
                label_notD.push_back(std::to_string(PC + static_cast<int>(i)));
                undefined_labels.push_back(line[i]);
                program.push_back("-1");
            }
        }
    }
}

void Assembler::WriteSymbols(const std::vector<std::string> &line)
{
    std::string sym = line[0];
    sym.pop_back();
    LexicERROR(sym);

    auto symbol_it = std::find(symbols.begin(), symbols.end(), sym);
    auto special_symbol_it = std::find(public_symbols.begin(), public_symbols.end(), sym);

    bool isSymbol = symbol_it != symbols.end();
    bool is_public = special_symbol_it != public_symbols.end();

    if (isSymbol || is_public)
    {
        int position = std::distance(symbols.begin(), symbol_it);
        if (!is_public)
        {

            if (!symbols_notD[position])
            {
                throw std::invalid_argument("Semantic Error: Symbol redefined");
            }
            symbols_notD[position] = false;
        }
        if (line[1] == "CONST")
        {
            if (line.size() != 3)
            {
                throw std::invalid_argument("Syntax Error: CONST must have a value");
            }
            if (is_hex(line[2]))
            {
                program.push_back(parse_hex_to_dec(line[2]));
            }
            else
            {

                program.push_back(line[2]);
            }

            if (is_public && !isSymbol)
                return;
        }
        else if (line[1] == "SPACE")
        {
            if (line.size() != 2)
            {
                throw std::invalid_argument("Syntax Error: SPACE must not have a value");
            }
            program.push_back("0");
            if (is_public && !isSymbol)
                return;
        }
        else
        {
            throw std::invalid_argument("Error");
        }
        int pos = std::stoi(symbol_addresses[position]);
        while (program[pos] != "-1")
        {
            int temp = std::stoi(program[pos]);
            program[pos] = std::to_string(PC);
            pos = temp;
        }
        program[pos] = std::to_string(PC);
    }
}

std::vector<std::string> Assembler::Assembler::ProcessCopyInstruction(std::vector<std::string> line)
{
    std::stringstream ss(line[1]);
    std::string first_symbol, second_symbol;
    std::getline(ss, first_symbol, ',');
    std::getline(ss, second_symbol, ',');
    line[1] = first_symbol;
    LexicERROR(line[1]);
    line.push_back(second_symbol);
    LexicERROR(line[2]);
    return line;
}

//----------------------------------ERRORS-----------------------------------//

void Assembler::Assembler::ErrorNotDefined()
{
    int i;
    std::stringstream ss;
    for (i = 0; i < undefined_labels.size(); i++)
    {
        if (undefined_labels[i] != "0")
        {
            ss << "Semantic Error: Label " << undefined_labels[i] << " not Defined";
            throw std::invalid_argument(ss.str());
        }
    }
    for (i = 0; i < symbols_notD.size(); i++)
    {
        if (std::find(public_symbols.begin(), public_symbols.end(), symbols[i]) != public_symbols.end())
            continue;

        if (symbols_notD[i])
        {
            ss << "Semantic Error: Symbol " << symbols[i] << " not Defined";
            throw std::invalid_argument(ss.str());
        }
    }
    for (i = 0; i < public_symbols.size(); i++)
    {
        if (definition_table[public_symbols[i]] == -1)
        {
            ss << "Semantic Error: Public Symbol " << public_symbols[i] << " not Defined";
            throw std::invalid_argument(ss.str());
        }
    }
}

void Assembler::Assembler::LexicERROR(std::string word)
{
    if (isdigit(word[0]))
    {
        throw std::invalid_argument("Lexic Error");
    }
    for (int i = 0; i < word.size(); i++)
    {
        if (!isalnum(word[i]))
        {
            if (word[i] != '_')
            {
                throw std::invalid_argument("Lexic Error");
            }
        }
    }
}

void Assembler::Assembler::InstructionConfig(std::vector<std::string> line)
{
    if (line[0] != "COPY" && line[0] != "STOP")
    {
        if (line.size() != 2)
        {
            throw std::invalid_argument("Syntax Error");
        }
    }
    else if (line[0] == "COPY")
    {
        if (line.size() != 3)
        {
            throw std::invalid_argument("Syntax Error");
        }
    }
    else
    {
        if (line.size() != 1)
        {
            throw std::invalid_argument("Syntax Error");
        }
    }
}

// --------------------- AUX FUNCTIONS --------------------- //

bool Assembler::is_hex(std::string num)
{
    if (num.substr(0, 2) == "0x" || num.substr(0, 2) == "0X")
    {
        for (int i = 2; i < num.size(); i++)
        {
            if (!isxdigit(num[i]))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

std::string Assembler::parse_hex_to_dec(std::string num)
{
    std::string hex_num = num.substr(2);

    unsigned int decimal_num;
    std::stringstream ss;
    ss << std::hex << hex_num;
    ss >> decimal_num;

    if (ss.fail())
    {
        throw std::invalid_argument("Invalid hexadecimal number");
    }

    std::stringstream result;
    result << decimal_num;

    return result.str();
}
