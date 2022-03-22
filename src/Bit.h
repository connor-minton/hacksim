#pragma once

#include <cstdint>
#include "Bits.h"

#include "ISequentialCircuit.h"
#include "Mux.h"
#include "DFF.h"

class Bit : public ISequentialCircuit {
public:
  // INPUT in, load
  inline bool in() const { return getBit<0>(m_pins); }
  inline bool load() const { return getBit<1>(m_pins); }

  inline void set_in(bool val) { setBit<0>(m_pins, val); }
  inline void set_load(bool val) { setBit<1>(m_pins, val); }

  // OUTPUT out
  inline bool out() const { return getBit<2>(m_pins); }

  // Bypass the clocking system to forcefully set the underlying value
  // to `val`
  inline void poke(bool val) { m_dff.poke(val); set_out(val); }

  Bit() { tock(); }

  inline void tick() {
    m_mux.set_a(m_dff.out());
    m_mux.set_b(in());
    m_mux.set_sel(load());
    m_mux.computeOutput();

    m_dff.set_in(m_mux.out());
  }

  inline void tock() {
    m_dff.tock();
    set_out(m_dff.out());
  }

private:
  // { in, load, out }
  uint8_t m_pins = 0;

  Mux m_mux;
  DFF m_dff;

  inline void set_out(bool val) { setBit<2>(m_pins, val); }
};

