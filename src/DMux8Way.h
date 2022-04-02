#pragma once

#include <cstdint>
#include "Bits.h"

#include "ICombinationalCircuit.h"
#include "DMux.h"
#include "DMux4Way.h"

/**
 * DMux8Way
 *
 * IN  in, sel
 * OUT a, b, c, d, e, f, g, h
 *
 * 8-way demultiplexor:
 * {a, b, c, d, e, f, g, h} = {in, 0, 0, 0, 0, 0, 0, 0} if sel == 000
 *                            {0, in, 0, 0, 0, 0, 0, 0} if sel == 001
 *                            etc.
 *                            {0, 0, 0, 0, 0, 0, 0, in} if sel == 111
 */

class DMux8Way : public ICombinationalCircuit {
public:
  DMux8Way() { computeOutput(); }

  // INPUT in, sel[3]
  inline bool in() const { return getBit<0>(m_pins); }
  inline uint8_t sel() const { return (m_pins & 0xe) >> 1; }

  inline void set_in(bool val) { setBit<0>(m_pins, val); }
  inline void set_sel(uint8_t val) {
    m_pins = (m_pins & 0xfff0) | ((val & 0x7) << 1) | (m_pins & 0x1);
  }

  // OUTPUT a, b, c, d, e, f, g, h
  inline bool a() const { return getBit<4>(m_pins); }
  inline bool b() const { return getBit<5>(m_pins); }
  inline bool c() const { return getBit<6>(m_pins); }
  inline bool d() const { return getBit<7>(m_pins); }
  inline bool e() const { return getBit<8>(m_pins); }
  inline bool f() const { return getBit<9>(m_pins); }
  inline bool g() const { return getBit<10>(m_pins); }
  inline bool h() const { return getBit<11>(m_pins); }

  void computeOutput() {
    m_mux.set_in(in());
    m_mux.set_sel(getBit<3>(m_pins));
    m_mux.computeOutput();

    m_mux4abcd.set_in(m_mux.a());
    m_mux4abcd.set_sel(sel() & 0x3);
    m_mux4abcd.computeOutput();

    m_mux4efgh.set_in(m_mux.b());
    m_mux4efgh.set_sel(sel() & 0x3);
    m_mux4efgh.computeOutput();

    set_a(m_mux4abcd.a());
    set_b(m_mux4abcd.b());
    set_c(m_mux4abcd.c());
    set_d(m_mux4abcd.d());
    set_e(m_mux4efgh.a());
    set_f(m_mux4efgh.b());
    set_g(m_mux4efgh.c());
    set_h(m_mux4efgh.d());
  }

private:
  // m_pins layout
  //   0   1..3       4  5  ... 10 11
  // { in, sel[0..2], a, b, ..., g, h }
  uint16_t m_pins;

  DMux m_mux;
  DMux4Way m_mux4abcd;
  DMux4Way m_mux4efgh;

  inline void set_a(bool val) { setBit<4>(m_pins, val); }
  inline void set_b(bool val) { setBit<5>(m_pins, val); }
  inline void set_c(bool val) { setBit<6>(m_pins, val); }
  inline void set_d(bool val) { setBit<7>(m_pins, val); }
  inline void set_e(bool val) { setBit<8>(m_pins, val); }
  inline void set_f(bool val) { setBit<9>(m_pins, val); }
  inline void set_g(bool val) { setBit<10>(m_pins, val); }
  inline void set_h(bool val) { setBit<11>(m_pins, val); }
};

