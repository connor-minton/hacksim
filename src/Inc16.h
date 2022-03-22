#pragma once

#include <cstdint>

#include "ICombinationalCircuit.h"
#include "Add16.h"

class Inc16 : public ICombinationalCircuit {
public:
  // INPUT in[16]
  inline uint16_t in() const { return m_in; }

  inline void set_in(uint16_t val) { m_in = val; }

  // OUTPUT out[16]
  inline uint16_t out() const { return m_out; }

  Inc16() { computeOutput(); }

  inline void computeOutput() {
    m_add.set_a(m_in);
    m_add.set_b(1);
    m_add.computeOutput();
    m_out = m_add.out();
  }

private:
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  Add16 m_add;
};

