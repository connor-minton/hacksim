#pragma once

#include <cstdint>
#include "Bits.h"

#include "ISequentialCircuit.h"
#include "DMux8Way.h"
#include "Mux8Way16.h"
#include "Register.h"

class RAM8 : public ISequentialCircuit {
public:
  RAM8() {
    for (int i = 0; i < 8; i++) {
      m_regs[i] = new shallow::Register();
    }
  }

  ~RAM8() {
    for (int i = 0; i < 8; i++) {
      delete m_regs[i];
    }
  }

  // INPUT in[16], load, address[3]
  inline uint16_t in() const { return m_in; }
  inline bool load() const { return getBit<0>(m_pins); }
  inline uint8_t address() const { return (m_pins & 0xe) >> 1; }

  inline void set_in(uint16_t val) { m_in = val; }
  inline void set_load(bool val) { setBit<0>(m_pins, val); }
  inline void set_address(uint8_t val) {
    m_pins = (m_pins & 0xf0) | ((val & 0x7) << 1) | (m_pins & 0x1);
  }

  // OUTPUT out[16]
  inline uint16_t out() const { return m_out; }

  // Bypass the clocking system to examine the value at `offset`
  inline uint16_t peek(uint16_t offset) const {
    return m_regs[offset & 0x7]->out();
  }

  // Bypass the clocking system to forcefully set the underlying value
  // at `offset` to `val`
  inline void poke(uint16_t offset, uint16_t val) {
    m_regs[offset & 0x7]->poke(val);
  }

  inline void tick() {
    m_dmux.set_in(load());
    m_dmux.set_sel(address());
    m_dmux.computeOutput();

    m_regs[0]->set_in(in());
    m_regs[1]->set_in(in());
    m_regs[2]->set_in(in());
    m_regs[3]->set_in(in());
    m_regs[4]->set_in(in());
    m_regs[5]->set_in(in());
    m_regs[6]->set_in(in());
    m_regs[7]->set_in(in());

    m_regs[0]->set_load(m_dmux.a());
    m_regs[1]->set_load(m_dmux.b());
    m_regs[2]->set_load(m_dmux.c());
    m_regs[3]->set_load(m_dmux.d());
    m_regs[4]->set_load(m_dmux.e());
    m_regs[5]->set_load(m_dmux.f());
    m_regs[6]->set_load(m_dmux.g());
    m_regs[7]->set_load(m_dmux.h());

    // optimization: only tick-tock the selected register
    m_regs[m_dmux.sel()]->tick();

    m_mux.set_sel(address());
    m_mux.set_a(m_regs[0]->out());
    m_mux.set_b(m_regs[1]->out());
    m_mux.set_c(m_regs[2]->out());
    m_mux.set_d(m_regs[3]->out());
    m_mux.set_e(m_regs[4]->out());
    m_mux.set_f(m_regs[5]->out());
    m_mux.set_g(m_regs[6]->out());
    m_mux.set_h(m_regs[7]->out());
    m_mux.computeOutput();

    m_out = m_mux.out();
  }

  inline void tock() {
    m_regs[m_dmux.sel()]->tock();

    // duplicate logic from tick, but necessary after tocking the reg
    m_mux.set_sel(address());
    m_mux.set_a(m_regs[0]->out());
    m_mux.set_b(m_regs[1]->out());
    m_mux.set_c(m_regs[2]->out());
    m_mux.set_d(m_regs[3]->out());
    m_mux.set_e(m_regs[4]->out());
    m_mux.set_f(m_regs[5]->out());
    m_mux.set_g(m_regs[6]->out());
    m_mux.set_h(m_regs[7]->out());
    m_mux.computeOutput();

    m_out = m_mux.out();
  }

private:
  // { load, address[0..2] }
  uint8_t m_pins = 0;
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  DMux8Way m_dmux;
  Mux8Way16 m_mux;
  IRegister* m_regs[8] = {0};
};

