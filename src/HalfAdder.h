#pragma once

#include <cstdint>
#include "Bits.h"

#include "ICombinationalCircuit.h"
#include "Xor.h"
#include "And.h"

/**
 * HalfAdder
 *
 * IN  a, b
 * OUT sum, carry
 *
 * Computes the sum of two bits.
 */

class HalfAdder : public ICombinationalCircuit {
public:
  // INPUT a, b
  inline bool a() const { return getBit<0>(m_pins); }
  inline bool b() const { return getBit<1>(m_pins); }

  inline void set_a(bool val) { setBit<0>(m_pins, val); }
  inline void set_b(bool val) { setBit<1>(m_pins, val); }

  // OUTPUT sum, carry
  inline bool sum() const { return getBit<2>(m_pins); }
  inline bool carry() const { return getBit<3>(m_pins); }

  HalfAdder() { computeOutput(); }

  void computeOutput() {
    m_xor.set_a(a());
    m_xor.set_b(b());
    m_xor.computeOutput();
    set_sum(m_xor.out());

    m_and.set_a(a());
    m_and.set_b(b());
    m_and.computeOutput();
    set_carry(m_and.out());
  }

private:
  // m_pins layout
  //   0  1  2    3
  // { a, b, sum, carry }
  uint8_t m_pins = 0;

  Xor m_xor;
  And m_and;

  inline void set_sum(bool val) { setBit<2>(m_pins, val); }
  inline void set_carry(bool val) { setBit<3>(m_pins, val); }
};

