#include <iostream>
#include <vector>

#include "FileUtils.h"
#include "Test.h"

void test_readHackFile(TestContext& cx) {
  std::vector<uint16_t> maxRom = {
    0x0000, 0xfc10, 0x0001, 0xf4d0,
    0x000a, 0xe301, 0x0001, 0xfc10,
    0x000c, 0xea87, 0x0000, 0xfc10,
    0x0002, 0xe308, 0x000e, 0xea87
  };

  auto actualRom = FileUtils::readHackFile("../test_programs/asm/Max.hack");

  cx.expectEqual(actualRom, maxRom);
}

int main() {
  TestContext cx;

  cx.test("FileUtils::readHackFile", test_readHackFile);

  cx.printResults();
}