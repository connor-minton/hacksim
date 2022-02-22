#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "Exceptions.h"

class FileUtils {
public:
  static std::vector<uint16_t> readHackFile(const std::string& filename);
};

std::vector<uint16_t>
FileUtils::readHackFile(const std::string& filename) {
  std::ifstream fin(filename);

  if (!fin) {
    std::string err = "Could not open file '" + filename + "' for input";
    throw Error(err.c_str());
  }

  std::vector<uint16_t> rom;
  std::string word;

  while (fin >> word) {
    if (word.size() != 16) {
      throw Error("Not all instructions are 16 bits in the input file");
    }
    for (char c : word) {
      if (c != '0' && c != '1') {
        throw Error("There is an instruction without only zeros and ones");
      }
    }

    rom.push_back((uint16_t)std::stoul(word, nullptr, 2));
  }

  return rom;
}
