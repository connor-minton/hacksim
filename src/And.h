#pragma once

#include <cstdint>
#include "Bits.h"

#include "ICombinationalCircuit.h"
#include "Nand.h"
#include "Not.h"

/**
 * And
 *
 * IN  a, b
 * OUT out
 *
 * out = 1 if (a == 1 and b == 1)
 *       0 otherwise
 */
class And : public ICombinationalCircuit {
public:
  // INPUT
  inline bool a() const { return getBit<0>(pins); }
  inline bool b() const { return getBit<1>(pins); }

  inline void set_a(bool val) { setBit<0>(pins, val); }
  inline void set_b(bool val) { setBit<1>(pins, val); }

  // OUTPUT
  inline bool out() const { return getBit<2>(pins); }

  And() { computeOutput(); }

  void computeOutput() {
    m_nand.set_a(a());
    m_nand.set_b(b());
    m_nand.computeOutput();
    m_not.set_in(m_nand.out());
    m_not.computeOutput();
    set_out(m_not.out());
  }

private:
  // pins layout
  //  0  1  2
  // {a, b, out}
  uint8_t pins = 0;

  // internal components
  Nand m_nand;
  Not m_not;

  inline void set_out(bool val) { setBit<2>(pins, val); }
};

