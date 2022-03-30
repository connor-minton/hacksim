#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include "Chips.h"
#include "ShallowChips.h"
#include "Test.h"

void test_Nand(TestContext& ctx) {
  Nand n1;
  n1.set_a(true);
  n1.set_b(false);
  n1.computeOutput();
  ctx.expectEqual(n1.out(), true);

  n1.set_a(false);
  n1.set_b(false);
  n1.computeOutput();
  ctx.expectEqual(n1.out(), true);

  n1.set_a(false);
  n1.set_b(true);
  n1.computeOutput();
  ctx.expectEqual(n1.out(), true);

  n1.set_a(true);
  n1.set_b(true);
  n1.computeOutput();
  ctx.expectEqual(n1.out(), false);
}

void test_Nand16(TestContext& ctx) {
  Nand16 chip;
  uint16_t a = 0x2d17;
  uint16_t b = 0xe8ad;

  uint16_t out = ~(a & b);

  chip.set_a(a);
  chip.set_b(b);

  chip.computeOutput();
  ctx.expectEqual(chip.out(), out);
}

void test_Not(TestContext& ctx) {
  Not n;
  n.set_in(true);
  n.computeOutput();
  ctx.expectEqual(n.out(), false);

  n.set_in(false);
  n.computeOutput();
  ctx.expectEqual(n.out(), true);
}

void test_And(TestContext& ctx) {
  And chip;
  chip.set_a(true);
  chip.set_b(false);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), false);

  chip.set_a(false);
  chip.set_b(false);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), false);

  chip.set_a(false);
  chip.set_b(true);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), false);

  chip.set_a(true);
  chip.set_b(true);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), true);
}

void test_Or(TestContext& ctx) {
  Or chip;
  chip.set_a(true);
  chip.set_b(false);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), true);

  chip.set_a(false);
  chip.set_b(false);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), false);

  chip.set_a(false);
  chip.set_b(true);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), true);

  chip.set_a(true);
  chip.set_b(true);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), true);
}

void test_Xor(TestContext& ctx) {
  Xor chip;
  chip.set_a(true);
  chip.set_b(false);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), true);

  chip.set_a(false);
  chip.set_b(false);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), false);

  chip.set_a(false);
  chip.set_b(true);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), true);

  chip.set_a(true);
  chip.set_b(true);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), false);
}

void test_Mux(TestContext& ctx) {
  Mux chip;
  chip.set_a(false);
  chip.set_b(false);

  chip.set_sel(false);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), false);
  chip.set_sel(true);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), false);

  chip.set_a(true);
  chip.set_b(false);

  chip.set_sel(false);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), true);
  chip.set_sel(true);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), false);

  chip.set_a(false);
  chip.set_b(true);

  chip.set_sel(false);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), false);
  chip.set_sel(true);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), true);

  chip.set_a(true);
  chip.set_b(true);

  chip.set_sel(false);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), true);
  chip.set_sel(true);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), true);
}

void test_DMux(TestContext& ctx) {
  DMux chip;

  chip.set_in(true);

  chip.set_sel(false);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), true);
  ctx.expectEqual(chip.b(), false);

  chip.set_sel(true);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), true);

  chip.set_in(false);

  chip.set_sel(false);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);

  chip.set_sel(true);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);
}

void test_Not16(TestContext& ctx) {
  Not16 chip;
  uint16_t in = 0x2d17;
  uint16_t out = ~in;

  chip.set_in(in);

  chip.computeOutput();
  ctx.expectEqual(chip.out(), out);
}

void test_And16(TestContext& ctx) {
  And16 chip;
  uint16_t a = 0x2d17;
  uint16_t b = 0xe8ad;
  uint16_t out = a & b;

  chip.set_a(a);
  chip.set_b(b);

  chip.computeOutput();
  ctx.expectEqual(chip.out(), out);
}

void test_Or16(TestContext& ctx) {
  Or16 chip;
  uint16_t a = 0x2d17;
  uint16_t b = 0xe8ad;
  uint16_t out = a | b;

  chip.set_a(a);
  chip.set_b(b);

  chip.computeOutput();
  ctx.expectEqual(chip.out(), out);
}

void test_Mux16(TestContext& ctx) {
  Mux16 chip;
  uint16_t a = 0x2d17;
  uint16_t b = 0xe8ad;

  chip.set_a(a);
  chip.set_b(b);

  chip.set_sel(false);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), a);

  chip.set_sel(true);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), b);
}

void test_Or8Way(TestContext& ctx) {
  Or8Way chip;
  uint8_t a = 0x00;
  uint8_t b = 0x60;
  uint8_t c = 0x9a;
  uint8_t d = 0xff;

  chip.set_in(a);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), false);

  chip.set_in(b);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), true);

  chip.set_in(c);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), true);

  chip.set_in(d);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), true);
}

