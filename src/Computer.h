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

class IComputer : public ISequentialCircuit {
public:
  // IN reset
  virtual bool reset() const = 0;
  virtual void set_reset(bool val) = 0;

  virtual void set_rom(const std::vector<uint16_t>& instructions) = 0;

  virtual IMemory& mem() = 0;

  virtual uint16_t curInstruction() const = 0;
  virtual uint16_t nextInstruction() const = 0;

  virtual std::string curInstrDbg() const = 0;
  virtual std::string nextInstrDbg() const = 0;
  
  virtual uint16_t curPC() const = 0;
  virtual uint16_t nextPC() const = 0;

  virtual ~IComputer() { }
};

class BaseComputer : public IComputer {
public:
  // IN reset
  inline bool reset() const { return m_reset; }
  inline void set_reset(bool val) { m_reset = val; }

  BaseComputer(IMemory* mem, ICPU* cpu_) : m_mem(mem), m_cpu(cpu_) { }

  virtual ~BaseComputer() { }

  inline void set_rom(const std::vector<uint16_t>& instructions)
    { m_rom.set_rom(instructions); }

  inline IMemory& mem() { return *m_mem; }

  uint16_t curInstruction() const;
  uint16_t nextInstruction() const;

  std::string curInstrDbg() const;
  std::string nextInstrDbg() const;

  uint16_t curPC() const;
  uint16_t nextPC() const;

  inline void tick() {
    m_rom.set_address(m_cpu->pc());

    m_mem->set_address(m_cpu->addressM());
    m_mem->tick();

    uint16_t memOut = m_mem->out();

    // m_rom is a shallow chip, so it is not necessary to call
    // m_rom.computeOutput() before observing its output instruction
    m_cpu->set_instruction(m_rom.instruction());
    m_cpu->set_reset(reset());
    m_cpu->set_inM(m_mem->out());
    m_cpu->tick();

    // outM and writeM are updated by the CPU tick()
    m_mem->set_in(m_cpu->outM());
    m_mem->set_load(m_cpu->writeM());
    m_mem->tick();

    assert(m_mem.out() == memOut);
    // If this assertion failed, it would mean that we would have to
    // `tick` the CPU again, entering a feedback loop. If the Memory
    // unit is implemented correctly, then m_mem.out() will be
    // different only if m_mem.set_address() caused the selected
    // memory register to be different before the tick.
  }

  inline void tock() {
    m_mem->tock();
    m_cpu->tock();
  }

protected:
  bool m_reset = false;

  shallow::ROM32K m_rom;
  IMemory*        m_mem = nullptr;
  ICPU*           m_cpu = nullptr;
};

uint16_t BaseComputer::curInstruction() const { return m_rom.instruction(); }
uint16_t BaseComputer::nextInstruction() const
  { return m_rom.instructionAt(m_cpu->pc()); }

std::string BaseComputer::curInstrDbg() const
  { return FileUtils::disassemble(curInstruction()); }

std::string BaseComputer::nextInstrDbg() const
  { return FileUtils::disassemble(nextInstruction()); }

uint16_t BaseComputer::curPC() const { return m_rom.address(); }
uint16_t BaseComputer::nextPC() const { return m_cpu->pc(); }

class Computer : public BaseComputer {
public:
  Computer(uint16_t* screen, uint16_t* kbd)
    : BaseComputer(new Memory(screen, kbd), new CPU())
  { }

  ~Computer() {
    delete m_mem;
  }
};

namespace shallow {

class Computer : public BaseComputer {
public:
  Computer(uint16_t* screen, uint16_t* kbd, uint16_t* clk)
    : BaseComputer(new shallow::Memory(screen, kbd, clk), new shallow::CPU())
  { }

  ~Computer() {
    delete m_mem;
  }
};

}

