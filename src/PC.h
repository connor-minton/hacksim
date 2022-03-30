#pragma once

#include <cstdint>
#include "Bits.h"

#include "ISequentialCircuit.h"
#include "Inc16.h"
#include "Mux16.h"
#include "Register.h"

class PC : public ISequentialCircuit {
public:
  // INPUT in[16], load, inc, reset
  inline uint16_t in() const { return m_in; }
  inline bool load() const { return getBit<0>(m_pins); }
  inline bool inc() const { return getBit<1>(m_pins); }
  inline bool reset() const { return getBit<2>(m_pins); }

  inline void set_in(uint16_t val) { m_in = val; }
  inline void set_load(bool val) { setBit<0>(m_pins, val); }
  inline void set_inc(bool val) { setBit<1>(m_pins, val); }
  inline void set_reset(bool val) { setBit<2>(m_pins, val); }

  // OUTPUT out[16]
  inline uint16_t out() const { return m_out; }

  PC() { m_reg.set_load(true); }

  void tick() {
    m_inc.set_in(m_reg.out());
    m_inc.computeOutput();

    m_incMux.set_a(m_reg.out());
    m_incMux.set_b(m_inc.out());
    m_incMux.set_sel(inc());
    m_incMux.computeOutput();

    m_loadMux.set_a(m_incMux.out());
    m_loadMux.set_b(in());
    m_loadMux.set_sel(load());
    m_loadMux.computeOutput();

    m_resetMux.set_a(m_loadMux.out());
    m_resetMux.set_b(0);
    m_resetMux.set_sel(reset());
    m_resetMux.computeOutput();

    m_reg.set_in(m_resetMux.out());
    m_reg.tick();
  }

  void tock() {
    m_reg.tock();
    m_out = m_reg.out();
  }

private:
  // { load, inc, reset }
  uint8_t m_pins = 0;
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  Inc16 m_inc;
  Mux16 m_incMux;
  Mux16 m_loadMux;
  Mux16 m_resetMux;
  Register m_reg;
};


