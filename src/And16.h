#pragma once

#include <cstdint>

#include "ICombinationalCircuit.h"
#include "Nand16.h"
#include "Not16.h"

class And16 : public ICombinationalCircuit {
public:
  // INPUT
  inline uint16_t a() const { return m_a; }
  inline uint16_t b() const { return m_b; }

  inline void set_a(uint16_t val) { m_a = val; }
  inline void set_b(uint16_t val) { m_b = val; }

  // OUTPUT
  inline uint16_t out() const { return m_out; }

  And16() { computeOutput(); }

  inline void computeOutput() {
    m_nand.set_a(a());
    m_nand.set_b(b());
    m_nand.computeOutput();
    m_not.set_in(m_nand.out());
    m_not.computeOutput();
    m_out = m_not.out();
  }

private:
  uint16_t m_a = 0;
  uint16_t m_b = 0;
  uint16_t m_out = 0;

  Nand16 m_nand;
  Not16 m_not;
};