void test_Mux4Way16(TestContext& ctx) {
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
  ctx.expectEqual(chip.out(), a);

  chip.set_sel(1);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), b);

  chip.set_sel(2);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), c);

  chip.set_sel(3);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), d);
}

void test_Mux8Way16(TestContext& ctx) {
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
  ctx.expectEqual(chip.out(), a);

  chip.set_sel(1);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), b);

  chip.set_sel(2);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), c);

  chip.set_sel(3);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), d);

  chip.set_sel(4);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), e);

  chip.set_sel(5);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), f);

  chip.set_sel(6);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), g);

  chip.set_sel(7);
  chip.computeOutput();
  ctx.expectEqual(chip.out(), h);
}

void test_DMux4Way(TestContext& ctx) {
  DMux4Way chip;

  chip.set_in(false);

  chip.set_sel(0);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), false);

  chip.set_sel(1);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), false);

  chip.set_sel(2);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), false);

  chip.set_sel(3);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), false);

  chip.set_in(true);

  chip.set_sel(0);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), true);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), false);

  chip.set_sel(1);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), true);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), false);

  chip.set_sel(2);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), true);
  ctx.expectEqual(chip.d(), false);

  chip.set_sel(3);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), true);
}

void test_DMux8Way(TestContext& ctx) {
  DMux8Way chip;

  chip.set_in(false);

  chip.set_sel(0);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), false);
  ctx.expectEqual(chip.e(), false);
  ctx.expectEqual(chip.f(), false);
  ctx.expectEqual(chip.g(), false);
  ctx.expectEqual(chip.h(), false);

  chip.set_sel(1);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), false);
  ctx.expectEqual(chip.e(), false);
  ctx.expectEqual(chip.f(), false);
  ctx.expectEqual(chip.g(), false);
  ctx.expectEqual(chip.h(), false);

  chip.set_sel(2);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), false);
  ctx.expectEqual(chip.e(), false);
  ctx.expectEqual(chip.f(), false);
  ctx.expectEqual(chip.g(), false);
  ctx.expectEqual(chip.h(), false);

  chip.set_sel(3);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), false);
  ctx.expectEqual(chip.e(), false);
  ctx.expectEqual(chip.f(), false);
  ctx.expectEqual(chip.g(), false);
  ctx.expectEqual(chip.h(), false);

  chip.set_sel(4);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), false);
  ctx.expectEqual(chip.e(), false);
  ctx.expectEqual(chip.f(), false);
  ctx.expectEqual(chip.g(), false);
  ctx.expectEqual(chip.h(), false);

  chip.set_sel(5);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), false);
  ctx.expectEqual(chip.e(), false);
  ctx.expectEqual(chip.f(), false);
  ctx.expectEqual(chip.g(), false);
  ctx.expectEqual(chip.h(), false);

  chip.set_sel(6);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), false);
  ctx.expectEqual(chip.e(), false);
  ctx.expectEqual(chip.f(), false);
  ctx.expectEqual(chip.g(), false);
  ctx.expectEqual(chip.h(), false);

  chip.set_sel(7);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), false);
  ctx.expectEqual(chip.e(), false);
  ctx.expectEqual(chip.f(), false);
  ctx.expectEqual(chip.g(), false);
  ctx.expectEqual(chip.h(), false);

  chip.set_in(true);

  chip.set_sel(0);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), true);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), false);
  ctx.expectEqual(chip.e(), false);
  ctx.expectEqual(chip.f(), false);
  ctx.expectEqual(chip.g(), false);
  ctx.expectEqual(chip.h(), false);

  chip.set_sel(1);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), true);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), false);
  ctx.expectEqual(chip.e(), false);
  ctx.expectEqual(chip.f(), false);
  ctx.expectEqual(chip.g(), false);
  ctx.expectEqual(chip.h(), false);

  chip.set_sel(2);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), true);
  ctx.expectEqual(chip.d(), false);
  ctx.expectEqual(chip.e(), false);
  ctx.expectEqual(chip.f(), false);
  ctx.expectEqual(chip.g(), false);
  ctx.expectEqual(chip.h(), false);

  chip.set_sel(3);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), true);
  ctx.expectEqual(chip.e(), false);
  ctx.expectEqual(chip.f(), false);
  ctx.expectEqual(chip.g(), false);
  ctx.expectEqual(chip.h(), false);

  chip.set_sel(4);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), false);
  ctx.expectEqual(chip.e(), true);
  ctx.expectEqual(chip.f(), false);
  ctx.expectEqual(chip.g(), false);
  ctx.expectEqual(chip.h(), false);

  chip.set_sel(5);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), false);
  ctx.expectEqual(chip.e(), false);
  ctx.expectEqual(chip.f(), true);
  ctx.expectEqual(chip.g(), false);
  ctx.expectEqual(chip.h(), false);

  chip.set_sel(6);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), false);
  ctx.expectEqual(chip.e(), false);
  ctx.expectEqual(chip.f(), false);
  ctx.expectEqual(chip.g(), true);
  ctx.expectEqual(chip.h(), false);

  chip.set_sel(7);
  chip.computeOutput();
  ctx.expectEqual(chip.a(), false);
  ctx.expectEqual(chip.b(), false);
  ctx.expectEqual(chip.c(), false);
  ctx.expectEqual(chip.d(), false);
  ctx.expectEqual(chip.e(), false);
  ctx.expectEqual(chip.f(), false);
  ctx.expectEqual(chip.g(), false);
  ctx.expectEqual(chip.h(), true);
}

