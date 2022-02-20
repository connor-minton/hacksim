#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <chrono>

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

void test_HalfAdder() {
  HalfAdder chip;

  chip.set_a(false);
  chip.set_b(false);
  chip.computeOutput();
  expectEqual(chip.sum(), false);
  expectEqual(chip.carry(), false);

  chip.set_a(true);
  chip.set_b(false);
  chip.computeOutput();
  expectEqual(chip.sum(), true);
  expectEqual(chip.carry(), false);

  chip.set_a(false);
  chip.set_b(true);
  chip.computeOutput();
  expectEqual(chip.sum(), true);
  expectEqual(chip.carry(), false);

  chip.set_a(true);
  chip.set_b(true);
  chip.computeOutput();
  expectEqual(chip.sum(), false);
  expectEqual(chip.carry(), true);
}

void test_FullAdder() {
  FullAdder chip;

  chip.set_a(false);
  chip.set_b(false);
  chip.set_c(false);
  chip.computeOutput();
  expectEqual(chip.sum(), false);
  expectEqual(chip.carry(), false);

  chip.set_a(true);
  chip.set_b(false);
  chip.set_c(false);
  chip.computeOutput();
  expectEqual(chip.sum(), true);
  expectEqual(chip.carry(), false);

  chip.set_a(false);
  chip.set_b(true);
  chip.set_c(false);
  chip.computeOutput();
  expectEqual(chip.sum(), true);
  expectEqual(chip.carry(), false);

  chip.set_a(true);
  chip.set_b(true);
  chip.set_c(false);
  chip.computeOutput();
  expectEqual(chip.sum(), false);
  expectEqual(chip.carry(), true);

  chip.set_a(false);
  chip.set_b(false);
  chip.set_c(true);
  chip.computeOutput();
  expectEqual(chip.sum(), true);
  expectEqual(chip.carry(), false);

  chip.set_a(true);
  chip.set_b(false);
  chip.set_c(true);
  chip.computeOutput();
  expectEqual(chip.sum(), false);
  expectEqual(chip.carry(), true);

  chip.set_a(false);
  chip.set_b(true);
  chip.set_c(true);
  chip.computeOutput();
  expectEqual(chip.sum(), false);
  expectEqual(chip.carry(), true);

  chip.set_a(true);
  chip.set_b(true);
  chip.set_c(true);
  chip.computeOutput();
  expectEqual(chip.sum(), true);
  expectEqual(chip.carry(), true);
}

void test_Add16() {
  Add16 chip;

  std::vector<uint16_t> x =
    { 0x1234, 0x0000, 0xe0e0, 0xabcd,
      0x4312, 0x0001, 0x00aa, 0xffff };

  for (int i = 0; i < x.size()-1; i++) {
    for (int j = i+1; j < x.size(); j++) {
      chip.set_a(x[i]);
      chip.set_b(x[j]);
      chip.computeOutput();
      expectEqual(chip.out(), (uint16_t)(x[i] + x[j]));
    }
  }
}

void test_Inc16() {
  Inc16 chip;

  std::vector<uint16_t> x =
    { 0x1234, 0x0000, 0xe0e0, 0xabcd,
      0x4312, 0x0001, 0x00aa, 0xffff };

  for (int i = 0; i < x.size()-1; i++) {
    chip.set_in(x[i]);
    chip.computeOutput();
    expectEqual(chip.out(), (uint16_t)(x[i] + 1));
  }
}

