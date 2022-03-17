#pragma once

#include <string>

class StringUtils {
public:
  static std::wstring asciiToUTF16(const std::string & s);
};

std::wstring StringUtils::asciiToUTF16(const std::string & s) {
  std::wstring ws;
  for (char c : s) {
    ws += static_cast<wchar_t>(c);
  }
  return ws;
}