void test_HalfAdder(TestContext& ctx) {
  HalfAdder chip;

  chip.set_a(false);
  chip.set_b(false);
  chip.computeOutput();
  ctx.expectEqual(chip.sum(), false);
  ctx.expectEqual(chip.carry(), false);

  chip.set_a(true);
  chip.set_b(false);
  chip.computeOutput();
  ctx.expectEqual(chip.sum(), true);
  ctx.expectEqual(chip.carry(), false);

  chip.set_a(false);
  chip.set_b(true);
  chip.computeOutput();
  ctx.expectEqual(chip.sum(), true);
  ctx.expectEqual(chip.carry(), false);

  chip.set_a(true);
  chip.set_b(true);
  chip.computeOutput();
  ctx.expectEqual(chip.sum(), false);
  ctx.expectEqual(chip.carry(), true);
}

void test_FullAdder(TestContext& ctx) {
  FullAdder chip;

  chip.set_a(false);
  chip.set_b(false);
  chip.set_c(false);
  chip.computeOutput();
  ctx.expectEqual(chip.sum(), false);
  ctx.expectEqual(chip.carry(), false);

  chip.set_a(true);
  chip.set_b(false);
  chip.set_c(false);
  chip.computeOutput();
  ctx.expectEqual(chip.sum(), true);
  ctx.expectEqual(chip.carry(), false);

  chip.set_a(false);
  chip.set_b(true);
  chip.set_c(false);
  chip.computeOutput();
  ctx.expectEqual(chip.sum(), true);
  ctx.expectEqual(chip.carry(), false);

  chip.set_a(true);
  chip.set_b(true);
  chip.set_c(false);
  chip.computeOutput();
  ctx.expectEqual(chip.sum(), false);
  ctx.expectEqual(chip.carry(), true);

  chip.set_a(false);
  chip.set_b(false);
  chip.set_c(true);
  chip.computeOutput();
  ctx.expectEqual(chip.sum(), true);
  ctx.expectEqual(chip.carry(), false);

  chip.set_a(true);
  chip.set_b(false);
  chip.set_c(true);
  chip.computeOutput();
  ctx.expectEqual(chip.sum(), false);
  ctx.expectEqual(chip.carry(), true);

  chip.set_a(false);
  chip.set_b(true);
  chip.set_c(true);
  chip.computeOutput();
  ctx.expectEqual(chip.sum(), false);
  ctx.expectEqual(chip.carry(), true);

  chip.set_a(true);
  chip.set_b(true);
  chip.set_c(true);
  chip.computeOutput();
  ctx.expectEqual(chip.sum(), true);
  ctx.expectEqual(chip.carry(), true);
}

void test_Add16(TestContext& ctx) {
  Add16 chip;

  std::vector<uint16_t> x =
    { 0x1234, 0x0000, 0xe0e0, 0xabcd,
      0x4312, 0x0001, 0x00aa, 0xffff };

  for (int i = 0; i < x.size()-1; i++) {
    for (int j = i+1; j < x.size(); j++) {
      chip.set_a(x[i]);
      chip.set_b(x[j]);
      chip.computeOutput();
      ctx.expectEqual(chip.out(), (uint16_t)(x[i] + x[j]));
    }
  }
}

void test_Inc16(TestContext& ctx) {
  Inc16 chip;

  std::vector<uint16_t> x =
    { 0x1234, 0x0000, 0xe0e0, 0xabcd,
      0x4312, 0x0001, 0x00aa, 0xffff };

  for (int i = 0; i < x.size()-1; i++) {
    chip.set_in(x[i]);
    chip.computeOutput();
    ctx.expectEqual(chip.out(), (uint16_t)(x[i] + 1));
  }
}

