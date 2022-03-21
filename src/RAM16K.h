#pragma once

#include <cstdint>
#include "Bits.h"

#include "DMux4Way.h"
#include "Mux4Way16.h"
#include "RAM4K.h"

class RAM16K {
public:
  // INPUT in[16], load, address[14]
  inline uint16_t in() const { return m_in; }
  inline bool load() const { return getBit<0>(m_pins); }
  inline uint16_t address() const { return (m_pins & 0x7ffe) >> 1; }

  inline void set_in(uint16_t val) { m_in = val; }
  inline void set_load(bool val) { setBit<0>(m_pins, val); }
  inline void set_address(uint16_t val) {
    m_pins = (m_pins & 0x8000) | ((val & 0x3fff) << 1) | (m_pins & 0x1);
  }

  // OUTPUT out[16]
  inline uint16_t out() const { return m_out; }

  // Bypass the clocking system to examine the value at `offset`
  inline uint16_t peek(uint16_t offset) {
    return m_rams[(offset & 0x3000) >> 12].peek(offset & 0xfff);
  }

  // Bypass the clocking system to forcefully set the underlying value
  // at `offset` to `val`
  inline void poke(uint16_t offset, uint16_t val) {
    m_rams[(offset & 0x3000) >> 12].poke(offset & 0xfff, val);
  }

  inline void tick() {
    m_dmux.set_in(load());
    m_dmux.set_sel((address() & 0x3000) >> 12);
    m_dmux.computeOutput();

    m_rams[0].set_in(in());
    m_rams[1].set_in(in());
    m_rams[2].set_in(in());
    m_rams[3].set_in(in());

    m_rams[0].set_load(m_dmux.a());
    m_rams[1].set_load(m_dmux.b());
    m_rams[2].set_load(m_dmux.c());
    m_rams[3].set_load(m_dmux.d());

    m_rams[0].set_address(address() & 0xfff);
    m_rams[1].set_address(address() & 0xfff);
    m_rams[2].set_address(address() & 0xfff);
    m_rams[3].set_address(address() & 0xfff);

    // optimization: only tick() the selected ram module
    m_rams[m_dmux.sel()].tick();

    m_mux.set_sel((address() & 0x3000) >> 12);
    m_mux.set_a(m_rams[0].out());
    m_mux.set_b(m_rams[1].out());
    m_mux.set_c(m_rams[2].out());
    m_mux.set_d(m_rams[3].out());
    m_mux.computeOutput();

    m_out = m_mux.out();
  }

  inline void tock() {
    // optimization: only tock() the selected ram module
    m_rams[m_dmux.sel()].tock();

    m_mux.set_sel((address() & 0x3000) >> 12);
    m_mux.set_a(m_rams[0].out());
    m_mux.set_b(m_rams[1].out());
    m_mux.set_c(m_rams[2].out());
    m_mux.set_d(m_rams[3].out());
    m_mux.computeOutput();

    m_out = m_mux.out();
  }

private:
  // { load, address[0..13] }
  uint16_t m_pins = 0;
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  DMux4Way m_dmux;
  Mux4Way16 m_mux;
  RAM4K m_rams[4];
};

