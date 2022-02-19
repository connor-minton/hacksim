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
  uint16_t in = 0x2d17;
  uint16_t out = ~in;

  chip.set_in(in);

  chip.computeOutput();
  expectEqual(chip.out(), out);
}

void test_And16() {
  And16 chip;
  uint16_t a = 0x2d17;
  uint16_t b = 0xe8ad;
  uint16_t out = a & b;

  chip.set_a(a);
  chip.set_b(b);

  chip.computeOutput();
  expectEqual(chip.out(), out);
}

void test_Or16() {
  Or16 chip;
  uint16_t a = 0x2d17;
  uint16_t b = 0xe8ad;
  uint16_t out = a | b;

  chip.set_a(a);
  chip.set_b(b);

  chip.computeOutput();
  expectEqual(chip.out(), out);
}

void test_Mux16() {
  Mux16 chip;
  uint16_t a = 0x2d17;
  uint16_t b = 0xe8ad;

  chip.set_a(a);
  chip.set_b(b);

  chip.set_sel(false);
  chip.computeOutput();
  expectEqual(chip.out(), a);

  chip.set_sel(true);
  chip.computeOutput();
  expectEqual(chip.out(), b);
}

void test_Or8Way() {
  Or8Way chip;
  uint8_t a = 0x00;
  uint8_t b = 0x60;
  uint8_t c = 0x9a;
  uint8_t d = 0xff;

  chip.set_in(a);
  chip.computeOutput();
  expectEqual(chip.out(), false);

  chip.set_in(b);
  chip.computeOutput();
  expectEqual(chip.out(), true);

  chip.set_in(c);
  chip.computeOutput();
  expectEqual(chip.out(), true);

  chip.set_in(d);
  chip.computeOutput();
  expectEqual(chip.out(), true);
}

void test_Mux4Way16() {
  Mux4Way16 chip;

  uint16_t a = 0x0000;
  uint16_t b = 0x4040;
  uint16_t c = 0x9a9a;
  uint16_t d = 0xffff;

  chip.set_a(a);
  chip.set_b(b);
  chip.set_c(c);
  chip.set_d(d);

  chip.set_sel(0);
  chip.computeOutput();
  expectEqual(chip.out(), a);

  chip.set_sel(1);
  chip.computeOutput();
  expectEqual(chip.out(), b);

  chip.set_sel(2);
  chip.computeOutput();
  expectEqual(chip.out(), c);

  chip.set_sel(3);
  chip.computeOutput();
  expectEqual(chip.out(), d);
}

void test_Mux8Way16() {
  Mux8Way16 chip;

  uint16_t a = 0x0000;
  uint16_t b = 0x4040;
  uint16_t c = 0x9a9a;
  uint16_t d = 0xfffd;
  uint16_t e = 0xbdff;
  uint16_t f = 0xfdeb;
  uint16_t g = 0xffbf;
  uint16_t h = 0xdbf7;

  chip.set_a(a);
  chip.set_b(b);
  chip.set_c(c);
  chip.set_d(d);
  chip.set_e(e);
  chip.set_f(f);
  chip.set_g(g);
  chip.set_h(h);

  chip.set_sel(0);
  chip.computeOutput();
  expectEqual(chip.out(), a);

  chip.set_sel(1);
  chip.computeOutput();
  expectEqual(chip.out(), b);

  chip.set_sel(2);
  chip.computeOutput();
  expectEqual(chip.out(), c);

  chip.set_sel(3);
  chip.computeOutput();
  expectEqual(chip.out(), d);

  chip.set_sel(4);
  chip.computeOutput();
  expectEqual(chip.out(), e);

  chip.set_sel(5);
  chip.computeOutput();
  expectEqual(chip.out(), f);

  chip.set_sel(6);
  chip.computeOutput();
  expectEqual(chip.out(), g);

  chip.set_sel(7);
  chip.computeOutput();
  expectEqual(chip.out(), h);
}

void test_DMux4Way() {
  DMux4Way chip;

  chip.set_in(false);

  chip.set_sel(0);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);

  chip.set_sel(1);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);

  chip.set_sel(2);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);

  chip.set_sel(3);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);

  chip.set_in(true);

  chip.set_sel(0);
  chip.computeOutput();
  expectEqual(chip.a(), true);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);

  chip.set_sel(1);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), true);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);

  chip.set_sel(2);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), true);
  expectEqual(chip.d(), false);

  chip.set_sel(3);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), true);
}

void test_DMux8Way() {
  DMux8Way chip;

  chip.set_in(false);

  chip.set_sel(0);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(1);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(2);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(3);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(4);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(5);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(6);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(7);
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

  chip.set_sel(0);
  chip.computeOutput();
  expectEqual(chip.a(), true);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(1);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), true);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(2);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), true);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(3);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), true);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(4);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), true);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(5);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), true);
  expectEqual(chip.g(), false);
  expectEqual(chip.h(), false);

  chip.set_sel(6);
  chip.computeOutput();
  expectEqual(chip.a(), false);
  expectEqual(chip.b(), false);
  expectEqual(chip.c(), false);
  expectEqual(chip.d(), false);
  expectEqual(chip.e(), false);
  expectEqual(chip.f(), false);
  expectEqual(chip.g(), true);
  expectEqual(chip.h(), false);

  chip.set_sel(7);
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
