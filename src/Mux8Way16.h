#pragma once

#include <cstdint>

#include "ICombinationalCircuit.h"
#include "Mux4Way16.h"
#include "Mux16.h"

class Mux8Way16 : public ICombinationalCircuit {
public:
  // INPUT a[16], b[16], c[16], d[16],
  //       e[16], f[16], g[16], h[16], sel[3]
  inline uint16_t a() const { return m_a; }
  inline uint16_t b() const { return m_b; }
  inline uint16_t c() const { return m_c; }
  inline uint16_t d() const { return m_d; }
  inline uint16_t e() const { return m_e; }
  inline uint16_t f() const { return m_f; }
  inline uint16_t g() const { return m_g; }
  inline uint16_t h() const { return m_h; }
  inline uint8_t sel() const { return (m_pins & 0x7); }

  inline void set_a(uint16_t val) { m_a = val; }
  inline void set_b(uint16_t val) { m_b = val; }
  inline void set_c(uint16_t val) { m_c = val; }
  inline void set_d(uint16_t val) { m_d = val; }
  inline void set_e(uint16_t val) { m_e = val; }
  inline void set_f(uint16_t val) { m_f = val; }
  inline void set_g(uint16_t val) { m_g = val; }
  inline void set_h(uint16_t val) { m_h = val; }
  inline void set_sel(uint8_t val) {
    m_pins = (m_pins & 0xf8) | (val & 0x7);
  }

  // OUTPUT out[16]
  inline uint16_t out() const { return m_out; }

  Mux8Way16() { computeOutput(); }

  void computeOutput() {
    m_abcd.set_a(m_a);
    m_abcd.set_b(m_b);
    m_abcd.set_c(m_c);
    m_abcd.set_d(m_d);
    m_abcd.set_sel(m_pins & 0x3);
    m_abcd.computeOutput();

    m_efgh.set_a(m_e);
    m_efgh.set_b(m_f);
    m_efgh.set_c(m_g);
    m_efgh.set_d(m_h);
    m_efgh.set_sel(m_pins & 0x3);
    m_efgh.computeOutput();

    m_muxOut.set_a(m_abcd.out());
    m_muxOut.set_b(m_efgh.out());
    m_muxOut.set_sel(getBit<2>(m_pins));
    m_muxOut.computeOutput();

    m_out = m_muxOut.out();
  }

private:
  // { sel[0..2] }
  uint8_t m_pins = 0;
  uint16_t m_a = 0;
  uint16_t m_b = 0;
  uint16_t m_c = 0;
  uint16_t m_d = 0;
  uint16_t m_e = 0;
  uint16_t m_f = 0;
  uint16_t m_g = 0;
  uint16_t m_h = 0;
  uint16_t m_out = 0;

  Mux4Way16 m_abcd;
  Mux4Way16 m_efgh;
  Mux16 m_muxOut;
};

