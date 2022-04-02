#pragma once

#include <cstdint>

#include "ICombinationalCircuit.h"
#include "Not16.h"
#include "Nand16.h"

/**
 * Or16
 *
 * IN  a[16], b[16]
 * OUT out[16]
 *
 * 16-bit bitwise Or:
 * for i = 0..15 out[i] = (a[i] or b[i])
 */
class Or16 : public ICombinationalCircuit {
public:
  // INPUT
  inline uint16_t a() const { return m_a; }
  inline uint16_t b() const { return m_b; }

  inline void set_a(uint16_t val) { m_a = val; }
  inline void set_b(uint16_t val) { m_b = val; }

  // OUTPUT
  inline uint16_t out() const { return m_out; }

  Or16() { computeOutput(); }

  void computeOutput() {
    m_notA.set_in(a());
    m_notA.computeOutput();
    m_notB.set_in(b());
    m_notB.computeOutput();

    m_nand.set_a(m_notA.out());
    m_nand.set_b(m_notB.out());
    m_nand.computeOutput();

    m_out = m_nand.out();
  }

private:
  // pins
  uint16_t m_a = 0;
  uint16_t m_b = 0;
  uint16_t m_out = 0;

  // internal components
  Not16 m_notA;
  Not16 m_notB;
  Nand16 m_nand;
};

