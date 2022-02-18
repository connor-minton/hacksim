#include <iostream>
#include <iomanip>
#include <vector>

#include "Chips.h"

int testNum = 1;
int successCt = 0;
int failedCt = 0;

template <typename T>
bool expectEqual(const T& actual, const T& theoretical);

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T> v) {
  size_t len = v.size();
  out << '{';
  for (size_t i = 0; i < len; i++) {
    out << v[i];
    if (i < len - 1) {
      out << ',';
    }
  }
  out << '}';

  return out;
}

void test_Nand() {
  Nand n1;
  n1.set_a(true);
  n1.set_b(false);
  n1.computeOutput();
  expectEqual(n1.out(), true);

  n1.set_a(false);
  n1.set_b(false);
  n1.computeOutput();
  expectEqual(n1.out(), true);

  n1.set_a(false);
  n1.set_b(true);
  n1.computeOutput();
  expectEqual(n1.out(), true);

  n1.set_a(true);
  n1.set_b(true);
  n1.computeOutput();
  expectEqual(n1.out(), false);
}

void test_Not() {
  Not n;
  n.set_in(true);
  n.computeOutput();
  expectEqual(n.out(), false);

  n.set_in(false);
  n.computeOutput();
  expectEqual(n.out(), true);
}

void test_And() {
  And chip;
  chip.set_a(true);
  chip.set_b(false);
  chip.computeOutput();
  expectEqual(chip.out(), false);

  chip.set_a(false);
  chip.set_b(false);
  chip.computeOutput();
  expectEqual(chip.out(), false);

  chip.set_a(false);
  chip.set_b(true);
  chip.computeOutput();
  expectEqual(chip.out(), false);

  chip.set_a(true);
  chip.set_b(true);
  chip.computeOutput();
  expectEqual(chip.out(), true);
}

void test_Or() {
  Or chip;
  chip.set_a(true);
  chip.set_b(false);
  chip.computeOutput();
  expectEqual(chip.out(), true);

  chip.set_a(false);
  chip.set_b(false);
  chip.computeOutput();
  expectEqual(chip.out(), false);

  chip.set_a(false);
  chip.set_b(true);
  chip.computeOutput();
  expectEqual(chip.out(), true);

  chip.set_a(true);
  chip.set_b(true);
  chip.computeOutput();
  expectEqual(chip.out(), true);
}

void test_Xor() {
  Xor chip;
  chip.set_a(true);
  chip.set_b(false);
  chip.computeOutput();
  expectEqual(chip.out(), true);

  chip.set_a(false);
  chip.set_b(false);
  chip.computeOutput();
  expectEqual(chip.out(), false);

  chip.set_a(false);
  chip.set_b(true);
  chip.computeOutput();
  expectEqual(chip.out(), true);

  chip.set_a(true);
  chip.set_b(true);
  chip.computeOutput();
  expectEqual(chip.out(), false);
}

void test_Mux() {
  Mux chip;
  chip.set_a(false);
  chip.set_b(false);

  chip.set_sel(false);
  chip.computeOutput();
  expectEqual(chip.out(), false);
  chip.set_sel(true);
  chip.computeOutput();
  expectEqual(chip.out(), false);

  chip.set_a(true);
  chip.set_b(false);

  chip.set_sel(false);
  chip.computeOutput();
  expectEqual(chip.out(), true);
  chip.set_sel(true);
  chip.computeOutput();
  expectEqual(chip.out(), false);

  chip.set_a(false);
  chip.set_b(true);

  chip.set_sel(false);
  chip.computeOutput();
  expectEqual(chip.out(), false);
  chip.set_sel(true);
  chip.computeOutput();
  expectEqual(chip.out(), true);

  chip.set_a(true);
  chip.set_b(true);

  chip.set_sel(false);
  chip.computeOutput();
  expectEqual(chip.out(), true);
  chip.set_sel(true);
  chip.computeOutput();
  expectEqual(chip.out(), true);
}

void test_DMux() {
  DMux chip;

  chip.set_in(true);

  chip.set_sel(false);
  chip.computeOutput();
  expectEqual(chip.a(), true);
  expectEqual(chip.b(), false);

  chip.set_sel(true);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), true);

  chip.set_in(false);

  chip.set_sel(false);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);

  chip.set_sel(true);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
}

