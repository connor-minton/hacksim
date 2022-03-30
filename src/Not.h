#pragma once

#include <cstdint>

#include "Bits.h"

#include "ICombinationalCircuit.h"
#include "Nand.h"

class Not : public ICombinationalCircuit {
public:
  // INPUT
  inline bool in() const { return getBit<0>(pins); }

  inline void set_in(bool val) {
    if (val) pins |= 0x1;
    else     pins &= ~0x1;
  }

  // OUTPUT
  inline bool out() const { return getBit<1>(pins); }

  Not() { computeOutput(); }

  void computeOutput() {
    m_nand.set_a(in());
    m_nand.set_b(in());
    m_nand.computeOutput();
    set_out(m_nand.out());
  }

private:
  // { in, out }
  uint8_t pins = 0;

  Nand m_nand;

  inline void set_out(bool val) { setBit<1>(pins, val); }
};

