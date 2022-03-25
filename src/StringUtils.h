#pragma once

#include <cassert>
#include <string>
#include <vector>

class StringUtils {
public:
  static std::wstring asciiToUTF16(const std::string & s);
  static std::string trimWhitespaceAndComments(const std::string& s);
  static std::vector<std::string> split(const std::string& s, char delim);
};

inline std::wstring StringUtils::asciiToUTF16(const std::string & s) {
  std::wstring ws;
  for (char c : s) {
    ws += static_cast<wchar_t>(c);
  }
  return ws;
}

inline std::string StringUtils::trimWhitespaceAndComments(const std::string& s) {
  std::string noComments;
  std::string ws = " \t\r\n";
  int firstRealChar = -1, lastRealChar = -1;

  size_t commentIdx = s.find("#");

  if (commentIdx != std::string::npos) {
    noComments = s.substr(0, commentIdx);
  }
  else {
    noComments = s;
  }

  for (int i = 0; i < noComments.size(); i++) {
    if (ws.find(noComments[i]) == std::string::npos) {
      if (firstRealChar < 0)
        firstRealChar = i;
      lastRealChar = i;
    }
  }

  if (firstRealChar >= 0 && firstRealChar != std::string::npos) {
    assert(lastRealChar >= 0 && lastRealChar != std::string::npos);
    return noComments.substr(firstRealChar, lastRealChar - firstRealChar + 1);
  }
  return "";
}

inline std::vector<std::string> StringUtils::split(const std::string& s, char delim) {
  std::vector<std::string> out;
  if (s.empty())
    return out;

  out.push_back("");
  for (size_t i = 0; i < s.size(); i++) {
    if (s[i] == delim) {
      out.push_back("");
    }
    else {
      out[out.size() - 1] += s[i];
    }
  }

  return out;
}
