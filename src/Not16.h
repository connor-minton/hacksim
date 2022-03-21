#pragma once

#include <cstdint>

#include "Nand16.h"

class Not16 {
public:
  // INPUT
  inline uint16_t in() const { return m_in; }

  inline void set_in(uint16_t val) { m_in = val; }

  // OUTPUT
  inline uint16_t out() const { return m_out; }

  Not16() { computeOutput(); }

  inline void computeOutput() {
    m_nand.set_a(in());
    m_nand.set_b(in());
    m_nand.computeOutput();
    m_out = m_nand.out();
  }

private:
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  Nand16 m_nand;
};

