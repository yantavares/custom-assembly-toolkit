#include "Tradutor.hpp"

void File::ReadFile(std::string filename)
{
  stop = 0;
  std::string line;
  std::string word;
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::invalid_argument("File not found");
  }

  while (getline(file, line))
  {
    if (line.empty())
    {
      continue;
    }

    std::istringstream iss(line);
    while (iss >> word)
    {
      object.push_back(std::stoi(word));
    }
  }

  std::string output_dir = "output/";

  if (filename.size() > 9 && filename.substr(0, 9) == "arquivos/")
  {
    filename = filename.substr(9);
  }

  Get_Data();

  output_filename = output_dir + filename.substr(0, filename.size() - 3) + "s";
}

void File::GetJumps()
{
  int size = 0;
  while (size < DataInit)
  {
    bool neww = true;

    if (object[size] <= JMPZ && object[size] >= JMP)
    {
      for (int i = 0; i < jmp_address.size(); i++)
      {
        if (object[size + 1] == jmp_address[i])
        {
          neww = false;
          break;
        }
      }
      if (neww)
      {
        jmp_address.push_back(object[size + 1]);
      }
    }
    else if (object[size] == COPY || object[size] == S_INPUT || object[size] == S_OUTPUT)
    {
      size++;
    }
    else if (object[size] == STOP)
    {
      size--;
    }
    size = size + 2;
  }
}

std::vector<std::string> File::Write_JMP_ADDRESS()
{
  std::vector<std::string> write;
  bool is_jump = false;
  for (int i = 0; i < jmp_address.size(); i++)
  {
    if (address == jmp_address[i])
    {
      is_jump = true;
      break;
    }
  }
  if (is_jump)
  {
    write.push_back("jmp");
    write.push_back(std::to_string(address));
    write.push_back(":");
    write.push_back("\n");
  }
  return write;
}

std::vector<std::string> File::Instructions()
{

  std::vector<std::string> command;

  switch (object[address])
  {
  case ADD:
  case SUB:
    command = Write_ADDSUB(object[address]);
    break;
  case MUL:
    command = Write_MUL();
    break;
  case DIV:
    command = Write_DIV();
    break;
  case JMP:
  case JMPN:
  case JMPP:
  case JMPZ:
    command = Write_JMP();
    break;
  case COPY:
    command = Write_COPY();
    break;
  case LOAD:
  case STORE:
    command = Write_LOADSTORE(object[address]);
    break;
  case INPUT:
    command = Write_Input();
    break;
  case OUTPUT:
    command = Write_Output();
    break;
  case STOP:
    command = Write_STOP();
    break;
  case S_INPUT:
    command = Write_S_INPUT();
    break;
  case S_OUTPUT:
    command = Write_S_OUTPUT();
    break;

  default:
    break;
  }
  return command;
}

void File::Get_Const()
{
  while (DataInit < object.size())
  {
    if (object[DataInit] == 0)
    {
      variable.push_back(DataInit);
    }
    else
    {

      constante.push_back(object[DataInit]);
      constante.push_back(DataInit);
    }
    DataInit++;
  }
}

void File::WriteFile()
{
  std::vector<std::string> command;
  std::vector<std::string> is_jump;
  std::ofstream file(output_filename);
  address = 0;
  if (!file.is_open())
  {
    std::cerr << "Unable to open file for writing: " << output_filename << std::endl;
    return;
  }

  AppendIOFunctions(file);

  file << "	global _start" << '\n';
  file << "_start:" << '\n';
  while (!stop && (address < object.size()))
  {
    is_jump = Write_JMP_ADDRESS();
    for (const std::string &i : is_jump)
    {
      file << i;
    }

    command = Instructions();
    if (object[address] == COPY || object[address] == S_INPUT || object[address] == S_OUTPUT)
    {
      address = address + 3;
    }
    else if (object[address] == STOP)
    {
      address++;
    }
    else
    {
      address = address + 2;
    }
    for (const std::string &i : command)
    {
      file << i;
    }
  }

  Get_Const();
  command = Write_Const();
  for (const std::string &i : command)
  {
    file << i;
  }

  command = Write_Variable();
  for (const std::string &i : command)
  {
    file << i;
  }

  file.close();
}

