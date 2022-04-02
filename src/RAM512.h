#pragma once

#include <cstdint>
#include "Bits.h"

#include "ISequentialCircuit.h"
#include "DMux8Way.h"
#include "Mux8Way16.h"
#include "RAM64.h"

/**
 * RAM512
 *
 * IN  in[16], load, address[9]
 * OUT out[16]
 *
 * Memory of 512 registers, each 16 bit-wide. Out holds the value
 * stored at the memory location specified by address. If load==1, then
 * the in value is loaded into the memory location specified by address
 * (the loaded value will be emitted to out from the next time step onward).
 */
class RAM512 : public ISequentialCircuit {
public:
  // INPUT in[16], load, address[9]
  inline uint16_t in() const { return m_in; }
  inline bool load() const { return getBit<0>(m_pins); }
  inline uint16_t address() const { return (m_pins & 0x3fe) >> 1; }

  inline void set_in(uint16_t val) { m_in = val; }
  inline void set_load(bool val) { setBit<0>(m_pins, val); }
  inline void set_address(uint16_t val) {
    m_pins = (m_pins & 0xfc00) | ((val & 0x1ff) << 1) | (m_pins & 0x1);
  }

  // OUTPUT out[16]
  inline uint16_t out() const { return m_out; }

  // Bypass the clocking system to examine the value at `offset`
  inline uint16_t peek(uint16_t offset) const {
    return m_rams[(offset & 0x1c0) >> 6].peek(offset & 0x3f);
  }

  // Bypass the clocking system to forcefully set the underlying value
  // at `offset` to `val`
  inline void poke(uint16_t offset, uint16_t val) {
    m_rams[(offset & 0x1c0) >> 6].poke(offset & 0x3f, val);
  }

  void tick() {
    m_dmux.set_in(load());
    m_dmux.set_sel((address() & 0x1c0) >> 6);
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

    m_rams[0].set_address(address() & 0x3f);
    m_rams[1].set_address(address() & 0x3f);
    m_rams[2].set_address(address() & 0x3f);
    m_rams[3].set_address(address() & 0x3f);
    m_rams[4].set_address(address() & 0x3f);
    m_rams[5].set_address(address() & 0x3f);
    m_rams[6].set_address(address() & 0x3f);
    m_rams[7].set_address(address() & 0x3f);

    // optimization: only tick() the selected ram module
    m_rams[m_dmux.sel()].tick();

    m_mux.set_sel((address() & 0x1c0) >> 6);
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

  void tock() {
    // optimization: only tock() the selected ram module
    m_rams[m_dmux.sel()].tock();

    m_mux.set_sel((address() & 0x1c0) >> 6);
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
  // m_pins layout
  //   0     1..9
  // { load, address[0..8] }
  uint16_t m_pins = 0;
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  // internal components
  DMux8Way m_dmux;
  Mux8Way16 m_mux;
  RAM64 m_rams[8];
};

