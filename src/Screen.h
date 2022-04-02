#pragma once

#include <cstdint>
#include "Bits.h"

#include "ISequentialCircuit.h"

namespace shallow {

/**
 * shallow::Screen
 *
 * IN  in[16], load, address[13]
 * OUT out[16]
 *
 * The Screen (memory map).
 * Functions exactly like a 16-bit 8K RAM:
 *    1. out(t)=Screen[address(t)](t)
 *    2. If load(t-1) then Screen[address(t-1)](t)=in(t-1)
 *
 * The built-in chip implementation has the side effect of continuously
 * refreshing a visual 256 by 512 black-and-white screen, simulated
 * by the simulator. Each row in the visual screen is represented
 * by 32 consecutive 16-bit words, starting at the top left corner
 * of the visual screen. Thus the pixel at row r from the top and
 * column c from the left (0<=r<=255, 0<=c<=511) reflects the c%16
 * bit (counting from LSB to MSB) of the word found in
 * Screen[r*32+c/16].
 */
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
  inline uint16_t peek(uint16_t offset) const { return m_screen[offset & 0x1fff]; }

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
  // m_pins layout
  //   0     1..13
  // { load, address[0..12] }
  uint16_t m_pins = 0;
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  // internal components
  uint16_t* m_screen;
};


}
