#pragma once

#include <cstdint>
#include <vector>

#include "ISequentialCircuit.h"

namespace shallow {

/**
 * shallow::ROM32K
 *
 * IN  address[15]
 * OUT instruction[16]
 *
 * Read-Only memory (ROM) of 32K registers, each 16-bit wide.
 * The chip is designed to facilitate data read, as follows:
 *     instruction(t) = ROM32K[address(t)](t)
 * In words: the chip always outputs the value stored at the
 * memory location specified by address.
 *
 * Shallow, because it would be more or less implemented like the RAM.
 *
 * The tick() and tock() functions do nothing. The user of ROM32K can
 * access an instruction immediately after setting the address with
 * set_address(). Alternatively, the user may call instructionAt(addr).
 */
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

  /**
   * Gets a copy of the data stored in ROM.
   */
  inline std::vector<uint16_t> get_rom() const { return m_instructions; }

  /**
   * Sets the data in the ROM unit to a copy of `instructions`.
   */
  inline void set_rom(std::vector<uint16_t> instructions)
    { m_instructions = instructions; }

  inline void tick() { }
  inline void tock() { }

private:
  std::vector<uint16_t> m_instructions;
  uint16_t m_address = 0;
};

}
