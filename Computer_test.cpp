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

  auto rom = FileUtils::readHackFile("test_programs/vm/BasicTest/BasicTest.hack");

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

  auto rom = FileUtils::readHackFile("test_programs/vm/PointerTest/PointerTest.hack");

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

  auto rom = FileUtils::readHackFile("test_programs/vm/StaticTest/StaticTest.hack");

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

  auto rom = FileUtils::readHackFile("test_programs/vm/SimpleAdd/SimpleAdd.hack");

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

void test_Computer_StackTest(TestContext& cx) {
  uint16_t screen[shallow::Screen::SCREEN_SIZE] = {0};
  uint16_t kbd = 0;

  Computer* computer = new Computer(screen, &kbd);

  auto rom = FileUtils::readHackFile("test_programs/vm/StackTest/StackTest.hack");

  computer->set_rom(rom);

  Memory& mem = computer->mem();
  mem.poke(0, 256);

  for (int i = 0; i < 1000; i++) {
    computer->tock();
  }

  cx.expectEqual((int16_t)mem.peek(0), 266);
  cx.expectEqual((int16_t)mem.peek(256), -1);
  cx.expectEqual((int16_t)mem.peek(257), 0);
  cx.expectEqual((int16_t)mem.peek(258), 0);
  cx.expectEqual((int16_t)mem.peek(259), 0);
  cx.expectEqual((int16_t)mem.peek(260), -1);
  cx.expectEqual((int16_t)mem.peek(261), 0);
  cx.expectEqual((int16_t)mem.peek(262), -1);
  cx.expectEqual((int16_t)mem.peek(263), 0);
  cx.expectEqual((int16_t)mem.peek(264), 0);
  cx.expectEqual((int16_t)mem.peek(265), -91);

  delete computer;
}

void test_Computer_FibonacciElement(TestContext& cx) {
  uint16_t screen[shallow::Screen::SCREEN_SIZE] = {0};
  uint16_t kbd = 0;

  Computer* computer = new Computer(screen, &kbd);

  auto rom = FileUtils::readHackFile("test_programs/vm/FibonacciElement/FibonacciElement.hack");

  computer->set_rom(rom);

  Memory& mem = computer->mem();

  for (int i = 0; i < 6000; i++) {
    computer->tock();
  }

  cx.expectEqual((int16_t)mem.peek(0), 262);
  cx.expectEqual((int16_t)mem.peek(261), 3);

  delete computer;
}

void test_Computer_NestedCall(TestContext& cx) {
  uint16_t screen[shallow::Screen::SCREEN_SIZE] = {0};
  uint16_t kbd = 0;

  Computer* computer = new Computer(screen, &kbd);

  auto rom = FileUtils::readHackFile("test_programs/vm/NestedCall/NestedCall.hack");

  computer->set_rom(rom);

  Memory& mem = computer->mem();
  mem.poke(0, 261);
  mem.poke(1, 261);
  mem.poke(2, 256);
  mem.poke(3, -3);
  mem.poke(4, -4);
  mem.poke(5, -1);
  mem.poke(6, -1);
  mem.poke(256, 1234);
  mem.poke(257, -1);
  mem.poke(258, -2);
  mem.poke(259, -3);
  mem.poke(260, -4);

  for (int i = 261; i < 300; i++) {
    mem.poke(i, -1);
  }

  for (int i = 0; i < 4000; i++) {
    computer->tock();
  }

  cx.expectEqual((int16_t)mem.peek(0), 261);
  cx.expectEqual((int16_t)mem.peek(1), 261);
  cx.expectEqual((int16_t)mem.peek(2), 256);
  cx.expectEqual((int16_t)mem.peek(3), 4000);
  cx.expectEqual((int16_t)mem.peek(4), 5000);
  cx.expectEqual((int16_t)mem.peek(5), 135);
  cx.expectEqual((int16_t)mem.peek(6), 246);

  delete computer;
}

