#pragma once

#include <vector>
#include <string>

class FileUtils {
public:
  static std::vector<uint16_t> readHackFile(const std::string& filename);
  static std::vector<uint16_t> readHackFile(const std::wstring& filename);
  static std::string disassemble(uint16_t instr);

private:
  static std::string disassembleDest(uint16_t instr);
  static std::string disassembleComp(uint16_t instr);
  static std::string disassembleJump(uint16_t instr);
};
