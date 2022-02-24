#include "Test.h"
#include "FileUtils.h"
#include "Chips.h"

void test_Computer_Max(TestContext& ctx) {
  uint16_t screen[shallow::Screen::SCREEN_SIZE] = {0};
  uint16_t kbd = 0;

  Computer* computer = new Computer(screen, &kbd);

  auto rom = FileUtils::readHackFile("test_programs/asm/Max.hack");

  computer->set_rom(rom);

  computer->set_reset(true);
  computer->tock();

  Memory& mem = computer->mem();
  mem.poke(0, 3);
  mem.poke(1, 5);

  computer->set_reset(false);
  for (int i = 0; i < 14; i++) {
    computer->tock();
  }

  ctx.expectEqual(mem.peek(0), 3);
  ctx.expectEqual(mem.peek(1), 5);
  ctx.expectEqual(mem.peek(2), 5);

  computer->set_reset(true);
  computer->tock();

  computer->set_reset(false);
  mem.poke(0, 23456);
  mem.poke(1, 12345);
  for (int i = 0; i < 10; i++) {
    computer->tock();
  }

  ctx.expectEqual(mem.peek(0), 23456);
  ctx.expectEqual(mem.peek(1), 12345);
  ctx.expectEqual(mem.peek(2), 23456);

  delete computer;
}

void test_Computer_Add(TestContext& ctx) {
  uint16_t screen[shallow::Screen::SCREEN_SIZE] = {0};
  uint16_t kbd = 0;

  Computer* computer = new Computer(screen, &kbd);

  auto rom = FileUtils::readHackFile("test_programs/asm/Add.hack");

  computer->set_rom(rom);

  computer->set_reset(true);
  computer->tock();
  computer->set_reset(false);

  Memory& mem = computer->mem();
  mem.poke(0, 0);

  for (int i = 0; i < 6; i++) {
    computer->tock();
  }

  ctx.expectEqual(mem.peek(0), 5);
  ctx.expectEqual(mem.peek(1), 0);
  ctx.expectEqual(mem.peek(2), 0);

  delete computer;
}

void test_Computer_TickTock(TestContext& cx) {
  uint16_t screen[shallow::Screen::SCREEN_SIZE] = {0};
  uint16_t kbd = 0;

  Computer* computer = new Computer(screen, &kbd);

  auto rom = FileUtils::readHackFile("test_programs/asm/TickTock.hack");

  computer->set_rom(rom);

  computer->set_reset(true);
  computer->tock();
  computer->set_reset(false);

  Memory& mem = computer->mem();
  mem.poke(0, 0);
  mem.poke(100, 0);

  for (int i = 0; i < 8; i++) {
    computer->tock();
  }

  cx.expectEqual(mem.peek(0), 51);
  cx.expectEqual(mem.peek(100), 50);

  delete computer;
}

void test_Computer_Rect(TestContext& cx) {
  uint16_t screen[shallow::Screen::SCREEN_SIZE] = {0};
  uint16_t kbd = 0;

  Computer* computer = new Computer(screen, &kbd);

  auto rom = FileUtils::readHackFile("test_programs/asm/Rect.hack");

  computer->set_rom(rom);

  Memory& mem = computer->mem();
  mem.poke(0, 4); // 16 pixels wide, 4 pixels long

  for (int i = 0; i < 63; i++) {
    computer->tock();
  }

  cx.expectEqual(screen[0], 0xffff);
  cx.expectEqual(screen[32], 0xffff);
  cx.expectEqual(screen[64], 0xffff);
  cx.expectEqual(screen[96], 0xffff);

  cx.expectEqual(screen[1], 0);
  cx.expectEqual(screen[33], 0);
  cx.expectEqual(screen[65], 0);
  cx.expectEqual(screen[97], 0);
  cx.expectEqual(screen[128], 0);

  delete computer;
}

void test_Computer_BasicTest(TestContext& cx) {
  uint16_t screen[shallow::Screen::SCREEN_SIZE] = {0};
  uint16_t kbd = 0;

  Computer* computer = new Computer(screen, &kbd);

  auto rom = FileUtils::readHackFile("test_programs/vm/BasicTest.hack");

  computer->set_rom(rom);

  Memory& mem = computer->mem();
  mem.poke(0, 256);
  mem.poke(1, 300);
  mem.poke(2, 400);
  mem.poke(3, 3000);
  mem.poke(4, 3010);

  for (int i = 0; i < 600; i++) {
    computer->tock();
  }

  cx.expectEqual(mem.peek(256), 472);
  cx.expectEqual(mem.peek(300), 10);
  cx.expectEqual(mem.peek(401), 21);
  cx.expectEqual(mem.peek(402), 22);
  cx.expectEqual(mem.peek(3006), 36);
  cx.expectEqual(mem.peek(3012), 42);
  cx.expectEqual(mem.peek(3015), 45);
  cx.expectEqual(mem.peek(11), 510);

  delete computer;
}

void test_Computer_PointerTest(TestContext& cx) {
  uint16_t screen[shallow::Screen::SCREEN_SIZE] = {0};
  uint16_t kbd = 0;

  Computer* computer = new Computer(screen, &kbd);

  auto rom = FileUtils::readHackFile("test_programs/vm/PointerTest.hack");

  computer->set_rom(rom);

  Memory& mem = computer->mem();
  mem.poke(0, 256);

  for (int i = 0; i < 450; i++) {
    computer->tock();
  }

  cx.expectEqual(mem.peek(256), 6084);
  cx.expectEqual(mem.peek(3), 3030);
  cx.expectEqual(mem.peek(4), 3040);
  cx.expectEqual(mem.peek(3032), 32);
  cx.expectEqual(mem.peek(3046), 46);

  delete computer;
}

void test_Computer_StaticTest(TestContext& cx) {
  uint16_t screen[shallow::Screen::SCREEN_SIZE] = {0};
  uint16_t kbd = 0;

  Computer* computer = new Computer(screen, &kbd);

  auto rom = FileUtils::readHackFile("test_programs/vm/StaticTest.hack");

  computer->set_rom(rom);

  Memory& mem = computer->mem();
  mem.poke(0, 256);

  for (int i = 0; i < 200; i++) {
    computer->tock();
  }

  cx.expectEqual(mem.peek(256), 1110);

  delete computer;
}

void test_Computer_SimpleAdd(TestContext& cx) {
  uint16_t screen[shallow::Screen::SCREEN_SIZE] = {0};
  uint16_t kbd = 0;

  Computer* computer = new Computer(screen, &kbd);

  auto rom = FileUtils::readHackFile("test_programs/vm/SimpleAdd.hack");

  computer->set_rom(rom);

  Memory& mem = computer->mem();
  mem.poke(0, 256);

  for (int i = 0; i < 60; i++) {
    computer->tock();
  }

  cx.expectEqual(mem.peek(0), 257);
  cx.expectEqual(mem.peek(256), 15);

  delete computer;
}

int main() {
  TestContext cx;

  cx.test("Computer / asm / Max", test_Computer_Max);
  cx.test("Computer / asm / Add", test_Computer_Add);
  cx.test("Computer / asm / TickTock", test_Computer_TickTock);
  cx.test("Computer / asm / Rect", test_Computer_Rect);

  cx.test("Computer / vm / BasicTest", test_Computer_BasicTest);
  cx.test("Computer / vm / PointerTest", test_Computer_PointerTest);
  cx.test("Computer / vm / StaticTest", test_Computer_StaticTest);
  cx.test("Computer / vm / SimpleAdd", test_Computer_SimpleAdd);

  std::cout << '\n';

  cx.printResults();
}