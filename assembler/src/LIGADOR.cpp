#include "LIGADOR.hpp"

Linker::Linker() : correctionFactor(0) {}

void Linker::linkFiles(const std::string &file1, const std::string &file2)
{
    std::ofstream outputStream(file1.substr(0, file1.size() - 2) + "_linked.obj");

    std::ifstream file1Stream(file1);
    std::ifstream file2Stream(file2);

    if (!file1Stream.is_open() || !file2Stream.is_open() || !outputStream.is_open())
    {
        std::cerr << "Erro ao abrir os arquivos" << std::endl;
        return;
    }

    generateTables(file1Stream, usageTable1, definitionTable1, relocationTable1, code1);
    generateTables(file2Stream, usageTable2, definitionTable2, relocationTable2, code2);

    correctionFactor = code1.size();

    applyCorrectionFactor(code2, relocationTable2);

    for (auto &[key, value] : usageTable1)
    {
        for (auto &v : value)
        {
            if (relocationTable1[v - 1] == 1)
            {
                code1[v - 1] = definitionTable2[key] + correctionFactor;
            }
            else
            {
                code1[v - 1] = definitionTable2[key];
            }
        }
    }

    for (auto &[key, value] : usageTable2)
    {
        for (auto &v : value)
        {
            code2[v - 1] = definitionTable1[key];
        }
    }

    for (int i = 0; i < code1.size(); i++)
    {
        outputStream << code1[i] << " ";
    }

    for (int i = 0; i < code2.size(); i++)
    {
        outputStream << code2[i] << " ";
    }

    file1Stream.close();
    file2Stream.close();
    outputStream.close();
}

void Linker::generateTables(std::ifstream &file, UsageTable &ut, DefinitionTable &dt, RelocationTable &rt, Code &c)
{
    std::string line;
    std::string section;
    while (std::getline(file, line))
    {
        if (line == "USO")
        {
            section = "USO";
            continue;
        }
        else if (line == "DEF")
        {
            section = "DEF";
            continue;
        }
        else if (line == "REAL")
        {
            section = "REAL";
            continue;
        }
        else if (section == "REAL" && (line.empty() || line == " "))
        {
            section = "CODE";
            continue;
        }

        if (line.empty())
            continue;

        if (section == "USO")
        {
            std::istringstream iss(line);
            std::string label;
            int value;
            iss >> label >> value;
            ut[label].push_back(static_cast<int>(value));
        }
        else if (section == "DEF")
        {
            std::istringstream iss(line);
            std::string label;
            int value;
            iss >> label >> value;
            dt[label] = static_cast<int>(value);
        }
        else if (section == "REAL")
        {
            for (char ch : line)
            {
                if (isdigit(ch))
                {
                    rt.push_back(ch - '0');
                }
            }
        }
        else if (section == "CODE")
        {
            int value;
            std::istringstream iss(line);
            while (iss >> value)
            {
                c.push_back(static_cast<int>(value));
            }
        }
        else
        {
            std::cerr << "Invalid section" << std::endl;
        }
    }
}

void Linker::applyCorrectionFactor(Code &c, RelocationTable &rt)
{
    for (int i = 0; i < c.size(); i++)
    {
        if (rt[i] == 1)
            c[i] += correctionFactor;
    }
}
