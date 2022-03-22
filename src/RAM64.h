#pragma once

#include <cstdint>
#include "Bits.h"

#include "ISequentialCircuit.h"
#include "DMux8Way.h"
#include "Mux8Way16.h"
#include "RAM8.h"

class RAM64 : public ISequentialCircuit {
public:
  // INPUT in[16], load, address[6]
  inline uint16_t in() const { return m_in; }
  inline bool load() const { return getBit<0>(m_pins); }
  inline uint8_t address() const { return (m_pins & 0x7e) >> 1; }

  inline void set_in(uint16_t val) { m_in = val; }
  inline void set_load(bool val) { setBit<0>(m_pins, val); }
  inline void set_address(uint8_t val) {
    m_pins = (m_pins & 0x80) | ((val & 0x3f) << 1) | (m_pins & 0x1);
  }

  // OUTPUT out[16]
  inline uint16_t out() const { return m_out; }

  // Bypass the clocking system to examine the value at `offset`
  inline uint16_t peek(uint16_t offset) {
    return m_rams[(offset & 0x38) >> 3].peek(offset & 0x7);
  }

  // Bypass the clocking system to forcefully set the underlying value
  // at `offset` to `val`
  inline void poke(uint16_t offset, uint16_t val) {
    m_rams[(offset & 0x38) >> 3].poke(offset & 0x7, val);
  }

  inline void tick() {
    m_dmux.set_in(load());
    m_dmux.set_sel((address() & 0x38) >> 3);
    m_dmux.computeOutput();

    m_rams[0].set_in(in());
    m_rams[1].set_in(in());
    m_rams[2].set_in(in());
    m_rams[3].set_in(in());
    m_rams[4].set_in(in());
    m_rams[5].set_in(in());
    m_rams[6].set_in(in());
    m_rams[7].set_in(in());

    m_rams[0].set_load(m_dmux.a());
    m_rams[1].set_load(m_dmux.b());
    m_rams[2].set_load(m_dmux.c());
    m_rams[3].set_load(m_dmux.d());
    m_rams[4].set_load(m_dmux.e());
    m_rams[5].set_load(m_dmux.f());
    m_rams[6].set_load(m_dmux.g());
    m_rams[7].set_load(m_dmux.h());

    m_rams[0].set_address(address() & 0x7);
    m_rams[1].set_address(address() & 0x7);
    m_rams[2].set_address(address() & 0x7);
    m_rams[3].set_address(address() & 0x7);
    m_rams[4].set_address(address() & 0x7);
    m_rams[5].set_address(address() & 0x7);
    m_rams[6].set_address(address() & 0x7);
    m_rams[7].set_address(address() & 0x7);

    // optimization: only tick() the selected ram module
    m_rams[m_dmux.sel()].tick();

    m_mux.set_sel((address() & 0x38) >> 3);
    m_mux.set_a(m_rams[0].out());
    m_mux.set_b(m_rams[1].out());
    m_mux.set_c(m_rams[2].out());
    m_mux.set_d(m_rams[3].out());
    m_mux.set_e(m_rams[4].out());
    m_mux.set_f(m_rams[5].out());
    m_mux.set_g(m_rams[6].out());
    m_mux.set_h(m_rams[7].out());
    m_mux.computeOutput();

    m_out = m_mux.out();
  }

  inline void tock() {
    // optimization: only tock() the selected ram module
    m_rams[m_dmux.sel()].tock();

    m_mux.set_sel((address() & 0x38) >> 3);
    m_mux.set_a(m_rams[0].out());
    m_mux.set_b(m_rams[1].out());
    m_mux.set_c(m_rams[2].out());
    m_mux.set_d(m_rams[3].out());
    m_mux.set_e(m_rams[4].out());
    m_mux.set_f(m_rams[5].out());
    m_mux.set_g(m_rams[6].out());
    m_mux.set_h(m_rams[7].out());
    m_mux.computeOutput();

    m_out = m_mux.out();
  }

private:
  // { load, address[0..5] }
  uint8_t m_pins = 0;
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  DMux8Way m_dmux;
  Mux8Way16 m_mux;
  RAM8 m_rams[8];
};

