#pragma once

#include <cstdint>

#include "ICombinationalCircuit.h"
#include "Nand16.h"

/**
 * Not16
 *
 * IN  in
 * OUT out
 *
 * 16-bit Not:
 * for i=0..15: out[i] = not in[i]
 */
class Not16 : public ICombinationalCircuit {
public:
  // INPUT
  inline uint16_t in() const { return m_in; }

  inline void set_in(uint16_t val) { m_in = val; }

  // OUTPUT
  inline uint16_t out() const { return m_out; }

  Not16() { computeOutput(); }

  void computeOutput() {
    m_nand.set_a(in());
    m_nand.set_b(in());
    m_nand.computeOutput();
    m_out = m_nand.out();
  }

private:
  // pins
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  // internal components
  Nand16 m_nand;
};

