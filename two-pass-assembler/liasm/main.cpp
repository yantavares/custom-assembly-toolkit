#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "FirstPassAnalyzer.hpp"
#include "Assembler.hpp"
#include "SecondPassAnalyzer.hpp"

int main(int argc, char *argv[])
{
    std::string MODE = "normal"; // Default mode.
                                 // Can be set to 'debug' or 'release' using command line arguments

    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <program_file>" << std::endl;
        std::cerr << "You can also set the mode by passing the mode as the second argument." << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
        std::cerr << "Could not open the file." << std::endl;
        return 1;
    }

    if (argc == 3)
    {
        MODE = argv[2];
        if (MODE != "release" && MODE != "debug")
        {
            std::cerr << "Invalid mode. Please use 'release' or 'debug'." << std::endl;
            return 1;
        }
        else
        {
            std::cout << "Running in " << MODE << " mode . . .\n"
                      << std::endl;
        }
    }

    std::unordered_map<std::string, u_int16_t> labels; // Labels

    try
    {
        FirstPassAnalyzer analyzer(labels);
        analyzer.analyse(file);
        if (MODE == "debug")
            analyzer.printLabels();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << " FIRST PASS ERROR!" << '\n';
        return 1;
    }
    // Makes sure to reset the file to the beginning
    file.clear();
    file.seekg(0, std::ios::beg);

    file.open(argv[1]);

    try
    {
        SecondPassAnalyzer analyzer2(labels);
        analyzer2.analyse(file);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << " SECOND PASS ERROR!" << '\n';
        return 1;
    }

    file.close();

    try
    {
        Assembler executor(labels, MODE);
        executor.execute();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << " RUNTIME ERROR!" << '\n';
        return 1;
    }

    return 0;
}
