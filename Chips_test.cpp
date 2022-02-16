#include <iostream>
#include <iomanip>

#include "Chips.h"

int testNum = 1;
int successCt = 0;
int failedCt = 0;

template <typename T>
bool expectEqual(const T& actual, const T& theoretical);

void test_Nand() {
  Nand n1;
  n1.a = true;
  n1.b = false;
  n1.computeOutput();
  expectEqual(n1.out, true);

  n1.a = false;
  n1.b = false;
  n1.computeOutput();
  expectEqual(n1.out, true);

  n1.a = false;
  n1.b = true;
  n1.computeOutput();
  expectEqual(n1.out, true);

  n1.a = true;
  n1.b = true;
  n1.computeOutput();
  expectEqual(n1.out, false);
}

void test_Not() {
  Not n;
  n.in = true;
  n.computeOutput();
  expectEqual(n.out, false);

  n.in = false;
  n.computeOutput();
  expectEqual(n.out, true);
}

void test_And() {
  And chip;
  chip.a = true;
  chip.b = false;
  chip.computeOutput();
  expectEqual(chip.out, false);

  chip.a = false;
  chip.b = false;
  chip.computeOutput();
  expectEqual(chip.out, false);

  chip.a = false;
  chip.b = true;
  chip.computeOutput();
  expectEqual(chip.out, false);

  chip.a = true;
  chip.b = true;
  chip.computeOutput();
  expectEqual(chip.out, true);
}

void test_Or() {
  Or chip;
  chip.a = true;
  chip.b = false;
  chip.computeOutput();
  expectEqual(chip.out, true);

  chip.a = false;
  chip.b = false;
  chip.computeOutput();
  expectEqual(chip.out, false);

  chip.a = false;
  chip.b = true;
  chip.computeOutput();
  expectEqual(chip.out, true);

  chip.a = true;
  chip.b = true;
  chip.computeOutput();
  expectEqual(chip.out, true);
}

void test_Xor() {
  Xor chip;
  chip.a = true;
  chip.b = false;
  chip.computeOutput();
  expectEqual(chip.out, true);

  chip.a = false;
  chip.b = false;
  chip.computeOutput();
  expectEqual(chip.out, false);

  chip.a = false;
  chip.b = true;
  chip.computeOutput();
  expectEqual(chip.out, true);

  chip.a = true;
  chip.b = true;
  chip.computeOutput();
  expectEqual(chip.out, false);
}

void test_Mux() {
  Mux chip;
  chip.a = false;
  chip.b = false;

  chip.sel = false;
  chip.computeOutput();
  expectEqual(chip.out, false);
  chip.sel = true;
  chip.computeOutput();
  expectEqual(chip.out, false);

  chip.a = true;
  chip.b = false;

  chip.sel = false;
  chip.computeOutput();
  expectEqual(chip.out, true);
  chip.sel = true;
  chip.computeOutput();
  expectEqual(chip.out, false);

  chip.a = false;
  chip.b = true;

  chip.sel = false;
  chip.computeOutput();
  expectEqual(chip.out, false);
  chip.sel = true;
  chip.computeOutput();
  expectEqual(chip.out, true);

  chip.a = true;
  chip.b = true;

  chip.sel = false;
  chip.computeOutput();
  expectEqual(chip.out, true);
  chip.sel = true;
  chip.computeOutput();
  expectEqual(chip.out, true);
}

void test_DMux() {
  DMux chip;

  chip.in = true;

  chip.sel = false;
  chip.computeOutput();
  expectEqual(chip.a, true);
  expectEqual(chip.b, false);

  chip.sel = true;
  chip.computeOutput();
  expectEqual(chip.a, false);
  expectEqual(chip.b, true);

  chip.in = false;

  chip.sel = false;
  chip.computeOutput();
  expectEqual(chip.a, false);
  expectEqual(chip.b, false);

  chip.sel = true;
  chip.computeOutput();
  expectEqual(chip.a, false);
  expectEqual(chip.b, false);
}

int main() {
  test_Nand();
  test_Not();
  test_And();
  test_Or();
  test_Xor();
  test_Mux();
  test_DMux();

  std::cout << "===================================\n"
            << "TESTS FAILED:    " << std::setw(5) << failedCt << '\n'
            << "TESTS PERFORMED: " << std::setw(5) << (failedCt + successCt) << '\n'
            << "===================================\n";
}

template <typename T>
bool expectEqual(const T& actual, const T& theoretical) {
  bool result = true;
  if (theoretical != actual) {
    std::cout << "FAILED [" << testNum << "]: expected "
              << theoretical << ", got " << actual << '\n';
    result = false;
    failedCt++;
  }
  else {
    successCt++;
  }
  testNum++;
  return result;
}
