#pragma once

#include <cstdint>

#include "Bits.h"

class Nand {
public:
  // INPUT
  inline bool a() const { return getBit<0>(pins); }
  inline bool b() const { return getBit<1>(pins); }

  inline void set_a(bool val) { setBit<0>(pins, val); }
  inline void set_b(bool val) { setBit<1>(pins, val); }

  // OUTPUT
  inline bool out() const { return (pins & 0x4) != 0; }

  Nand() { computeOutput(); }

  inline void computeOutput() {
    set_out(!(a() && b()));
  }

private:
  // { a, b, out }
  uint8_t pins = 0;

  inline void set_out(bool val) { setBit<2>(pins, val); }
};

