#include "SecondPassAnalyzer.hpp"

SecondPassAnalyzer::SecondPassAnalyzer(std::unordered_map<std::string, uint16_t> &labels)
    : labels(labels), elementSize(16) // 16 bits per element
{
    MEMdata = std::make_unique<std::fstream>("./MEMdata.txt", std::ios::in | std::ios::out | std::ios::binary);
    MEMtext = std::make_unique<std::fstream>("./MEMtext.txt", std::ios::in | std::ios::out | std::ios::binary);

    if (!MEMdata->is_open())
    {
        throw std::runtime_error("Failed to open MEMdata.txt");
    }
    if (!MEMtext->is_open())
    {
        throw std::runtime_error("Failed to open MEMtext.txt");
    }
}

int SecondPassAnalyzer::analyse(std::ifstream &file)
{
    std::string line, instr, var;
    std::string label;
    u_int16_t address = 0;

    while (getline(file, line))
    {
        std::istringstream iss(line);

        if (!(iss >> instr))
        {
            continue; // Empty line or no instruction
        }

        parseInstruction(instr, iss, label, address);
    }

    for (auto const &[key, val] : labels)
    {
        if (usedLabels[key] == 0)
        {
            std::cerr << "[Warning] "
                      << "Label " << key << " is defined but not used.\n";
        }
    }

    file.close();

    return 0;
}

void SecondPassAnalyzer::parseInstruction(std::string &instr, std::istringstream &iss,
                                          std::string &label, u_int16_t &address)
{
    if (instr.back() == ':')
    {
        label = instr.substr(0, instr.size() - 1);

        if (!(iss >> instr))
        {
            return;
        }
    }

    if (instr[0] == ';')
    {
        return; // Comment detected, stop processing the line
    }

    if (instr == "CONST")
    {
        std::string value;
        iss >> value;

        int16_t parsedValue = parseValue(value);

        writeSignedConstantToMemory(labels[label], parsedValue); // Write to MEMdata
        writeValueToFile(1, address++, 0x10);                    // Write NOP to MEMtext
        return;
    }
    else if (instr == "SPACE")
    {
        writeValueToFile(0, labels[label], 0); // Write to MEMdata
        writeValueToFile(1, address++, 0x10);  // Write NOP to MEMtext
        return;
    }
    else if (instr == "COPY")
    {
        std::string src, dest;
        iss >> src >> dest;

        writeValueToFile(1, address++, getOpcode(instr)); // Write to MEMtext

        if (labels.find(src) != labels.end())
        {
            usedLabels[src]++;
            writeValueToFile(1, address++, labels[src]); // Write to MEMtext
        }
        else
        {
            std::cerr << "Invalid source label " << src << '\n';
            throw std::runtime_error("Invalid source label.");
        }
        if (labels.find(dest) != labels.end())
        {
            usedLabels[dest]++;
            writeValueToFile(1, address++, labels[dest]); // Write to MEMtext
        }
        else
        {
            std::cerr << "Invalid destination label " << dest << '\n';
            throw std::runtime_error("Invalid destination label.");
        }

        return;
    }

    writeTokenToMEMtext(instr, address, label); // Write to MEMtext

    address++;

    while (iss >> instr)
    {
        if (instr[0] == ';')
        {
            break; // Comment detected, stop processing the line
        }
        writeTokenToMEMtext(instr, address, label); // Write to MEMtext
        address++;
    }
}

u_int16_t SecondPassAnalyzer::getOpcode(std::string &inst)
{
    if (opcodes.find(inst) == opcodes.end())
    {
        std::cerr << "Invalid token " << inst << '\n';
        throw std::runtime_error("Invalid token.");
    }
    return opcodes[inst];
}

void SecondPassAnalyzer::writeValueToFile(u_int16_t type, u_int16_t index, u_int16_t value)
{
    // Convert the value to a binary string
    std::string binaryString = std::bitset<16>(value).to_string();

    // Calculate the position in the file.
    // Note: This calculation assumes each line in the file is 16 characters long plus a newline character.
    std::streampos pos = index * (elementSize + 1); // +1 for the newline character

    switch (type)
    {
    case 0:
        MEMdata->seekp(pos);
        *MEMdata << binaryString << std::endl; // Write the binary string with a newline
        break;

    case 1:
        MEMtext->seekp(pos);
        *MEMtext << binaryString << std::endl; // Write the binary string with a newline
        break;
    }
}

void SecondPassAnalyzer::writeSignedConstantToMemory(u_int16_t index, int16_t value)
{
    // Convert the value to a binary string
    std::string binaryString = std::bitset<16>(value).to_string();

    // Calculate the position in the file.
    // Note: This calculation assumes each line in the file is 16 characters long plus a newline character.
    std::streampos pos = index * (elementSize + 1); // +1 for the newline character

    MEMdata->seekp(pos);
    *MEMdata << binaryString << std::endl; // Write the binary string with a newline
}

int16_t SecondPassAnalyzer::parseValue(std::string &value)
{
    int16_t parsedValue;

    if (value[0] == '-') // Negative value
    {
        try
        {
            parsedValue = -1 * std::stoi(value.substr(1));
        }
        catch (const std::exception &e)
        {
            std::cerr << "Invalid value " << value << '\n';
            throw std::runtime_error("Invalid value for CONST - must be an integer.");
        }
    }

    else
    {
        try
        {
            parsedValue = std::stoi(value);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Invalid value " << value << '\n';
            throw std::runtime_error("Invalid value for CONST - must be an integer.");
        }
    }

    if (parsedValue > 32767)
    {
        throw std::runtime_error("Value out of range.");
    }

    return parsedValue;
}

void SecondPassAnalyzer::writeTokenToMEMtext(std::string &instr, u_int16_t &address, std::string &label)
{
    if (labels.find(instr) != labels.end())
    {
        usedLabels[instr]++;
        writeValueToFile(1, address, labels[instr]); // Write to MEMtext
    }
    else
    {
        writeValueToFile(1, address, getOpcode(instr)); // Write to MEMtext
    }
}