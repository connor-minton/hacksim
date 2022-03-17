#include <iostream>

#include "StringUtils.h"

int main() {
  std::string s = "Test String";

  std::wcout << "Should see 'Test String': " << StringUtils::asciiToUTF16(s) << std::endl;
}
