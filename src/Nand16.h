#pragma once

#include <cstdint>

class Nand16 {
public:
  // IN a[16], b[16]
  inline uint16_t a() const { return m_a; }
  inline uint16_t b() const { return m_b; }

  inline void set_a(uint16_t val) { m_a = val; }
  inline void set_b(uint16_t val) { m_b = val; }

  // OUT out[16]
  inline uint16_t out() const { return m_out; }

  Nand16() { computeOutput(); }

  inline void computeOutput() {
    m_out = ~(a() & b());
  }

private:
  uint16_t m_a = 0;
  uint16_t m_b = 0;
  uint16_t m_out = 0;
};