void test_ALU_zero(ALU& chip, uint16_t x, uint16_t y) {
  chip.set_x(x);
  chip.set_y(y);
  chip.set_zx(true);
  chip.set_nx(false);
  chip.set_zy(true);
  chip.set_ny(false);
  chip.set_f(true);
  chip.set_no(false);
  chip.computeOutput();

  uint16_t result = (uint16_t)(0);

  expectEqual(chip.out(), result);
  expectEqual(chip.zr(), result == 0);
  expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_one(ALU& chip, uint16_t x, uint16_t y) {
  chip.set_x(x);
  chip.set_y(y);
  chip.set_zx(true);
  chip.set_nx(true);
  chip.set_zy(true);
  chip.set_ny(true);
  chip.set_f(true);
  chip.set_no(true);
  chip.computeOutput();

  uint16_t result = (uint16_t)(1);

  expectEqual(chip.out(), result);
  expectEqual(chip.zr(), result == 0);
  expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_negone(ALU& chip, uint16_t x, uint16_t y) {
  chip.set_x(x);
  chip.set_y(y);
  chip.set_zx(true);
  chip.set_nx(true);
  chip.set_zy(true);
  chip.set_ny(false);
  chip.set_f(true);
  chip.set_no(false);
  chip.computeOutput();

  uint16_t result = (uint16_t)(0xffff);

  expectEqual(chip.out(), result);
  expectEqual(chip.zr(), result == 0);
  expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_x(ALU& chip, uint16_t x, uint16_t y) {
  chip.set_x(x);
  chip.set_y(y);
  chip.set_zx(false);
  chip.set_nx(false);
  chip.set_zy(true);
  chip.set_ny(true);
  chip.set_f(false);
  chip.set_no(false);
  chip.computeOutput();

  uint16_t result = (uint16_t)(x);

  expectEqual(chip.out(), result);
  expectEqual(chip.zr(), result == 0);
  expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_y(ALU& chip, uint16_t x, uint16_t y) {
  chip.set_x(x);
  chip.set_y(y);
  chip.set_zx(true);
  chip.set_nx(true);
  chip.set_zy(false);
  chip.set_ny(false);
  chip.set_f(false);
  chip.set_no(false);
  chip.computeOutput();

  uint16_t result = (uint16_t)(y);

  expectEqual(chip.out(), result);
  expectEqual(chip.zr(), result == 0);
  expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_notx(ALU& chip, uint16_t x, uint16_t y) {
  chip.set_x(x);
  chip.set_y(y);
  chip.set_zx(false);
  chip.set_nx(false);
  chip.set_zy(true);
  chip.set_ny(true);
  chip.set_f(false);
  chip.set_no(true);
  chip.computeOutput();

  uint16_t result = (uint16_t)(~x);

  expectEqual(chip.out(), result);
  expectEqual(chip.zr(), result == 0);
  expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_noty(ALU& chip, uint16_t x, uint16_t y) {
  chip.set_x(x);
  chip.set_y(y);
  chip.set_zx(true);
  chip.set_nx(true);
  chip.set_zy(false);
  chip.set_ny(false);
  chip.set_f(false);
  chip.set_no(true);
  chip.computeOutput();

  uint16_t result = (uint16_t)(~y);

  expectEqual(chip.out(), result);
  expectEqual(chip.zr(), result == 0);
  expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_negx(ALU& chip, uint16_t x, uint16_t y) {
  chip.set_x(x);
  chip.set_y(y);
  chip.set_zx(false);
  chip.set_nx(false);
  chip.set_zy(true);
  chip.set_ny(true);
  chip.set_f(true);
  chip.set_no(true);
  chip.computeOutput();

  uint16_t result = (uint16_t)(-x);

  expectEqual(chip.out(), result);
  expectEqual(chip.zr(), result == 0);
  expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_negy(ALU& chip, uint16_t x, uint16_t y) {
  chip.set_x(x);
  chip.set_y(y);
  chip.set_zx(true);
  chip.set_nx(true);
  chip.set_zy(false);
  chip.set_ny(false);
  chip.set_f(true);
  chip.set_no(true);
  chip.computeOutput();

  uint16_t result = (uint16_t)(-y);

  expectEqual(chip.out(), result);
  expectEqual(chip.zr(), result == 0);
  expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_xplus1(ALU& chip, uint16_t x, uint16_t y) {
  chip.set_x(x);
  chip.set_y(y);
  chip.set_zx(false);
  chip.set_nx(true);
  chip.set_zy(true);
  chip.set_ny(true);
  chip.set_f(true);
  chip.set_no(true);
  chip.computeOutput();

  uint16_t result = x + 1;

  expectEqual(chip.out(), result);
  expectEqual(chip.zr(), result == 0);
  expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_yplus1(ALU& chip, uint16_t x, uint16_t y) {
  chip.set_x(x);
  chip.set_y(y);
  chip.set_zx(true);
  chip.set_nx(true);
  chip.set_zy(false);
  chip.set_ny(true);
  chip.set_f(true);
  chip.set_no(true);
  chip.computeOutput();

  uint16_t result = y + 1;

  expectEqual(chip.out(), result);
  expectEqual(chip.zr(), result == 0);
  expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_xminus1(ALU& chip, uint16_t x, uint16_t y) {
  chip.set_x(x);
  chip.set_y(y);
  chip.set_zx(false);
  chip.set_nx(false);
  chip.set_zy(true);
  chip.set_ny(true);
  chip.set_f(true);
  chip.set_no(false);
  chip.computeOutput();

  uint16_t result = (uint16_t)(x - 1);

  expectEqual(chip.out(), result);
  expectEqual(chip.zr(), result == 0);
  expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_yminus1(ALU& chip, uint16_t x, uint16_t y) {
  chip.set_x(x);
  chip.set_y(y);
  chip.set_zx(true);
  chip.set_nx(true);
  chip.set_zy(false);
  chip.set_ny(false);
  chip.set_f(true);
  chip.set_no(false);
  chip.computeOutput();

  uint16_t result = (uint16_t)(y - 1);

  expectEqual(chip.out(), result);
  expectEqual(chip.zr(), result == 0);
  expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_xplusy(ALU& chip, uint16_t x, uint16_t y) {
  chip.set_x(x);
  chip.set_y(y);
  chip.set_zx(false);
  chip.set_nx(false);
  chip.set_zy(false);
  chip.set_ny(false);
  chip.set_f(true);
  chip.set_no(false);
  chip.computeOutput();

  uint16_t result = (uint16_t)(x + y);

  expectEqual(chip.out(), result);
  expectEqual(chip.zr(), result == 0);
  expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_xminusy(ALU& chip, uint16_t x, uint16_t y) {
  chip.set_x(x);
  chip.set_y(y);
  chip.set_zx(false);
  chip.set_nx(true);
  chip.set_zy(false);
  chip.set_ny(false);
  chip.set_f(true);
  chip.set_no(true);
  chip.computeOutput();

  uint16_t result = (uint16_t)(x - y);

  expectEqual(chip.out(), result);
  expectEqual(chip.zr(), result == 0);
  expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_yminusx(ALU& chip, uint16_t x, uint16_t y) {
  chip.set_x(x);
  chip.set_y(y);
  chip.set_zx(false);
  chip.set_nx(false);
  chip.set_zy(false);
  chip.set_ny(true);
  chip.set_f(true);
  chip.set_no(true);
  chip.computeOutput();

  uint16_t result = (uint16_t)(y - x);

  expectEqual(chip.out(), result);
  expectEqual(chip.zr(), result == 0);
  expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_xandy(ALU& chip, uint16_t x, uint16_t y) {
  chip.set_x(x);
  chip.set_y(y);
  chip.set_zx(false);
  chip.set_nx(false);
  chip.set_zy(false);
  chip.set_ny(false);
  chip.set_f(false);
  chip.set_no(false);
  chip.computeOutput();

  uint16_t result = (uint16_t)(x & y);

  expectEqual(chip.out(), result);
  expectEqual(chip.zr(), result == 0);
  expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_xory(ALU& chip, uint16_t x, uint16_t y) {
  chip.set_x(x);
  chip.set_y(y);
  chip.set_zx(false);
  chip.set_nx(true);
  chip.set_zy(false);
  chip.set_ny(true);
  chip.set_f(false);
  chip.set_no(true);
  chip.computeOutput();

  uint16_t result = (uint16_t)(x | y);

  expectEqual(chip.out(), result);
  expectEqual(chip.zr(), result == 0);
  expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU() {
  ALU chip;

  std::vector<uint16_t> x =
    { 0x1234, 0x0000, 0xe0e0, 0xabcd,
      0x4312, 0x0001, 0x00aa, 0xffff };

  for (int i = 0; i < x.size()-1; i++) {
    for (int j = i+1; j < x.size(); j++) {
      test_ALU_zero(chip, x[i], x[j]);
      test_ALU_one(chip, x[i], x[j]);
      test_ALU_negone(chip, x[i], x[j]);
      test_ALU_x(chip, x[i], x[j]);
      test_ALU_y(chip, x[i], x[j]);
      test_ALU_notx(chip, x[i], x[j]);
      test_ALU_noty(chip, x[i], x[j]);
      test_ALU_negx(chip, x[i], x[j]);
      test_ALU_negy(chip, x[i], x[j]);
      test_ALU_xplus1(chip, x[i], x[j]);
      test_ALU_yplus1(chip, x[i], x[j]);
      test_ALU_xminus1(chip, x[i], x[j]);
      test_ALU_yminus1(chip, x[i], x[j]);
      test_ALU_xplusy(chip, x[i], x[j]);
      test_ALU_xminusy(chip, x[i], x[j]);
      test_ALU_yminusx(chip, x[i], x[j]);
      test_ALU_xandy(chip, x[i], x[j]);
      test_ALU_xory(chip, x[i], x[j]);
    }
  }
}

void test_DFF() {
  DFF chip;

  chip.set_in(false);
  chip.tock();
  expectEqual(chip.out(), false);

  chip.set_in(true);
  expectEqual(chip.out(), false);
  chip.tock();
  expectEqual(chip.out(), true);

  chip.set_in(false);
  expectEqual(chip.out(), true);
  chip.tock();
  expectEqual(chip.out(), false);
}

void test_Bit() {
  Bit chip;

  chip.set_in(false);
  chip.set_load(true);
  chip.tock();

  expectEqual(chip.out(), false);

  chip.set_in(true);
  chip.set_load(false);
  expectEqual(chip.out(), false);
  chip.tock();
  expectEqual(chip.out(), false);

  chip.set_in(true);
  chip.set_load(true);
  expectEqual(chip.out(), false);
  chip.tock();
  expectEqual(chip.out(), true);

  chip.set_in(false);
  chip.set_load(false);
  expectEqual(chip.out(), true);
  chip.tock();
  expectEqual(chip.out(), true);

  chip.set_in(false);
  chip.set_load(true);
  expectEqual(chip.out(), true);
  chip.tock();
  expectEqual(chip.out(), false);
}

void test_Register() {
  Register chip;

  chip.set_in(0x0000);
  chip.set_load(true);
  chip.tock();

  expectEqual(chip.out(), (uint16_t)0x0000);

  chip.set_load(false);
  chip.tock();
  expectEqual(chip.out(), (uint16_t)0x0000);

  chip.set_in(0x1234);
  chip.set_load(false);
  expectEqual(chip.out(), (uint16_t)0x0000);
  chip.tock();
  expectEqual(chip.out(), (uint16_t)0x0000);

  chip.set_load(true);
  chip.tock();
  expectEqual(chip.out(), (uint16_t)0x1234);

  chip.set_load(false);
  chip.set_in(0);
  expectEqual(chip.out(), (uint16_t)0x1234);
  chip.tock();
  expectEqual(chip.out(), (uint16_t)0x1234);

  chip.set_load(true);
  expectEqual(chip.out(), (uint16_t)0x1234);
  chip.tock();
  expectEqual(chip.out(), (uint16_t)0x0000);
}

void test_PC() {
  PC chip;

  expectEqual(chip.out(), (uint16_t)0);

  chip.set_load(true);
  chip.set_reset(false);
  chip.set_inc(false);
  chip.set_in(0x30);
  expectEqual(chip.out(), (uint16_t)0);
  chip.tock();
  expectEqual(chip.out(), (uint16_t)0x30);

  chip.set_load(true);
  chip.set_reset(true);
  chip.set_inc(true);
  expectEqual(chip.out(), (uint16_t)0x30);
  chip.tock();
  expectEqual(chip.out(), (uint16_t)0);

  chip.set_load(true);
  chip.set_reset(false);
  chip.set_inc(true);
  expectEqual(chip.out(), (uint16_t)0);
  chip.tock();
  expectEqual(chip.out(), (uint16_t)0x30);

  chip.set_load(false);
  chip.set_reset(false);
  chip.set_inc(false);
  expectEqual(chip.out(), (uint16_t)0x30);
  chip.tock();
  expectEqual(chip.out(), (uint16_t)0x30);

  chip.set_load(false);
  chip.set_reset(false);
  chip.set_inc(true);
  expectEqual(chip.out(), (uint16_t)0x30);
  chip.tock();
  expectEqual(chip.out(), (uint16_t)0x31);
  chip.tock();
  expectEqual(chip.out(), (uint16_t)0x32);
  chip.tock();
  expectEqual(chip.out(), (uint16_t)0x33);
  chip.tock();
  expectEqual(chip.out(), (uint16_t)0x34);
  chip.tock();
  expectEqual(chip.out(), (uint16_t)0x35);
  chip.set_inc(false);
  chip.tock();
  expectEqual(chip.out(), (uint16_t)0x35);
  chip.tock();
  expectEqual(chip.out(), (uint16_t)0x35);
}

template<typename RAM_T>
void test_RAMn(int n) {
  RAM_T* chip = new RAM_T();

  chip->set_load(false);
  chip->set_in(0x1234);
  for (int i = 0; i < n; i++) {
    chip->set_address(i);
    chip->tock();
    expectEqual(chip->out(), (uint16_t)0);
  }

  chip->set_load(true);
  for (int i = 0; i < n; i++) {
    chip->set_address(i);
    chip->set_in(i*2);
    chip->tock();
    expectEqual(chip->out(), (uint16_t)(i*2));
  }

  chip->set_load(false);
  for (int i = 0; i < n; i++) {
    chip->set_address(i);
    chip->tock();
    expectEqual(chip->out(), (uint16_t)(i*2));
  }

  chip->set_load(true);
  for (int i = 0; i < n; i++) {
    chip->set_address(i);
    chip->set_in(0);
    chip->tock();
    expectEqual(chip->out(), (uint16_t)0);
  }

  chip->set_load(false);
  chip->set_in(0x1234);
  for (int i = 0; i < n; i++) {
    chip->set_address(i);
    chip->tock();
    expectEqual(chip->out(), (uint16_t)0);
  }

  delete chip;
}

void test(std::string name, void (*func)()) {
  std::chrono::high_resolution_clock clk;
  std::cout << name << "...";
  int old = failedCt;
  auto t1 = clk.now();
  func();
  auto t2 = clk.now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
  if (old == failedCt) {
    std::cout << " PASS (" << ms.count() << " ms)\n";
  }
  else {
    std::cout << name << "... FAIL (" << ms.count() << " ms)\n";
  }
}

int main() {
  test("Nand", test_Nand);
  test("Not", test_Not);
  test("And", test_And);
  test("Or", test_Or);
  test("Xor", test_Xor);
  test("Mux", test_Mux);
  test("DMux", test_DMux);
  test("Not16", test_Not16);
  test("And16", test_And16);
  test("Or16", test_Or16);
  test("Mux16", test_Mux16);
  test("Or8Way", test_Or8Way);
  test("Mux4Way16", test_Mux4Way16);
  test("Mux8Way16", test_Mux8Way16);
  test("DMux4Way", test_DMux4Way);
  test("DMux8Way", test_DMux8Way);
  test("HalfAdder", test_HalfAdder);
  test("FullAdder", test_FullAdder);
  test("Add16", test_Add16);
  test("Inc16", test_Inc16);
  test("ALU", test_ALU);
  test("DFF", test_DFF);
  test("Bit", test_Bit);
  test("Register", test_Register);
  test("PC", test_PC);
  test("RAM8", [](){ test_RAMn<RAM8>(8); });
  test("RAM64", [](){ test_RAMn<RAM64>(64); });
  test("RAM512", [](){ test_RAMn<RAM512>(512); });
  test("RAM4K", [](){ test_RAMn<RAM4K>(4096); });
  test("RAM16K", [](){ test_RAMn<RAM16K>(16384); });

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
            << "size of DMux8Way: "  << sizeof(DMux8Way) << '\n'
            << "size of HalfAdder: "  << sizeof(HalfAdder) << '\n'
            << "size of FullAdder: "  << sizeof(FullAdder) << '\n'
            << "size of Add16: "  << sizeof(Add16) << '\n'
            << "size of Inc16: "  << sizeof(Inc16) << '\n'
            << "size of ALU: "  << sizeof(ALU) << '\n'
            << "size of DFF: "  << sizeof(DFF) << '\n'
            << "size of Bit: "  << sizeof(Bit) << '\n'
            << "size of Register: "  << sizeof(Register) << '\n'
            << "size of PC: "  << sizeof(PC) << '\n'
            << "size of RAM8: "  << sizeof(RAM8) << '\n'
            << "size of RAM64: "  << sizeof(RAM64) << '\n'
            << "size of RAM512: "  << sizeof(RAM512) << '\n'
            << "size of RAM4K: "  << sizeof(RAM4K) << '\n'
            << "size of RAM16K: "  << sizeof(RAM16K) << '\n';

  std::cout << "===================================\n"
            << "TESTS FAILED:    " << std::setw(7) << failedCt << '\n'
            << "TESTS PERFORMED: " << std::setw(7) << (failedCt + successCt) << '\n'
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
