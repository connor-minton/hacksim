#pragma once

#include <cstdint>
#include "Bits.h"

#include "ISequentialCircuit.h"

class DFF : public ISequentialCircuit {
public:
  // INPUT in
  inline bool in() const { return getBit<0>(m_pins); }

  inline void set_in(bool val) { setBit<0>(m_pins, val); }

  // OUTPUT out
  inline bool out() const { return getBit<1>(m_pins); }

  inline void poke(bool val) { set_out(val); }

  DFF() { tock(); }

  inline void tick() { }

  inline void tock() {
    set_out(in());
  }

private:
  // { in, out }
  uint8_t m_pins = 0;

  inline void set_out(bool val) { setBit<1>(m_pins, val); }
};

