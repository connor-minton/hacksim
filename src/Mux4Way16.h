#pragma once

#include <cstdint>

#include "Mux16.h"

class Mux4Way16 {
public:
  // INPUT a[16], b[16], c[16], d[16], sel[2]
  inline uint16_t a() const { return m_a; }
  inline uint16_t b() const { return m_b; }
  inline uint16_t c() const { return m_c; }
  inline uint16_t d() const { return m_d; }
  inline uint16_t sel() const { return m_pins & 0x3; }

  inline void set_a(uint16_t val) { m_a = val; }
  inline void set_b(uint16_t val) { m_b = val; }
  inline void set_c(uint16_t val) { m_c = val; }
  inline void set_d(uint16_t val) { m_d = val; }
  inline void set_sel(uint8_t val) {
    m_pins = (m_pins & 0xfc) | (val & 0x3);
  }

  // OUTPUT out[16]
  inline uint16_t out() const { return m_out; }

  Mux4Way16() { computeOutput(); }

  inline void computeOutput() {
    m_ab.set_a(m_a);
    m_ab.set_b(m_b);
    m_ab.set_sel(getBit<0>(m_pins));
    m_ab.computeOutput();

    m_cd.set_a(m_c);
    m_cd.set_b(m_d);
    m_cd.set_sel(getBit<0>(m_pins));
    m_cd.computeOutput();

    m_muxOut.set_a(m_ab.out());
    m_muxOut.set_b(m_cd.out());
    m_muxOut.set_sel(getBit<1>(m_pins));
    m_muxOut.computeOutput();

    m_out = m_muxOut.out();
  }

private:
  // { sel[0..1] }
  uint8_t m_pins = 0;
  uint16_t m_a = 0;
  uint16_t m_b = 0;
  uint16_t m_c = 0;
  uint16_t m_d = 0;
  uint16_t m_out = 0;

  Mux16 m_ab;
  Mux16 m_cd;
  Mux16 m_muxOut;
};