void test_Not16() {
  Not16 chip;
  std::vector<bool> in =  {0,0,1,0, 1,1,0,1, 0,0,0,1, 0,1,1,1};
  std::vector<bool> out = {1,1,0,1, 0,0,1,0, 1,1,1,0, 1,0,0,0};

  for (int i = 0; i < 16; i++) {
    chip.in[i] = in[i];
  }

  chip.computeOutput();
  expectEqual(std::vector<bool>(chip.out, chip.out + 16), out);
}

void test_And16() {
  And16 chip;
  std::vector<bool> a  =  {0,0,1,0, 1,1,0,1, 0,0,0,1, 0,1,1,1};
  std::vector<bool> b  =  {1,1,1,0, 1,0,0,0, 1,0,1,0, 1,1,0,1};
  std::vector<bool> out = {0,0,1,0, 1,0,0,0, 0,0,0,0, 0,1,0,1};

  for (int i = 0; i < 16; i++) {
    chip.a[i] = a[i];
    chip.b[i] = b[i];
  }

  chip.computeOutput();
  expectEqual(std::vector<bool>(chip.out, chip.out + 16), out);
}

void test_Or16() {
  Or16 chip;
  std::vector<bool> a  =  {0,0,1,0, 1,1,0,1, 0,0,0,1, 0,1,1,1};
  std::vector<bool> b  =  {1,1,1,0, 1,0,0,0, 1,0,1,0, 1,1,0,1};
  std::vector<bool> out = {1,1,1,0, 1,1,0,1, 1,0,1,1, 1,1,1,1};

  for (int i = 0; i < 16; i++) {
    chip.a[i] = a[i];
    chip.b[i] = b[i];
  }

  chip.computeOutput();
  expectEqual(std::vector<bool>(chip.out, chip.out + 16), out);
}

void test_Mux16() {
  Mux16 chip;
  std::vector<bool> a  =  {0,0,1,0, 1,1,0,1, 0,0,0,1, 0,1,1,1};
  std::vector<bool> b  =  {1,1,1,0, 1,0,0,0, 1,0,1,0, 1,1,0,1};

  for (int i = 0; i < 16; i++) {
    chip.a[i] = a[i];
    chip.b[i] = b[i];
  }

  chip.sel = false;
  chip.computeOutput();
  expectEqual(std::vector<bool>(chip.out, chip.out + 16), a);

  chip.sel = true;
  chip.computeOutput();
  expectEqual(std::vector<bool>(chip.out, chip.out + 16), b);
}

void test_Or8Way() {
  Or8Way chip;
  std::vector<bool> in1 = {0,0,0,0, 0,0,0,0};
  std::vector<bool> in2 = {0,1,0,0, 0,0,0,0};
  std::vector<bool> in3 = {1,0,0,1, 1,0,1,0};
  std::vector<bool> in4 = {1,1,1,1, 1,1,1,1};

  for (int i = 0; i < 16; i++) {
    chip.in[i] = in1[i];
  }
  chip.computeOutput();
  expectEqual(chip.out, false);

  for (int i = 0; i < 16; i++) {
    chip.in[i] = in2[i];
  }
  chip.computeOutput();
  expectEqual(chip.out, true);

  for (int i = 0; i < 16; i++) {
    chip.in[i] = in3[i];
  }
  chip.computeOutput();
  expectEqual(chip.out, true);

  for (int i = 0; i < 16; i++) {
    chip.in[i] = in4[i];
  }
  chip.computeOutput();
  expectEqual(chip.out, true);
}

void test_Mux4Way16() {
  Mux4Way16 chip;

  std::vector<bool> in1 = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
  std::vector<bool> in2 = {0,1,0,0, 0,0,0,0, 0,1,0,0, 0,0,0,0};
  std::vector<bool> in3 = {1,0,0,1, 1,0,1,0, 1,0,0,1, 1,0,1,0};
  std::vector<bool> in4 = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1};

  for (int i = 0; i < 16; i++) {
    chip.a[i] = in1[i];
    chip.b[i] = in2[i];
    chip.c[i] = in3[i];
    chip.d[i] = in4[i];
  }

  chip.sel[0] = false;
  chip.sel[1] = false;
  chip.computeOutput();
  expectEqual(std::vector<bool>(chip.out, chip.out+16), in1);

  chip.sel[0] = true;
  chip.sel[1] = false;
  chip.computeOutput();
  expectEqual(std::vector<bool>(chip.out, chip.out+16), in2);

  chip.sel[0] = false;
  chip.sel[1] = true;
  chip.computeOutput();
  expectEqual(std::vector<bool>(chip.out, chip.out+16), in3);

  chip.sel[0] = true;
  chip.sel[1] = true;
  chip.computeOutput();
  expectEqual(std::vector<bool>(chip.out, chip.out+16), in4);
}

