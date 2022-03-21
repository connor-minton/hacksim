#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <bitset>

#include "Exceptions.h"
#include "Bits.h"
#include "StringUtils.h"

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

std::vector<uint16_t>
FileUtils::readHackFile(const std::string& filename) {
  return readHackFile(StringUtils::asciiToUTF16(filename));
}

std::vector<uint16_t> 
FileUtils::readHackFile(const std::wstring& filename) {
  std::ifstream fin(filename);

  if (!fin) {
    throw Error("Could not open file for input");
  }

  std::vector<uint16_t> rom;
  std::string word;

  while (fin >> word) {
    if (rom.size() == 32768) {
      throw Error("The input program is too large for the 32K ROM");
    }
    if (word.size() != 16) {
      throw Error("Not all instructions are 16 bits in the input file");
    }
    for (char c : word) {
      if (c != '0' && c != '1') {
        throw Error("There is an instruction with characters other than '0' and '1'");
      }
    }

    rom.push_back((uint16_t)std::stoul(word, nullptr, 2));
  }

  return rom;
}

std::string FileUtils::disassemble(uint16_t instr) {
  std::string out;
  std::bitset<16> bits(instr);

  // C-instruction
  if (bits[15]) {
    out = disassembleDest(instr) + disassembleComp(instr)
      + disassembleJump(instr);
  }

  // A-instruction
  else {
    out = "@" + std::to_string(instr);
  }

  return out;
}

std::string FileUtils::disassembleDest(uint16_t instr) {
  std::string out;

  bool a = getBit<5>(instr);
  bool d = getBit<4>(instr);
  bool m = getBit<3>(instr);

  if (a) out += 'A';
  if (m) out += 'M';
  if (d) out += 'D';

  if (a || m || d)
    out += '=';

  return out;
}

std::string FileUtils::disassembleComp(uint16_t instr) {
  uint16_t comp = (instr & 0x1fc0) >> 6;
  bool useM = getBit<12>(instr);

  switch (comp & 0x3f) {
  case 0x2a: // 101010
    return "0";
  case 0x3f: // 111111
    return "1";
  case 0x3a: // 111010
    return "-1";
  case 0x0c: // 001100
    return "D";
  case 0x30: // 110000
    return (useM) ? "M" : "A";
  case 0x0d: // 001101
    return "!D";
  case 0x31: // 110001
    return (useM) ? "!M" : "!A";
  case 0x0f: // 001111
    return "-D";
  case 0x33: // 110011
    return (useM) ? "-M" : "-A";
  case 0x1f: // 011111
    return "D+1";
  case 0x37: // 110111
    return (useM) ? "M+1" : "A+1";
  case 0x0e: // 001110
    return "D-1";
  case 0x32: // 110010
    return (useM) ? "M-1" : "A-1";
  case 0x02: // 000010
    return (useM) ? "D+M" : "D+A";
  case 0x13: // 010011
    return (useM) ? "D-M" : "D-A";
  case 0x07: // 000111
    return (useM) ? "M-D" : "A-D";
  case 0x00: // 000000
    return (useM) ? "D&M" : "D&A";
  case 0x15: // 010101
    return (useM) ? "D|M" : "D|A";
  default:
    return "???";
  }
}

std::string FileUtils::disassembleJump(uint16_t instr) {
  uint16_t jump = instr & 0x7;

  switch (jump) {
  case 1: return ";JGT";
  case 2: return ";JEQ";
  case 3: return ";JGE";
  case 4: return ";JLT";
  case 5: return ";JNE";
  case 6: return ";JLE";
  case 7: return ";JMP";
  default: return "";
  }
}