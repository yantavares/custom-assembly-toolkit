#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <cstdint>

class File
{
private:
  std::vector<int32_t> object;
  std::string output_filename;
  bool stop;
  int address;
  uint32_t DataInit;

  std::vector<int> constante;
  std::vector<uint> variable;
  std::vector<int> jmp_address;

  void AppendIOFunctions(std::ofstream &file);

public:
  void ReadFile(std::string file);

  void GetJumps();
  void Get_Const();
  void WriteFile();
  void Get_Data();

  std::vector<std::string> Instructions();

  std::vector<std::string> Write_ADDSUB(int opc);
  std::vector<std::string> Write_MUL();
  std::vector<std::string> Write_DIV();
  std::vector<std::string> Write_JMP();
  std::vector<std::string> Write_LOADSTORE(int opc);
  std::vector<std::string> Write_COPY();
  std::vector<std::string> Write_STOP();
  std::vector<std::string> Write_Output();
  std::vector<std::string> Write_Input();
  std::vector<std::string> Write_S_INPUT();
  std::vector<std::string> Write_S_OUTPUT();

  std::vector<std::string> Write_Const();
  std::vector<std::string> Write_Variable();
  std::vector<std::string> Write_JMP_ADDRESS();
};

enum opcodes
{
  ADD = 1,
  SUB = 2,
  MUL = 3,
  DIV = 4,
  JMP = 5,
  JMPN = 6,
  JMPP = 7,
  JMPZ = 8,
  COPY = 9,
  LOAD = 10,
  STORE = 11,
  INPUT = 12,
  OUTPUT = 13,
  STOP = 14,
  S_INPUT = 15,
  S_OUTPUT = 16
};