void test_Mux8Way16() {
  Mux8Way16 chip;

  std::vector<bool> in1 = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
  std::vector<bool> in2 = {0,1,0,0, 0,0,0,0, 0,1,0,0, 0,0,0,0};
  std::vector<bool> in3 = {1,0,0,1, 1,0,1,0, 1,0,0,1, 1,0,1,0};
  std::vector<bool> in4 = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,0,1};
  std::vector<bool> in5 = {1,0,1,1, 1,1,0,1, 1,1,1,1, 1,1,1,1};
  std::vector<bool> in6 = {1,1,1,1, 1,1,0,1, 1,1,1,0, 1,0,1,1};
  std::vector<bool> in7 = {1,1,1,1, 1,1,1,1, 1,0,1,1, 1,1,1,1};
  std::vector<bool> in8 = {1,1,0,1, 1,0,1,1, 1,1,1,1, 0,1,1,1};

  for (int i = 0; i < 16; i++) {
    chip.a[i] = in1[i];
    chip.b[i] = in2[i];
    chip.c[i] = in3[i];
    chip.d[i] = in4[i];
    chip.e[i] = in5[i];
    chip.f[i] = in6[i];
    chip.g[i] = in7[i];
    chip.h[i] = in8[i];
  }

  chip.sel[0] = false;
  chip.sel[1] = false;
  chip.sel[2] = false;
  chip.computeOutput();
  expectEqual(std::vector<bool>(chip.out, chip.out+16), in1);

  chip.sel[0] = true;
  chip.sel[1] = false;
  chip.sel[2] = false;
  chip.computeOutput();
  expectEqual(std::vector<bool>(chip.out, chip.out+16), in2);

  chip.sel[0] = false;
  chip.sel[1] = true;
  chip.sel[2] = false;
  chip.computeOutput();
  expectEqual(std::vector<bool>(chip.out, chip.out+16), in3);

  chip.sel[0] = true;
  chip.sel[1] = true;
  chip.sel[2] = false;
  chip.computeOutput();
  expectEqual(std::vector<bool>(chip.out, chip.out+16), in4);

  chip.sel[0] = false;
  chip.sel[1] = false;
  chip.sel[2] = true;
  chip.computeOutput();
  expectEqual(std::vector<bool>(chip.out, chip.out+16), in5);

  chip.sel[0] = true;
  chip.sel[1] = false;
  chip.sel[2] = true;
  chip.computeOutput();
  expectEqual(std::vector<bool>(chip.out, chip.out+16), in6);

  chip.sel[0] = false;
  chip.sel[1] = true;
  chip.sel[2] = true;
  chip.computeOutput();
  expectEqual(std::vector<bool>(chip.out, chip.out+16), in7);

  chip.sel[0] = true;
  chip.sel[1] = true;
  chip.sel[2] = true;
  chip.computeOutput();
  expectEqual(std::vector<bool>(chip.out, chip.out+16), in8);
}

