#pragma once

#include <cassert>
#include <cstdint>
#include <vector>
#include <string>
#include "FileUtils.h"

#include "ISequentialCircuit.h"
#include "Memory.h"
#include "CPU.h"
#include "ShallowChips.h"

class Computer : public ISequentialCircuit {
public:
  // IN reset
  inline bool reset() const { return m_reset; }
  inline void set_reset(bool val) { m_reset = val; }

  Computer(uint16_t* screen, uint16_t* kbd)
    : m_mem(screen, kbd) { }

  inline void set_rom(const std::vector<uint16_t>& instructions)
    { m_rom.set_rom(instructions); }

  inline Memory& mem() { return m_mem; }

  uint16_t curInstruction() const;
  uint16_t nextInstruction() const;

  std::string curInstrDbg() const;
  std::string nextInstrDbg() const;

  uint16_t curPC() const;
  uint16_t nextPC() const;

  inline void tick() {
    m_rom.set_address(m_cpu.pc());

    m_mem.set_address(m_cpu.addressM());
    m_mem.tick();

    uint16_t memOut = m_mem.out();

    // m_rom is a shallow chip, so it is not necessary to call
    // m_rom.computeOutput() before observing its output instruction
    m_cpu.set_instruction(m_rom.instruction());
    m_cpu.set_reset(reset());
    m_cpu.set_inM(m_mem.out());
    m_cpu.tick();

    // outM and writeM are updated by the CPU tick()
    m_mem.set_in(m_cpu.outM());
    m_mem.set_load(m_cpu.writeM());
    m_mem.tick();

    assert(m_mem.out() == memOut);
    // If this assertion failed, it would mean that we would have to
    // `tick` the CPU again, entering a feedback loop. If the Memory
    // unit is implemented correctly, then m_mem.out() will be
    // different only if m_mem.set_address() caused the selected
    // memory register to be different before the tick.
  }

  inline void tock() {
    m_mem.tock();
    m_cpu.tock();
  }

private:
  bool m_reset = false;

  shallow::ROM32K m_rom;
  Memory          m_mem;
  CPU             m_cpu;
};

uint16_t Computer::curInstruction() const { return m_rom.instruction(); }
uint16_t Computer::nextInstruction() const
  { return m_rom.instructionAt(m_cpu.pc()); }

std::string Computer::curInstrDbg() const
  { return FileUtils::disassemble(curInstruction()); }

std::string Computer::nextInstrDbg() const
  { return FileUtils::disassemble(nextInstruction()); }

uint16_t Computer::curPC() const { return m_rom.address(); }
uint16_t Computer::nextPC() const { return m_cpu.pc(); }
