#pragma once

#include <cstdint>
#include "Bits.h"

#include "Not.h"
#include "Nand.h"

class Xor {
public:
  // INPUT
  inline bool a() const { return getBit<0>(pins); }
  inline bool b() const { return getBit<1>(pins); }

  inline void set_a(bool val) { setBit<0>(pins, val); }
  inline void set_b(bool val) { setBit<1>(pins, val); }

  // OUTPUT
  inline bool out() const { return getBit<2>(pins); }

  Xor() { computeOutput(); }

  inline void computeOutput() {
    m_notA.set_in(a());
    m_notA.computeOutput();
    m_notB.set_in(b());
    m_notB.computeOutput();
    m_nand1.set_a(a());
    m_nand1.set_b(m_notB.out());
    m_nand1.computeOutput();
    m_nand2.set_a(m_notA.out());
    m_nand2.set_b(b());
    m_nand2.computeOutput();
    m_nandOut.set_a(m_nand1.out());
    m_nandOut.set_b(m_nand2.out());
    m_nandOut.computeOutput();
    set_out(m_nandOut.out());
  }

private:
  // {a, b, out}
  uint8_t pins = 0;

  Not m_notA;
  Not m_notB;
  Nand m_nand1;
  Nand m_nand2;
  Nand m_nandOut;

  inline void set_out(bool val) { setBit<2>(pins, val); }
};