void test_DMux4Way() {
  DMux4Way chip;

  chip.in = false;

  chip.sel[0] = false;
  chip.sel[1] = false;
  chip.computeOutput();
  expectEqual(chip.a, false);
  expectEqual(chip.b, false);
  expectEqual(chip.c, false);
  expectEqual(chip.d, false);

  chip.sel[0] = true;
  chip.sel[1] = false;
  chip.computeOutput();
  expectEqual(chip.a, false);
  expectEqual(chip.b, false);
  expectEqual(chip.c, false);
  expectEqual(chip.d, false);

  chip.sel[0] = false;
  chip.sel[1] = true;
  chip.computeOutput();
  expectEqual(chip.a, false);
  expectEqual(chip.b, false);
  expectEqual(chip.c, false);
  expectEqual(chip.d, false);

  chip.sel[0] = true;
  chip.sel[1] = true;
  chip.computeOutput();
  expectEqual(chip.a, false);
  expectEqual(chip.b, false);
  expectEqual(chip.c, false);
  expectEqual(chip.d, false);

  chip.in = true;

  chip.sel[0] = false;
  chip.sel[1] = false;
  chip.computeOutput();
  expectEqual(chip.a, true);
  expectEqual(chip.b, false);
  expectEqual(chip.c, false);
  expectEqual(chip.d, false);

  chip.sel[0] = true;
  chip.sel[1] = false;
  chip.computeOutput();
  expectEqual(chip.a, false);
  expectEqual(chip.b, true);
  expectEqual(chip.c, false);
  expectEqual(chip.d, false);

  chip.sel[0] = false;
  chip.sel[1] = true;
  chip.computeOutput();
  expectEqual(chip.a, false);
  expectEqual(chip.b, false);
  expectEqual(chip.c, true);
  expectEqual(chip.d, false);

  chip.sel[0] = true;
  chip.sel[1] = true;
  chip.computeOutput();
  expectEqual(chip.a, false);
  expectEqual(chip.b, false);
  expectEqual(chip.c, false);
  expectEqual(chip.d, true);
}

void test_DMux8Way() {
  DMux8Way chip;

  chip.set_in(false);

  chip.set_sel(0, false);
  chip.set_sel(1, false);
  chip.set_sel(2, false);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(0, true);
  chip.set_sel(1, false);
  chip.set_sel(2, false);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(0, false);
  chip.set_sel(1, true);
  chip.set_sel(2, false);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(0, true);
  chip.set_sel(1, true);
  chip.set_sel(2, false);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(0, false);
  chip.set_sel(1, false);
  chip.set_sel(2, true);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(0, true);
  chip.set_sel(1, false);
  chip.set_sel(2, true);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(0, false);
  chip.set_sel(1, true);
  chip.set_sel(2, true);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(0, true);
  chip.set_sel(1, true);
  chip.set_sel(2, true);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_in(true);

  chip.set_sel(0, false);
  chip.set_sel(1, false);
  chip.set_sel(2, false);
  chip.computeOutput();
  expectEqual(chip.a(), true);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(0, true);
  chip.set_sel(1, false);
  chip.set_sel(2, false);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), true);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(0, false);
  chip.set_sel(1, true);
  chip.set_sel(2, false);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), true);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(0, true);
  chip.set_sel(1, true);
  chip.set_sel(2, false);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), true);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(0, false);
  chip.set_sel(1, false);
  chip.set_sel(2, true);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), true);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(0, true);
  chip.set_sel(1, false);
  chip.set_sel(2, true);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), true);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(0, false);
  chip.set_sel(1, true);
  chip.set_sel(2, true);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), true);
  expectEqual(chip.h(), false);

  chip.set_sel(0, true);
  chip.set_sel(1, true);
  chip.set_sel(2, true);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), true);
}

int main() {
  test_Nand();
  test_Not();
  test_And();
  test_Or();
  test_Xor();
  test_Mux();
  test_DMux();
  test_Not16();
  test_And16();
  test_Or16();
  test_Mux16();
  test_Or8Way();
  test_Mux4Way16();
  test_Mux8Way16();
  test_DMux4Way();
  test_DMux8Way();

  std::cout << "size of Nand: "      << sizeof(Nand) << '\n'
            << "size of And: "       << sizeof(And) << '\n'
            << "size of Not: "       << sizeof(Not) << '\n'
            << "size of Or: "        << sizeof(Or) << '\n'
            << "size of Xor: "       << sizeof(Xor) << '\n'
            << "size of Mux: "       << sizeof(Mux) << '\n'
            << "size of DMux: "      << sizeof(DMux) << '\n'
            << "size of Not16: "     << sizeof(Not16) << '\n'
            << "size of And16: "     << sizeof(And16) << '\n'
            << "size of Or16: "      << sizeof(Or16) << '\n'
            << "size of Mux16: "     << sizeof(Mux16) << '\n'
            << "size of Or8Way: "    << sizeof(Or8Way) << '\n'
            << "size of Mux4Way16: " << sizeof(Mux4Way16) << '\n'
            << "size of Mux8Way16: " << sizeof(Mux8Way16) << '\n'
            << "size of DMux4Way: "  << sizeof(DMux4Way) << '\n'
            << "size of DMux8Way: "  << sizeof(DMux8Way) << '\n';

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
