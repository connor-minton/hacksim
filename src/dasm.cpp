/**
 * dasm
 *
 * Disassembles a Hack ROM. Dumps each instruction in assembly language to stdout.
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#include "FileUtils.h"
#include "Exceptions.h"

const char* USAGE = "usage: dasm <hackfile>";

void die(std::string err) {
  std::cerr << err << '\n';
  exit(1);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    die(USAGE);
  }

  try {
    std::vector<uint16_t> rom = FileUtils::readHackFile(std::string(argv[1]));
    for (auto instr : rom) {
      std::cout << FileUtils::disassemble(instr) << '\n';
    }
  }
  catch (Error& e) {
    die("error: " + std::string(e.what()));
  }
}