void test_Computer_SimpleFunction(TestContext& cx) {
  uint16_t screen[shallow::Screen::SCREEN_SIZE] = {0};
  uint16_t kbd = 0;

  Computer* computer = new Computer(screen, &kbd);

  auto rom = FileUtils::readHackFile("test_programs/vm/SimpleFunction/SimpleFunction.hack");

  computer->set_rom(rom);

  Memory& mem = computer->mem();
  mem.poke(0, 317);
  mem.poke(1, 317);
  mem.poke(2, 310);
  mem.poke(3, 3000);
  mem.poke(4, 4000);
  mem.poke(310, 1234);
  mem.poke(311, 37);
  mem.poke(312, 1000);
  mem.poke(313, 305);
  mem.poke(314, 300);
  mem.poke(315, 3010);
  mem.poke(316, 4010);

  for (int i = 0; i < 300; i++) {
    computer->tock();
  }

  cx.expectEqual((int16_t)mem.peek(0), 311);
  cx.expectEqual((int16_t)mem.peek(1), 305);
  cx.expectEqual((int16_t)mem.peek(2), 300);
  cx.expectEqual((int16_t)mem.peek(3), 3010);
  cx.expectEqual((int16_t)mem.peek(4), 4010);
  cx.expectEqual((int16_t)mem.peek(310), 1196);

  delete computer;
}

void test_Computer_StaticsTest(TestContext& cx) {
  uint16_t screen[shallow::Screen::SCREEN_SIZE] = {0};
  uint16_t kbd = 0;

  Computer* computer = new Computer(screen, &kbd);

  auto rom = FileUtils::readHackFile("test_programs/vm/StaticsTest/StaticsTest.hack");

  computer->set_rom(rom);

  Memory& mem = computer->mem();
  mem.poke(0, 256);

  for (int i = 0; i < 2500; i++) {
    computer->tock();
  }

  cx.expectEqual((int16_t)mem.peek(0), 263);
  cx.expectEqual((int16_t)mem.peek(261), -2);
  cx.expectEqual((int16_t)mem.peek(262), 8);

  delete computer;
}

void test_Computer_BasicLoop(TestContext& cx) {
  uint16_t screen[shallow::Screen::SCREEN_SIZE] = {0};
  uint16_t kbd = 0;

  Computer* computer = new Computer(screen, &kbd);

  auto rom = FileUtils::readHackFile("test_programs/vm/BasicLoop/BasicLoop.hack");

  computer->set_rom(rom);

  Memory& mem = computer->mem();
  mem.poke(0, 256);
  mem.poke(1, 300);
  mem.poke(2, 400);
  mem.poke(400, 3);

  for (int i = 0; i < 600; i++) {
    computer->tock();
  }

  cx.expectEqual((int16_t)mem.peek(0), 257);
  cx.expectEqual((int16_t)mem.peek(256), 6);

  delete computer;
}

void test_Computer_FibonacciSeries(TestContext& cx) {
  uint16_t screen[shallow::Screen::SCREEN_SIZE] = {0};
  uint16_t kbd = 0;

  Computer* computer = new Computer(screen, &kbd);

  auto rom = FileUtils::readHackFile("test_programs/vm/FibonacciSeries/FibonacciSeries.hack");

  computer->set_rom(rom);

  Memory& mem = computer->mem();
  mem.poke(0, 256);
  mem.poke(1, 300);
  mem.poke(2, 400);
  mem.poke(400, 7);
  mem.poke(401, 3000);

  for (int i = 0; i < 1100; i++) {
    computer->tock();
  }

  cx.expectEqual((int16_t)mem.peek(3000), 0);
  cx.expectEqual((int16_t)mem.peek(3001), 1);
  cx.expectEqual((int16_t)mem.peek(3002), 1);
  cx.expectEqual((int16_t)mem.peek(3003), 2);
  cx.expectEqual((int16_t)mem.peek(3004), 3);
  cx.expectEqual((int16_t)mem.peek(3005), 5);

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
  cx.test("Computer / vm / StackTest", test_Computer_StackTest);
  cx.test("Computer / vm / FibonacciElement", test_Computer_FibonacciElement);
  cx.test("Computer / vm / NestedCall", test_Computer_NestedCall);
  cx.test("Computer / vm / SimpleFunction", test_Computer_SimpleFunction);
  cx.test("Computer / vm / StaticsTest", test_Computer_StaticsTest);
  cx.test("Computer / vm / BasicLoop", test_Computer_BasicLoop);
  cx.test("Computer / vm / FibonacciSeries", test_Computer_FibonacciSeries);

  std::cout << '\n';

  cx.printResults();
}