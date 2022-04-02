#pragma once

#include <cstdint>
#include "Bits.h"

#include "ICombinationalCircuit.h"
#include "Not.h"
#include "And.h"

/**
 * DMux
 *
 * IN  in, sel
 * OUT a, b
 *
 * Demultiplexor:
 * {a, b} = {in, 0} if sel == 0
 *          {0, in} if sel == 1
 */

class DMux : public ICombinationalCircuit {
public:
  // INPUT
  inline bool in() const { return getBit<0>(pins); }
  inline bool sel() const { return getBit<1>(pins); }

  inline void set_in(bool val) { setBit<0>(pins, val); }
  inline void set_sel(bool val) { setBit<1>(pins, val); }

  // OUTPUT
  inline bool a() const { return getBit<2>(pins); }
  inline bool b() const { return getBit<3>(pins); }

  DMux() { computeOutput(); }

  void computeOutput() {
    m_notSel.set_in(sel());
    m_notSel.computeOutput();
    m_andA.set_a(in());
    m_andA.set_b(m_notSel.out());
    m_andA.computeOutput();
    m_andB.set_a(in());
    m_andB.set_b(sel());
    m_andB.computeOutput();
    set_a(m_andA.out());
    set_b(m_andB.out());
  }

private:
  // pins layout
  //  0   1    2  3
  // {in, sel, a, b}
  uint8_t pins = 0;

  // internal components
  Not m_notSel;
  And m_andA;
  And m_andB;

  inline void set_a(bool val) { setBit<2>(pins, val); }
  inline void set_b(bool val) { setBit<3>(pins, val); }
};

