#pragma once

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

}