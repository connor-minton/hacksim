#pragma once

#include <cstdint>
#include "Bits.h"

#include "ICombinationalCircuit.h"
#include "Not.h"
#include "And.h"
#include "Or.h"

class Mux : public ICombinationalCircuit {
public:
  // INPUT
  inline bool a() const { return getBit<0>(pins); }
  inline bool b() const { return getBit<1>(pins); }
  inline bool sel() const { return getBit<2>(pins); }

  inline void set_a(bool val) { setBit<0>(pins, val); }
  inline void set_b(bool val) { setBit<1>(pins, val); }
  inline void set_sel(bool val) { setBit<2>(pins, val); }

  // OUTPUT
  inline bool out() const { return getBit<3>(pins); }

  Mux() { computeOutput(); }

  inline void computeOutput() {
    m_notSel.set_in(sel());
    m_notSel.computeOutput();
    m_andA.set_a(a());
    m_andA.set_b(m_notSel.out());
    m_andA.computeOutput();
    m_andB.set_a(b());
    m_andB.set_b(sel());
    m_andB.computeOutput();
    m_orOut.set_a(m_andA.out());
    m_orOut.set_b(m_andB.out());
    m_orOut.computeOutput();
    set_out(m_orOut.out());
  }

private:
  // {a, b, sel, out}
  uint8_t pins = 0;

  Not m_notSel;
  And m_andA;
  And m_andB;
  Or m_orOut;

  inline void set_out(bool val) { setBit<3>(pins, val); }
};
