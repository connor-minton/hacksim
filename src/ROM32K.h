#pragma once

#include <cstdint>
#include <vector>

#include "ISequentialCircuit.h"

namespace shallow {

class ROM32K : public ISequentialCircuit {
public:
  // IN address[15]
  inline uint16_t address() const { return m_address; }

  inline void set_address(uint16_t val) { m_address = val & 0x7fff; }

  // OUT out[16]
  inline uint16_t instruction() const {
    if (address() < m_instructions.size())
      return m_instructions[address()];
    return 0;
  }

  inline uint16_t instructionAt(uint16_t addr) const {
    if (addr < m_instructions.size())
      return m_instructions[addr];
    return 0;
  }

  inline std::vector<uint16_t> get_rom() const { return m_instructions; }

  inline void set_rom(std::vector<uint16_t> instructions)
    { m_instructions = instructions; }

  inline void tick() { }
  inline void tock() { }

private:
  std::vector<uint16_t> m_instructions;
  uint16_t m_address = 0;
};

}