std::vector<std::string> File::Write_ADDSUB(int opc)
{
  std::vector<std::string> command;
  std::string addr;
  if (opc == ADD)
  {
    command.push_back("	add ");
  }
  else
  {
    command.push_back("	sub ");
  }

  addr = "a" + std::to_string(object[address + 1]);
  command.push_back("ebx, ");
  command.push_back("[");
  command.push_back(addr);
  command.push_back("]");
  command.push_back("\n");

  return command;
}

std::vector<std::string> File::Write_LOADSTORE(int opc)
{
  std::vector<std::string> command;
  std::string addr;

  command.push_back("	mov ");
  addr = "a" + std::to_string(object[address + 1]);
  if (opc == LOAD)
  {
    command.push_back("ebx, ");
    command.push_back("[");
    command.push_back(addr);
    command.push_back("]");
    command.push_back("\n");
  }
  else
  {
    command.push_back("[");
    command.push_back(addr);
    command.push_back("], ");
    command.push_back("ebx");
    command.push_back("\n");
  }
  return command;
}

std::vector<std::string> File::Write_COPY()
{
  std::string addr;
  std::vector<std::string> command;

  command.push_back("	push ebx");
  command.push_back("\n");

  addr = "a" + std::to_string(object[address + 1]);
  command.push_back("	mov ebx, [");
  command.push_back(addr);
  command.push_back("]");
  command.push_back("\n");

  addr = "a" + std::to_string(object[address + 2]);
  command.push_back("	mov [");
  command.push_back(addr);
  command.push_back("], ");
  command.push_back("ebx");
  command.push_back("\n");

  command.push_back("	pop ebx");
  command.push_back("\n");

  return command;
}

std::vector<std::string> File::Write_STOP()
{
  stop = true;
  std::vector<std::string> command;

  command.push_back("	mov eax, 1");
  command.push_back("\n");
  command.push_back("	mov ebx, 0");
  command.push_back("\n");
  command.push_back("	int 80h");
  command.push_back("\n");

  return command;
}

std::vector<std::string> File::Write_JMP()
{
  std::vector<std::string> command;
  std::string addr;
  command.push_back("	cmp ebx, 0");
  command.push_back("\n");
  switch (object[address])
  {
  case JMP:
    command.push_back("	jmp ");
    break;

  case JMPN:
    command.push_back("	jl ");
    break;

  case JMPP:
    command.push_back("	jg ");
    break;

  case JMPZ:
    command.push_back("	je ");
  default:
    break;
  }
  addr = "jmp" + std::to_string(object[address + 1]);
  command.push_back(addr);
  command.push_back("\n");

  return command;
}

std::vector<std::string> File::Write_MUL()
{
  std::vector<std::string> command;
  std::string addr;
  command.push_back("	imul ebx, [");

  addr = "a" + std::to_string(object[address + 1]);
  command.push_back(addr);
  command.push_back("]");
  command.push_back("\n");
  command.push_back("	jo OVERFLOW");
  command.push_back("\n");

  return command;
}

std::vector<std::string> File::Write_DIV()
{
  std::vector<std::string> command;
  std::string addr;
  command.push_back("	push edx");
  command.push_back("\n");
  command.push_back("	push ebx");
  command.push_back("\n");

  addr = "a" + std::to_string(object[address + 1]);
  command.push_back("	mov ebx, [");
  command.push_back(addr);
  command.push_back("]");
  command.push_back("\n");
  command.push_back("	cdq");
  command.push_back("\n");
  command.push_back("	idiv ebx");

  command.push_back("\n");
  command.push_back("	pop ebx");
  command.push_back("\n");
  command.push_back("	pop edx");
  command.push_back("\n");
  return command;
}

std::vector<std::string> File::Write_Const()
{
  int i = 0;
  std::vector<std::string> command;
  std::string addr;

  command.push_back("section .data");
  command.push_back("\n");
  command.push_back("  read_msg db 0, 'Quantidade de Bytes lidos: ', 0\n");
  command.push_back("  len_read_msg equ $-read_msg\n");
  command.push_back("  written_msg db 0, 'Quantidade de Bytes escritos: ', 0\n");
  command.push_back("  len_written_msg equ $-written_msg\n");
  command.push_back("  newline db 0xa, 0\n");
  command.push_back("  minus_str db '-', 0\n");
  command.push_back("  output_overflow db 'Deu Overflow', 0\n");
  command.push_back("  len_overflow equ $-output_overflow\n");
  while (i < constante.size())
  {
    addr = "a" + std::to_string(constante[i + 1]);
    command.push_back(addr);
    command.push_back(" dd ");
    command.push_back(std::to_string(constante[i]));
    command.push_back("\n");
    i = i + 2;
  }

  return command;
}