void test_ALU_zero(TestContext& ctx, ALU& chip, uint16_t x, uint16_t y) {
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

  ctx.expectEqual(chip.out(), result);
  ctx.expectEqual(chip.zr(), result == 0);
  ctx.expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_one(TestContext& ctx, ALU& chip, uint16_t x, uint16_t y) {
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

  ctx.expectEqual(chip.out(), result);
  ctx.expectEqual(chip.zr(), result == 0);
  ctx.expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_negone(TestContext& ctx, ALU& chip, uint16_t x, uint16_t y) {
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

  ctx.expectEqual(chip.out(), result);
  ctx.expectEqual(chip.zr(), result == 0);
  ctx.expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_x(TestContext& ctx, ALU& chip, uint16_t x, uint16_t y) {
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

  ctx.expectEqual(chip.out(), result);
  ctx.expectEqual(chip.zr(), result == 0);
  ctx.expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_y(TestContext& ctx, ALU& chip, uint16_t x, uint16_t y) {
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

  ctx.expectEqual(chip.out(), result);
  ctx.expectEqual(chip.zr(), result == 0);
  ctx.expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_notx(TestContext& ctx, ALU& chip, uint16_t x, uint16_t y) {
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

  ctx.expectEqual(chip.out(), result);
  ctx.expectEqual(chip.zr(), result == 0);
  ctx.expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_noty(TestContext& ctx, ALU& chip, uint16_t x, uint16_t y) {
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

  ctx.expectEqual(chip.out(), result);
  ctx.expectEqual(chip.zr(), result == 0);
  ctx.expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_negx(TestContext& ctx, ALU& chip, uint16_t x, uint16_t y) {
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

  ctx.expectEqual(chip.out(), result);
  ctx.expectEqual(chip.zr(), result == 0);
  ctx.expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_negy(TestContext& ctx, ALU& chip, uint16_t x, uint16_t y) {
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

  ctx.expectEqual(chip.out(), result);
  ctx.expectEqual(chip.zr(), result == 0);
  ctx.expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_xplus1(TestContext& ctx, ALU& chip, uint16_t x, uint16_t y) {
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

  ctx.expectEqual(chip.out(), result);
  ctx.expectEqual(chip.zr(), result == 0);
  ctx.expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_yplus1(TestContext& ctx, ALU& chip, uint16_t x, uint16_t y) {
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

  ctx.expectEqual(chip.out(), result);
  ctx.expectEqual(chip.zr(), result == 0);
  ctx.expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_xminus1(TestContext& ctx, ALU& chip, uint16_t x, uint16_t y) {
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

  ctx.expectEqual(chip.out(), result);
  ctx.expectEqual(chip.zr(), result == 0);
  ctx.expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_yminus1(TestContext& ctx, ALU& chip, uint16_t x, uint16_t y) {
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

  ctx.expectEqual(chip.out(), result);
  ctx.expectEqual(chip.zr(), result == 0);
  ctx.expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_xplusy(TestContext& ctx, ALU& chip, uint16_t x, uint16_t y) {
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

  ctx.expectEqual(chip.out(), result);
  ctx.expectEqual(chip.zr(), result == 0);
  ctx.expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_xminusy(TestContext& ctx, ALU& chip, uint16_t x, uint16_t y) {
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

  ctx.expectEqual(chip.out(), result);
  ctx.expectEqual(chip.zr(), result == 0);
  ctx.expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_yminusx(TestContext& ctx, ALU& chip, uint16_t x, uint16_t y) {
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

  ctx.expectEqual(chip.out(), result);
  ctx.expectEqual(chip.zr(), result == 0);
  ctx.expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_xandy(TestContext& ctx, ALU& chip, uint16_t x, uint16_t y) {
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

  ctx.expectEqual(chip.out(), result);
  ctx.expectEqual(chip.zr(), result == 0);
  ctx.expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU_xory(TestContext& ctx, ALU& chip, uint16_t x, uint16_t y) {
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

  ctx.expectEqual(chip.out(), result);
  ctx.expectEqual(chip.zr(), result == 0);
  ctx.expectEqual(chip.ng(), getBit<15>(result));
}

void test_ALU(TestContext& ctx) {
  ALU chip;

  std::vector<uint16_t> x =
    { 0x1234, 0x0000, 0xe0e0, 0xabcd,
      0x4312, 0x0001, 0x00aa, 0xffff };

  for (int i = 0; i < x.size()-1; i++) {
    for (int j = i+1; j < x.size(); j++) {
      test_ALU_zero(ctx, chip, x[i], x[j]);
      test_ALU_one(ctx, chip, x[i], x[j]);
      test_ALU_negone(ctx, chip, x[i], x[j]);
      test_ALU_x(ctx, chip, x[i], x[j]);
      test_ALU_y(ctx, chip, x[i], x[j]);
      test_ALU_notx(ctx, chip, x[i], x[j]);
      test_ALU_noty(ctx, chip, x[i], x[j]);
      test_ALU_negx(ctx, chip, x[i], x[j]);
      test_ALU_negy(ctx, chip, x[i], x[j]);
      test_ALU_xplus1(ctx, chip, x[i], x[j]);
      test_ALU_yplus1(ctx, chip, x[i], x[j]);
      test_ALU_xminus1(ctx, chip, x[i], x[j]);
      test_ALU_yminus1(ctx, chip, x[i], x[j]);
      test_ALU_xplusy(ctx, chip, x[i], x[j]);
      test_ALU_xminusy(ctx, chip, x[i], x[j]);
      test_ALU_yminusx(ctx, chip, x[i], x[j]);
      test_ALU_xandy(ctx, chip, x[i], x[j]);
      test_ALU_xory(ctx, chip, x[i], x[j]);
    }
  }
}

void test_DFF(TestContext& ctx) {
  DFF chip;

  chip.set_in(false);
  chip.tock();
  ctx.expectEqual(chip.out(), false);

  chip.set_in(true);
  ctx.expectEqual(chip.out(), false);
  chip.tock();
  ctx.expectEqual(chip.out(), true);

  chip.set_in(false);
  ctx.expectEqual(chip.out(), true);
  chip.tock();
  ctx.expectEqual(chip.out(), false);
}

void test_Bit(TestContext& ctx) {
  Bit chip;

  chip.set_in(false);
  chip.set_load(true);
  chip.tick();
  chip.tock();

  ctx.expectEqual(chip.out(), false);

  chip.set_in(true);
  chip.set_load(false);
  chip.tick();
  ctx.expectEqual(chip.out(), false);
  chip.tock();
  ctx.expectEqual(chip.out(), false);

  chip.set_in(true);
  chip.set_load(true);
  chip.tick();
  ctx.expectEqual(chip.out(), false);
  chip.tock();
  ctx.expectEqual(chip.out(), true);

  chip.set_in(false);
  chip.set_load(false);
  chip.tick();
  ctx.expectEqual(chip.out(), true);
  chip.tock();
  ctx.expectEqual(chip.out(), true);

  chip.set_in(false);
  chip.set_load(true);
  chip.tick();
  ctx.expectEqual(chip.out(), true);
  chip.tock();
  ctx.expectEqual(chip.out(), false);
}

void test_Register(TestContext& ctx) {
  Register chip;

  chip.set_in(0x0000);
  chip.set_load(true);
  chip.tick();
  chip.tock();

  ctx.expectEqual(chip.out(), (uint16_t)0x0000);

  chip.set_load(false);
  chip.tick();
  chip.tock();
  ctx.expectEqual(chip.out(), (uint16_t)0x0000);

  chip.set_in(0x1234);
  chip.set_load(false);
  ctx.expectEqual(chip.out(), (uint16_t)0x0000);
  chip.tick();
  chip.tock();
  ctx.expectEqual(chip.out(), (uint16_t)0x0000);

  chip.set_load(true);
  chip.tick();
  chip.tock();
  ctx.expectEqual(chip.out(), (uint16_t)0x1234);

  chip.set_load(false);
  chip.set_in(0);
  ctx.expectEqual(chip.out(), (uint16_t)0x1234);
  chip.tick();
  chip.tock();
  ctx.expectEqual(chip.out(), (uint16_t)0x1234);

  chip.set_load(true);
  ctx.expectEqual(chip.out(), (uint16_t)0x1234);
  chip.tick();
  chip.tock();
  ctx.expectEqual(chip.out(), (uint16_t)0x0000);
}

void test_PC(TestContext& ctx) {
  PC chip;

  ctx.expectEqual(chip.out(), (uint16_t)0);

  chip.set_load(true);
  chip.set_reset(false);
  chip.set_inc(false);
  chip.set_in(0x30);
  chip.tick();
  ctx.expectEqual(chip.out(), (uint16_t)0);
  chip.tock();
  ctx.expectEqual(chip.out(), (uint16_t)0x30);

  chip.set_load(true);
  chip.set_reset(true);
  chip.set_inc(true);
  chip.tick();
  ctx.expectEqual(chip.out(), (uint16_t)0x30);
  chip.tock();
  ctx.expectEqual(chip.out(), (uint16_t)0);

  chip.set_load(true);
  chip.set_reset(false);
  chip.set_inc(true);
  chip.tick();
  ctx.expectEqual(chip.out(), (uint16_t)0);
  chip.tock();
  ctx.expectEqual(chip.out(), (uint16_t)0x30);

  chip.set_load(false);
  chip.set_reset(false);
  chip.set_inc(false);
  chip.tick();
  ctx.expectEqual(chip.out(), (uint16_t)0x30);
  chip.tock();
  ctx.expectEqual(chip.out(), (uint16_t)0x30);

  chip.set_load(false);
  chip.set_reset(false);
  chip.set_inc(true);
  chip.tick();
  ctx.expectEqual(chip.out(), (uint16_t)0x30);
  chip.tock();
  chip.tick();
  ctx.expectEqual(chip.out(), (uint16_t)0x31);
  chip.tock();
  chip.tick();
  ctx.expectEqual(chip.out(), (uint16_t)0x32);
  chip.tock();
  chip.tick();
  ctx.expectEqual(chip.out(), (uint16_t)0x33);
  chip.tock();
  chip.tick();
  ctx.expectEqual(chip.out(), (uint16_t)0x34);
  chip.tock();
  ctx.expectEqual(chip.out(), (uint16_t)0x35);
  chip.set_inc(false);
  chip.tick();
  chip.tock();
  ctx.expectEqual(chip.out(), (uint16_t)0x35);
  chip.tick();
  chip.tock();
  ctx.expectEqual(chip.out(), (uint16_t)0x35);
}

template<typename RAM_T>
void test_RAMn(TestContext& ctx, RAM_T& chip, int n) {
  chip.set_load(false);
  chip.set_in(0x1234);
  for (int i = 0; i < n; i++) {
    chip.set_address(i);
    chip.tick();
    chip.tock();
    ctx.expectEqual(chip.out(), (uint16_t)0);
    ctx.expectEqual(chip.peek(i), (uint16_t)0);
  }

  chip.set_load(true);
  for (int i = 0; i < n; i++) {
    chip.set_address(i);
    chip.set_in(i*2);
    chip.tick();
    ctx.expectEqual(chip.out(), (uint16_t)0);
    chip.tock();
    ctx.expectEqual(chip.out(), (uint16_t)(i*2));
    ctx.expectEqual(chip.peek(i), (uint16_t)(i*2));
  }

  chip.set_load(false);
  for (int i = 0; i < n; i++) {
    chip.set_address(i);
    chip.tick();
    chip.tock();
    ctx.expectEqual(chip.out(), (uint16_t)(i*2));
    ctx.expectEqual(chip.peek(i), (uint16_t)(i*2));
  }

  chip.set_load(true);
  for (int i = 0; i < n; i++) {
    chip.set_address(i);
    chip.set_in(0);
    chip.tick();
    ctx.expectEqual(chip.out(), (uint16_t)(i*2));
    chip.tock();
    ctx.expectEqual(chip.out(), (uint16_t)0);
    ctx.expectEqual(chip.peek(i), (uint16_t)(0));
  }

  chip.set_load(false);
  chip.set_in(0x1234);
  for (int i = 0; i < n; i++) {
    chip.set_address(i);
    chip.tick();
    chip.tock();
    ctx.expectEqual(chip.out(), (uint16_t)0);
    ctx.expectEqual(chip.peek(i), (uint16_t)(0));
  }

  // test poke and peek
  chip.set_load(false);
  for (int i = 0; i < n; i++) {
    chip.poke(i, i * 2);
    ctx.expectEqual(chip.peek(i), (uint16_t)(i*2));
    chip.set_address(i);
    chip.tick();
    ctx.expectEqual(chip.out(), (uint16_t)(i*2));
    chip.tock();
    ctx.expectEqual(chip.out(), (uint16_t)(i*2));
  }
}

void test_Memory(TestContext& ctx) {
  uint16_t kbd = 0;
  uint16_t clk = 0;
  uint16_t screen[shallow::Screen::SCREEN_SIZE] = {0};

  IMemory* chip = new Memory(screen, &kbd, &clk);

  // test lower memory
  chip->set_load(false);
  chip->set_in(0x1234);
  chip->set_address(0x0000);
  chip->tick();
  chip->tock();
  ctx.expectEqual(chip->out(), (uint16_t)0);
  ctx.expectEqual(chip->peek(0x0000), (uint16_t)0);

  chip->set_load(true);

  chip->set_in(1);
  chip->set_address(0x1234);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)0);
  chip->tock();
  chip->set_in(2);
  chip->set_address(0x2345);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)0);
  chip->tock();
  chip->set_in(3);
  chip->set_address(0x3456);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)0);
  chip->tock();
  chip->set_in(4);
  chip->set_address(0x3fff);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)0);
  chip->tock();

  chip->set_load(false);

  chip->set_address(0x1234);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)1);
  chip->tock();
  ctx.expectEqual(chip->out(), (uint16_t)1);
  ctx.expectEqual(chip->peek(0x1234), (uint16_t)1);
  chip->set_address(0x2345);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)2);
  chip->tock();
  ctx.expectEqual(chip->out(), (uint16_t)2);
  ctx.expectEqual(chip->peek(0x2345), (uint16_t)2);
  chip->set_address(0x3456);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)3);
  chip->tock();
  ctx.expectEqual(chip->out(), (uint16_t)3);
  ctx.expectEqual(chip->peek(0x3456), (uint16_t)3);
  chip->set_address(0x3fff);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)4);
  chip->tock();
  ctx.expectEqual(chip->out(), (uint16_t)4);
  ctx.expectEqual(chip->peek(0x3fff), (uint16_t)4);

  // check that the screen is still zeroed out
  chip->set_load(false);
  for (int i = Memory::SCREEN; i < Memory::KBD; i++) {
    chip->set_address(i);
    chip->tick();
    chip->tock();
    ctx.expectEqual(chip->out(), (uint16_t)0);
    ctx.expectEqual(chip->peek(i), (uint16_t)0);
  }

  // test screen memory
  chip->set_load(true);

  chip->set_in(1);
  chip->set_address(0x4234);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)0);
  chip->tock();
  chip->set_in(2);
  chip->set_address(0x4444);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)0);
  chip->tock();
  chip->set_in(3);
  chip->set_address(0x5555);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)0);
  chip->tock();
  chip->set_in(4);
  chip->set_address(0x5fff);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)0);
  chip->tock();

  chip->set_load(false);

  chip->set_address(0x4234);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)1);
  chip->tock();
  ctx.expectEqual(chip->out(), (uint16_t)1);
  ctx.expectEqual(chip->peek(0x4234), (uint16_t)1);
  chip->set_address(0x4444);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)2);
  chip->tock();
  ctx.expectEqual(chip->out(), (uint16_t)2);
  ctx.expectEqual(chip->peek(0x4444), (uint16_t)2);
  chip->set_address(0x5555);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)3);
  chip->tock();
  ctx.expectEqual(chip->out(), (uint16_t)3);
  ctx.expectEqual(chip->peek(0x5555), (uint16_t)3);
  chip->set_address(0x5fff);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)4);
  chip->tock();
  ctx.expectEqual(chip->out(), (uint16_t)4);
  ctx.expectEqual(chip->peek(0x5fff), (uint16_t)4);

  // test keyboard
  chip->set_load(false);
  chip->set_in(0x1234);
  chip->set_address(Memory::KBD);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)0);
  chip->tock();
  ctx.expectEqual(kbd, (uint16_t)0);
  ctx.expectEqual(chip->out(), (uint16_t)0);
  ctx.expectEqual(chip->peek(Memory::KBD), (uint16_t)0);

  // should not be able to set keyboard from chip
  chip->set_load(true);
  chip->tick();
  ctx.expectEqual(kbd, (uint16_t)0);
  chip->tock();
  ctx.expectEqual(kbd, (uint16_t)0);
  ctx.expectEqual(chip->out(), (uint16_t)0);
  ctx.expectEqual(chip->peek(Memory::KBD), (uint16_t)0);

  // change in kbd should be reflected through Memory::out()
  chip->set_load(false);
  kbd = 0x41;
  chip->tick();
  ctx.expectEqual(chip->out(), kbd);
  chip->tock();
  ctx.expectEqual(chip->out(), kbd);
  ctx.expectEqual(chip->peek(Memory::KBD), kbd);

  // check lower memory hasn't changed
  chip->set_load(false);

  chip->set_address(0x1234);
  chip->tick();
  chip->tock();
  ctx.expectEqual(chip->out(), (uint16_t)1);
  ctx.expectEqual(chip->peek(0x1234), (uint16_t)1);
  chip->set_address(0x2345);
  chip->tick();
  chip->tock();
  ctx.expectEqual(chip->out(), (uint16_t)2);
  ctx.expectEqual(chip->peek(0x2345), (uint16_t)2);
  chip->set_address(0x3456);
  chip->tick();
  chip->tock();
  ctx.expectEqual(chip->out(), (uint16_t)3);
  ctx.expectEqual(chip->peek(0x3456), (uint16_t)3);
  chip->set_address(0x3fff);
  chip->tick();
  chip->tock();
  ctx.expectEqual(chip->out(), (uint16_t)4);
  ctx.expectEqual(chip->peek(0x3fff), (uint16_t)4);

  // test poke
  chip->set_load(false);

  chip->poke(0x1234, 10);
  ctx.expectEqual(chip->peek(0x1234), (uint16_t)10);
  chip->set_address(0x1234);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)10);
  chip->tock();
  ctx.expectEqual(chip->out(), (uint16_t)10);

  chip->poke(0x2345, 20);
  ctx.expectEqual(chip->peek(0x2345), (uint16_t)20);
  chip->set_address(0x2345);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)20);
  chip->tock();
  ctx.expectEqual(chip->out(), (uint16_t)20);

  chip->poke(0x3456, 30);
  ctx.expectEqual(chip->peek(0x3456), (uint16_t)30);
  chip->set_address(0x3456);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)30);
  chip->tock();
  ctx.expectEqual(chip->out(), (uint16_t)30);

  chip->poke(0x3fff, 40);
  ctx.expectEqual(chip->peek(0x3fff), (uint16_t)40);
  chip->set_address(0x3fff);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)40);
  chip->tock();
  ctx.expectEqual(chip->out(), (uint16_t)40);

  chip->poke(0x4fff, 50);
  ctx.expectEqual(chip->peek(0x4fff), (uint16_t)50);
  chip->set_address(0x4fff);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)50);
  chip->tock();
  ctx.expectEqual(chip->out(), (uint16_t)50);

  chip->poke(Memory::KBD, 60);
  ctx.expectEqual(chip->peek(Memory::KBD), (uint16_t)60);
  chip->set_address(Memory::KBD);
  chip->tick();
  ctx.expectEqual(chip->out(), (uint16_t)60);
  chip->tock();
  ctx.expectEqual(chip->out(), (uint16_t)60);

  delete chip;
}

