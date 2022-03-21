#pragma once

#include <cstdint>
#include "Bits.h"

#include "Xor.h"
#include "And.h"
#include "Or.h"

class FullAdder {
public:
  // INPUT a, b, c
  inline bool a() const { return getBit<0>(m_pins); }
  inline bool b() const { return getBit<1>(m_pins); }
  inline bool c() const { return getBit<2>(m_pins); }

  inline void set_a(bool val) { setBit<0>(m_pins, val); }
  inline void set_b(bool val) { setBit<1>(m_pins, val); }
  inline void set_c(bool val) { setBit<2>(m_pins, val); }

  // OUTPUT sum, carry
  inline bool sum() const { return getBit<3>(m_pins); }
  inline bool carry() const { return getBit<4>(m_pins); }

  FullAdder() { computeOutput(); }

  inline void computeOutput() {
    m_axb.set_a(a());
    m_axb.set_b(b());
    m_axb.computeOutput();

    m_xSum.set_a(m_axb.out());
    m_xSum.set_b(c());
    m_xSum.computeOutput();

    m_axbnc.set_a(m_axb.out());
    m_axbnc.set_b(c());
    m_axbnc.computeOutput();

    m_anb.set_a(a());
    m_anb.set_b(b());
    m_anb.computeOutput();

    m_orCarry.set_a(m_axbnc.out());
    m_orCarry.set_b(m_anb.out());
    m_orCarry.computeOutput();

    set_sum(m_xSum.out());
    set_carry(m_orCarry.out());
  }

private:
  // { a, b, c, sum, carry }
  uint8_t m_pins = 0;

  Xor m_axb;
  Xor m_xSum;

  And m_axbnc;
  And m_anb;

  Or m_orCarry;

  inline void set_sum(bool val) { setBit<3>(m_pins, val); }
  inline void set_carry(bool val) { setBit<4>(m_pins, val); }
};