std::vector<std::string> File::Write_Input()
{
  std::vector<std::string> command;
  std::string label_addr = "a" + std::to_string(object[address + 1]);

  command.push_back("    push ebx\n");
  command.push_back("    push input_buffer\n");
  command.push_back("    push " + label_addr + "\n");
  command.push_back("    call input\n");
  command.push_back("    pop edx\n");
  command.push_back("    pop edx\n");
  command.push_back("    pop ebx\n");

  return command;
}

std::vector<std::string> File::Write_Output()
{
  std::vector<std::string> command;
  std::string label_addr = "a" + std::to_string(object[address + 1]);

  command.push_back("    push ebx\n");
  command.push_back("    push buffer\n");
  command.push_back("    push " + label_addr + "\n");
  command.push_back("    call output\n");
  command.push_back("    pop edx\n");
  command.push_back("    pop edx\n");
  command.push_back("    pop ebx\n");

  return command;
}

std::vector<std::string> File::Write_S_INPUT()
{
  std::vector<std::string> command;
  std::string label_addr = "a" + std::to_string(object[address + 1]);
  int num_chars = object[address + 2];

  command.push_back("    push ebx\n");
  command.push_back("    push " + std::to_string(num_chars) + "\n");
  command.push_back("    push " + label_addr + "\n");
  command.push_back("    call s_input\n");
  command.push_back("    pop edx\n");
  command.push_back("    pop edx\n");
  command.push_back("    pop ebx\n");

  return command;
}

std::vector<std::string> File::Write_S_OUTPUT()
{
  std::vector<std::string> command;
  std::string label_addr = "a" + std::to_string(object[address + 1]);
  int num_chars = object[address + 2];

  command.push_back("    push ebx\n");
  command.push_back("    push " + std::to_string(num_chars) + "\n");
  command.push_back("    push " + label_addr + "\n");
  command.push_back("    call s_output\n");
  command.push_back("    pop edx\n");
  command.push_back("    pop edx\n");
  command.push_back("    pop ebx\n");

  return command;
}

std::vector<std::string> File::Write_Variable()
{
  int i = 0;
  std::vector<std::string> command;
  std::string addr;

  command.push_back("section .bss\n");
  command.push_back("  buffer resb 11\n");
  command.push_back("  input_buffer resb 12\n");
  while (i < variable.size())
  {
    addr = "a" + std::to_string(variable[i]);
    command.push_back(addr);
    command.push_back(" resd 1\n");
    i++;
  }
  return command;
}

void File::AppendIOFunctions(std::ofstream &file)
{
  std::ifstream io_file("new_io.asm");
  if (!io_file.is_open())
  {
    std::cerr << "Unable to open io.asm file" << std::endl;
    return;
  }

  std::string line;
  while (getline(io_file, line))
  {
    file << line << '\n';
  }

  io_file.close();
}

void File::Get_Data()
{
  int size = 0;
  DataInit = 0xffffffff;
  while (size < DataInit)
  {
    bool neww = true;

    if ((object[size] < JMP || object[size] > JMPZ) && object[size] != STOP)
    {
      if (object[size + 1] < DataInit)
        DataInit = object[size + 1];
      if (object[size] == COPY)
      {
        if (object[size + 2] < DataInit)
          DataInit = object[size + 2];
        size++;
      }
    }

    else if (object[size] == STOP)
      size--;

    else if (object[size] <= JMPZ && object[size] >= JMP)
    {
      for (int i = 0; i < jmp_address.size(); i++)
      {
        if (object[size + 1] == jmp_address[i])
        {
          neww = false;
          break;
        }
      }
      if (neww)
      {
        jmp_address.push_back(object[size + 1]);
      }
    }
    size = size + 2;
  }
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    return 1;
  }

  std::string filename = argv[1];

  if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".obj")
    filename += ".obj";

  File R;
  R.ReadFile(filename);
  R.WriteFile();

  return 0;
}
