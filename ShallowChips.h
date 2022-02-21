#pragma once

#include <vector>

#include "Bits.h"

namespace shallow {

class Screen {
public:
  static const size_t SCREEN_SIZE = 8192;

  // IN in[16], load, address[13]
  inline uint16_t in() const { return m_in; }
  inline bool load() const { return getBit<0>(m_pins); }
  inline uint16_t address() const { return (m_pins & 0x3ffe) >> 1; }

  inline void set_in(uint16_t val) { m_in = val; }
  inline void set_load(bool val) { setBit<0>(m_pins, val); }
  inline void set_address(uint16_t val) {
    m_pins = (m_pins & 0xc000) | ((val & 0x1fff) << 1) | (m_pins & 0x1);
  }

  // OUT out[16]
  inline uint16_t out() const { return m_out; }

  inline void tock() {
    if (load()) {
      m_screen[address()] = m_in;
    }
    m_out = m_screen[address()];
  }

private:
  // { load, address[0..12] }
  uint16_t m_pins = 0;
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  uint16_t m_screen[SCREEN_SIZE] = {0};
};

class ROM32K {
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

  ROM32K(std::vector<uint16_t> instructions)
    : m_instructions(instructions) { }

private:
  std::vector<uint16_t> m_instructions;
  uint16_t m_address = 0;
};

}