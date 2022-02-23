#include "Test.h"
#include "FileUtils.h"
#include "Chips.h"

void test_Computer_Max(TestContext& ctx) {
  uint16_t screen[shallow::Screen::SCREEN_SIZE] = {0};
  uint16_t kbd = 0;

  Computer* computer = new Computer(screen, &kbd);

  auto rom = FileUtils::readHackFile("hack/Max.hack");

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

  auto rom = FileUtils::readHackFile("hack/Add.hack");

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

  auto rom = FileUtils::readHackFile("hack/TickTock.hack");

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

  auto rom = FileUtils::readHackFile("hack/Rect.hack");

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

int main() {
  TestContext cx;

  cx.test("Computer / Max", test_Computer_Max);
  cx.test("Computer / Add", test_Computer_Add);
  cx.test("Computer / TickTock", test_Computer_TickTock);
  cx.test("Computer / Rect", test_Computer_Rect);

  std::cout << '\n';

  cx.printResults();
}