void test_ROM32K(TestContext& ctx) {
  std::vector<uint16_t> instr = {
    0x1234, 0x2345, 0x3456, 0x4567
  };

  shallow::ROM32K chip;
  chip.set_rom(instr);

  for (int i = 0; i < instr.size(); i++) {
    chip.set_address(i);
    ctx.expectEqual(chip.instruction(), instr[i]);
  }

  chip.set_address(1000);
  ctx.expectEqual(chip.instruction(), (uint16_t)0);
}

void printSizes() {
  std::cout << "\nsize of Nand: "      << sizeof(Nand) << '\n'
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
            << "size of RAM16K: "  << sizeof(RAM16K) << '\n'
            << "size of Memory: "  << sizeof(Memory) << '\n'
            << "size of Computer: "  << sizeof(Computer) << '\n';
}

int main() {
  TestContext ctx;

  ctx.test("Nand", test_Nand);
  ctx.test("Nand16", test_Nand16);
  ctx.test("Not", test_Not);
  ctx.test("And", test_And);
  ctx.test("Or", test_Or);
  ctx.test("Xor", test_Xor);
  ctx.test("Mux", test_Mux);
  ctx.test("DMux", test_DMux);
  ctx.test("Not16", test_Not16);
  ctx.test("And16", test_And16);
  ctx.test("Or16", test_Or16);
  ctx.test("Mux16", test_Mux16);
  ctx.test("Or8Way", test_Or8Way);
  ctx.test("Mux4Way16", test_Mux4Way16);
  ctx.test("Mux8Way16", test_Mux8Way16);
  ctx.test("DMux4Way", test_DMux4Way);
  ctx.test("DMux8Way", test_DMux8Way);
  ctx.test("HalfAdder", test_HalfAdder);
  ctx.test("FullAdder", test_FullAdder);
  ctx.test("Add16", test_Add16);
  ctx.test("Inc16", test_Inc16);
  ctx.test("ALU", test_ALU);
  ctx.test("DFF", test_DFF);
  ctx.test("Bit", test_Bit);
  ctx.test("Register", test_Register);
  ctx.test("PC", test_PC);
  ctx.test("RAM8", [](auto& ctx){
    RAM8* chip = new RAM8();
    test_RAMn<RAM8>(ctx, *chip, 8);
    delete chip;
  });
  ctx.test("RAM64", [](auto& ctx){
    RAM64* chip = new RAM64();
    test_RAMn<RAM64>(ctx, *chip, 64);
    delete chip;
  });
  ctx.test("RAM512", [](auto& ctx){
    RAM512* chip = new RAM512();
    test_RAMn<RAM512>(ctx, *chip, 512);
    delete chip;
  });
  ctx.test("RAM4K", [](auto& ctx){
    RAM4K* chip = new RAM4K();
    test_RAMn<RAM4K>(ctx, *chip, 4096);
    delete chip;
  });
  ctx.test("RAM16K", [](auto& ctx){
    RAM16K* chip = new RAM16K();
    test_RAMn<RAM16K>(ctx, *chip, 16384);
    delete chip;
  });
  ctx.test("shallow::Screen", [](auto& ctx){
    uint16_t buf[shallow::Screen::SCREEN_SIZE] = {0};
    shallow::Screen* chip = new shallow::Screen(buf);
    test_RAMn<shallow::Screen>(ctx, *chip, 8192);
    delete chip;
  });
  ctx.test("Memory", test_Memory);
  ctx.test("shallow::ROM32K", test_ROM32K);

  // printSizes();
  std::cout << '\n';
  ctx.printResults();
}
