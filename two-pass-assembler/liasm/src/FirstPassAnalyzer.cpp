#include "FirstPassAnalyzer.hpp"

FirstPassAnalyzer::FirstPassAnalyzer(
    std::unordered_map<std::string, u_int16_t> &labels)
    : labels(labels)
{
}

int FirstPassAnalyzer::analyse(std::ifstream &file)
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

        scanIntruction(instr, iss, label, address);
    }

    file.close();

    return 0;
}

void FirstPassAnalyzer::scanIntruction(std::string &instr, std::istringstream &iss,
                                       std::string &label, u_int16_t &address)
{
    std::string var;

    if (instr.back() == ':')
    {
        label = instr.substr(0, instr.size() - 1);

        if (!isLabelValid(label))
        {
            return;
        }

        labels[label] = address;

        if (!(iss >> instr))
        {
            return;
        }
    }

    if (!isInstrValid(instr))
    {
        return;
    }

    address++;

    if (instr == "CONST")
    {
        if (!(iss >> var))
        {
            throw std::runtime_error("CONST instruction must have a value.");
            return;
        }
    }
    else if (instr == "SPACE")
    {
        if (iss >> var && var[0] != ';')
        {
            throw std::runtime_error("SPACE instruction must not have a value.");
            return;
        }
    }
    else if (instr == "COPY")
    {
        std::string src, dest;

        if (!(iss >> src) || !(iss >> dest))
        {
            throw std::runtime_error("COPY instruction must have two values.");
            return;
        }

        if (!isLabelValid(src, true) || !isLabelValid(dest, true))
        {
            throw std::runtime_error("Invalid label in COPY instruction.");
            return;
        }
        address += 2;
    }
    else if (instr == "STOP" || instr == "THROW" || instr == "NOP")
    {
        if (iss >> var && var[0] != ';')
        {
            throw std::runtime_error("STOP and THROW instructions must not have a value.");
            return;
        }
    }
    else
    {
        if (!(iss >> var))
        {
            throw std::runtime_error("Instruction " + instr + " must have a value.");
            return;
        }

        if (!isLabelValid(var, false))
        {
            return;
        }
        address++;
    }
}

void FirstPassAnalyzer::printLabels()
{
    std::cout << "Label"
              << " ->  "
              << "Address" << std::endl;

    for (auto &kv : labels)
    {
        int spacesToAddLabel = 5 - kv.first.size(); // 5 is the size of the string "LABEL"
        std::string spacesLabel(spacesToAddLabel, ' ');

        std::cout << kv.first << spacesLabel << " ->  " << kv.second << std::endl;
    }

    std::cout << std::endl;
}

bool FirstPassAnalyzer::isLabelValid(std::string &label, bool checkExits)
{
    if (label[0] == ';')
    {
        return false;
    }

    if (checkExits)
    {
        if (labels.find(label) != labels.end())
        {
            return false;
        }
    }

    if (!isalpha(label[0]))
    {
        std::cerr << "Label " << label << " must start with a letter!" << '\n';
        return false;
    }

    if (std::find(reservedWords.begin(), reservedWords.end(), label) != reservedWords.end())
    {
        std::cerr << "Label " << label << " is a reserved word!" << '\n';
        throw std::runtime_error("Label is a reserved word.");
        return false;
    }

    return true;
}

bool FirstPassAnalyzer::isInstrValid(std::string &instr)
{
    if (instr[0] == ';')
    {
        return false;
    }

    if (std::find(reservedWords.begin(), reservedWords.end(), instr) == reservedWords.end())
    {
        return false;
    }

    return true;
}