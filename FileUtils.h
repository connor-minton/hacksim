#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "Exceptions.h"

static std::vector<uint16_t> readHackFile(std::string filename) {
  std::ifstream fin(filename);

  if (!fin) {
    std::string err = "Could not open file '" + filename + "' for input";
    throw Error(err.c_str());
  }

  return std::vector<uint16_t>();
}