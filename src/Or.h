#pragma once

#include <cstdint>
#include "Bits.h"

#include "ICombinationalCircuit.h"
#include "Not.h"
#include "Nand.h"

class Or : public ICombinationalCircuit {
public:
  // INPUT
  inline bool a() const { return getBit<0>(pins); }
  inline bool b() const { return getBit<1>(pins); }

  inline void set_a(bool val) { setBit<0>(pins, val); }
  inline void set_b(bool val) { setBit<1>(pins, val); }

  // OUTPUT
  inline bool out() const { return getBit<2>(pins); }

  Or() { computeOutput(); }

  void computeOutput() {
    m_notA.set_in(a());
    m_notB.set_in(b());
    m_notA.computeOutput();
    m_notB.computeOutput();
    m_nand.set_a(m_notA.out());
    m_nand.set_b(m_notB.out());
    m_nand.computeOutput();
    set_out(m_nand.out());
  }

private:
  // {a, b, out}
  uint8_t pins = 0;

  Not m_notA;
  Not m_notB;
  Nand m_nand;

  inline void set_out(bool val) { setBit<2>(pins, val); }
};
