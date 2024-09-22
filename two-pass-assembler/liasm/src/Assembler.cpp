#include "Assembler.hpp"

Assembler::Assembler(std::unordered_map<std::string, u_int16_t> labels, std::string mode = "normal")
    : mode(mode), labels(labels), elementSize(16) // 16 bits per element
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

int Assembler::execute()
{
    u_int16_t instr;

    while (true)
    {
        instr = readValueFromFile(1, PC); // 1 -> MEMtext

        if (instr > 0x10 || instr < 0x01)
        {
            std::cerr << "Invalid instruction: " << instr << " at address " << PC << std::endl;
            throw std::runtime_error("Execution failed due to invalid instruction.");
        }

        if (executeInstruction(instr) == 1)
            break;
    }
    return 0;
}

u_int16_t Assembler::executeInstruction(u_int16_t &instr)
{

    if (instr == 0x01)
    {
        addI(readValueFromFile(1, PC + 1)); // ADD
        return 0;
    }
    else if (instr == 0x02)
    {
        subI(readValueFromFile(1, PC + 1)); // SUB
        return 0;
    }
    else if (instr == 0x03)
    {
        multI(readValueFromFile(1, PC + 1)); // MUL
        return 0;
    }
    else if (instr == 0x04)
    {
        divI(readValueFromFile(1, PC + 1)); // DIV
        return 0;
    }
    else if (instr == 0x05)
    {
        jumpI(readValueFromFile(1, PC + 1)); // JMP
        return 0;
    }
    else if (instr == 0x06)
    {
        jumpNI(readValueFromFile(1, PC + 1)); // JMPN
        return 0;
    }
    else if (instr == 0x07)
    {
        jumpPI(readValueFromFile(1, PC + 1)); // JMPP
        return 0;
    }
    else if (instr == 0x08)
    {
        jumpZI(readValueFromFile(1, PC + 1)); // JMPZ
        return 0;
    }
    else if (instr == 0x09)
    {
        copyI(readValueFromFile(1, PC + 1), readValueFromFile(1, PC + 2)); // COPY
        return 0;
    }
    else if (instr == 0x0A)
    {
        loadI(readValueFromFile(1, PC + 1)); // LOAD
        return 0;
    }
    else if (instr == 0x0B)
    {
        storeI(readValueFromFile(1, PC + 1)); // STORE
        return 0;
    }
    else if (instr == 0x0C)
    {
        inputI(readValueFromFile(1, PC + 1)); // INPUT
        return 0;
    }
    else if (instr == 0x0D)
    {
        outputI(readValueFromFile(1, PC + 1)); // OUTPUT
        return 0;
    }
    else if (instr == 0x0E)
    {
        stopI(); // STOP
        return 1;
    }
    else if (instr == 0x0F)
    {
        throwI(); // THROW
        return 1;
    }
    else if (instr == 0x10)
    {
        PC += 1; // NOP
        return 0;
    }
    else
    {
        std::cerr << "Invalid instruction: " << instr << " at address " << PC << std::endl;
        throw std::runtime_error("Execution failed due to invalid instruction.");
    }
}

void Assembler::inputI(u_int16_t variable)
{
    u_int16_t value;
    if (mode == "debug")
        std::cout << "Enter a value for " << findKeyByValue(variable) << ": ";
    std::cin >> value;
    writeValueToFile(0, variable, value); // 0 -> MEMdata
    PC += 2;
}

void Assembler::loadI(u_int16_t variable)
{
    ACC = readSignedValueFromFile(0, variable); // 0 -> MEMdata
    PC += 2;
}

void Assembler::addI(u_int16_t variable)
{
    ACC += readSignedValueFromFile(0, variable);
    PC += 2;
}

void Assembler::subI(u_int16_t variable)
{
    ACC -= readSignedValueFromFile(0, variable);
    PC += 2;
}

void Assembler::multI(u_int16_t variable)
{
    ACC *= readSignedValueFromFile(0, variable);
    PC += 2;
}

void Assembler::divI(u_int16_t variable)
{
    ACC /= readSignedValueFromFile(0, variable);
    PC += 2;
}

void Assembler::storeI(u_int16_t variable)
{
    writeValueToFile(0, variable, ACC); // 0 -> MEMdata
    PC += 2;
}

void Assembler::outputI(u_int16_t variable)
{
    if (mode == "debug")
        std::cout << findKeyByValue(variable) << " = " << readSignedValueFromFile(0, variable) << std::endl;
    else
        std::cout << readSignedValueFromFile(0, variable) << std::endl;
    PC += 2;
}

void Assembler::jumpZI(u_int16_t addr)
{
    if (ACC == 0)
    {
        PC = addr;
    }
    else
        PC += 2;
}

void Assembler::jumpPI(u_int16_t addr)
{
    if (ACC > 0x00)
    {
        PC = addr;
    }
    else
        PC += 2;
}

void Assembler::jumpNI(u_int16_t addr)
{
    if (ACC < 0)
    {
        PC = addr;
    }
    else
        PC += 2;
}

void Assembler::jumpI(u_int16_t addr)
{
    PC = addr;
}

void Assembler::copyI(u_int16_t source, u_int16_t dest)
{
    writeValueToFile(0, dest, readSignedValueFromFile(0, source)); // 0 -> MEMdata
    PC += 3;
}

void Assembler::stopI()
{
    std::cout << "\nProgram execution stopped." << std::endl;
}

void Assembler::throwI()
{

    std::cerr << "Exception thrown at address " << PC << std::endl; // THROW
    throw std::runtime_error("Exception thrown by the program.");
}

std::string Assembler::findKeyByValue(u_int16_t &addr)
{
    for (const auto &pair : labels)
    {
        if (pair.second == addr)
        {
            return pair.first;
        }
    }
    return "ACC"; // Return ACC if no label is found
}

u_int16_t Assembler::readValueFromFile(u_int16_t type, u_int16_t index)
{
    u_int16_t value = 0;
    std::string binaryString;

    // Calculate the position in the file, assuming each line is 17 characters (16 bits + newline)
    std::streampos pos = index * (elementSize + 1); // +1 for the newline character

    switch (type)
    {
    case 0:
        MEMdata->seekg(pos);
        std::getline(*MEMdata, binaryString); // Read the line into the string
        break;

    case 1:
        MEMtext->seekg(pos);
        std::getline(*MEMtext, binaryString); // Read the line into the string
        break;
    }

    // Convert the binary string back to a uint16_t value
    if (!binaryString.empty())
    {
        value = static_cast<u_int16_t>(std::bitset<16>(binaryString).to_ulong());
    }

    return value;
}

int16_t Assembler::readSignedValueFromFile(u_int16_t type, u_int16_t index)
{
    int16_t value = 0;
    std::string binaryString;

    // Calculate the position in the file, assuming each line is 17 characters (16 bits + newline)
    std::streampos pos = index * (elementSize + 1); // +1 for the newline character

    switch (type)
    {
    case 0:
        MEMdata->seekg(pos);
        std::getline(*MEMdata, binaryString); // Read the line into the string
        break;

    case 1:
        MEMtext->seekg(pos);
        std::getline(*MEMtext, binaryString); // Read the line into the string
        break;
    }

    // Convert the binary string back to a int16_t value
    if (!binaryString.empty())
    {
        value = static_cast<int16_t>(std::bitset<16>(binaryString).to_ulong());
    }

    return value;
}

void Assembler::writeValueToFile(u_int16_t type, u_int16_t index, u_int16_t value)
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
