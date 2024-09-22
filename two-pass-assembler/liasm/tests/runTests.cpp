#include <iostream>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <vector>
#include <sys/wait.h>
#include <format>

// Define ANSI escape codes for colors
const char *RED = "\033[31m";
const char *GREEN = "\033[32m";
const char *RESET = "\033[0m";

std::vector<std::string> getTestFiles(const std::string &directory)
{
    std::vector<std::string> files;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(directory.c_str())) != nullptr)
    {
        while ((ent = readdir(dir)) != nullptr)
        {
            if (ent->d_type == DT_REG)
            { // Make sure it's a file and not a directory
                files.emplace_back(ent->d_name);
            }
        }
        closedir(dir);
    }
    else
    {
        std::perror("Could not open directory");
        exit(EXIT_FAILURE);
    }
    return files;
}

int main()
{
    const std::string directory = "./tests";
    const std::string commandPrefix = "make run FILE=";

    auto testFiles = getTestFiles(directory);
    for (const auto &file : testFiles)
    {
        if (file == "runTests.cpp")
        {
            continue;
        }
        std::string command = commandPrefix + directory + "/" + file;
        std::cout << "\nRunning: " << command << std::endl;

        int status = system(command.c_str());

        if (WIFEXITED(status))
        {
            int exitStatus = WEXITSTATUS(status);
            if (exitStatus == 0)
            {
                if (file.find("error") != std::string::npos)
                {
                    std::cerr << std::format("{}Error: {} ran without errors.{}\n", RED, file, RESET);
                    continue;
                }
                std::cout << std::format("{}Success: {} ran without errors.{}\n", GREEN, file, RESET);
            }
            else
            {
                if (file.find("error") != std::string::npos)
                {
                    std::cout << std::format("{}Success: {} exited with status {}.{}\n", GREEN, file, exitStatus, RESET);
                    continue;
                }
                std::cerr << std::format("{}Error: {} exited with status {}.{}\n", RED, file, exitStatus, RESET);
            }
        }
        else
        {
            std::cerr << std::format("{}Error running command for file: {}.{}\n", RED, file, RESET);
        }
    }
    return 0;
}
