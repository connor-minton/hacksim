#include "AppConfig.h"

#include <fstream>
#include <unordered_map>

#include "Exceptions.h"
#include "StringUtils.h"

bool AppConfig::m_shallowMemory = false;
bool AppConfig::m_shallowCPU = false;

void AppConfig::initializeFromFile(const std::wstring& filename) {
  std::ifstream fin(filename);
  if (!fin) {
    throw Error("Could not open config file.");
  }

  std::string line;
  int linenum = 0;
  std::unordered_map<std::string, std::string> keyValues;

  while (std::getline(fin, line)) {
    linenum++;
    std::string trimmed = StringUtils::trimWhitespaceAndComments(line);
    if (trimmed.size() == 0) {
      // skip empty lines and lines containing only comments
      continue;
    }

    // an important line must have an equals sign
    auto fields = StringUtils::split(trimmed, '=');
    if (fields.size() != 2) {
      throw Error("Error in config file at line " + std::to_string(linenum) + ".");
    }
    fields[0] = StringUtils::trimWhitespaceAndComments(fields[0]);
    fields[1] = StringUtils::trimWhitespaceAndComments(fields[1]);

    keyValues[fields[0]] = fields[1];
  }

  initialize(keyValues);
}

void AppConfig::initialize(const std::unordered_map<std::string, std::string> & keyValues) {
  std::unordered_map<std::string,std::string>::const_iterator
    it;

  it = keyValues.find("shallowMemory");
  if (it != keyValues.cend() && it->second == "true") {
    m_shallowMemory = true;
  }

  it = keyValues.find("shallowCPU");
  if (it != keyValues.cend() && it->second == "true") {
    m_shallowCPU = true;
  }
}