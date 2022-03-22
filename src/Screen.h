#pragma once

#include <cstdint>
#include "Bits.h"

#include "ISequentialCircuit.h"

namespace shallow {

class Screen : public ISequentialCircuit {
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

  Screen(uint16_t* buf) : m_screen(buf) { }

  // Bypass the clocking system to examine the value at `offset`
  inline uint16_t peek(uint16_t offset) { return m_screen[offset & 0x1fff]; }

  // Bypass the clocking system to forcefully set the underlying value
  // at `offset` to `val`
  inline void poke(uint16_t offset, uint16_t val) {
    m_screen[offset & 0x1fff] = val;
  }

  inline void tick() {
    m_out = m_screen[address()];
  }

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

  uint16_t* m_screen;
};


}
