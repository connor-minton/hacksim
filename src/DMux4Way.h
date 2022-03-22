#pragma once

#include <cstdint>
#include "Bits.h"

#include "ICombinationalCircuit.h"
#include "DMux.h"

class DMux4Way : public ICombinationalCircuit {
public:
  // INPUT in, sel[2]
  inline bool in() const { return getBit<0>(m_pins); }
  inline uint8_t sel() const { return (m_pins & 0x6) >> 1; }

  inline void set_in(bool val) { setBit<0>(m_pins, val); }
  inline void set_sel(uint8_t val) {
    m_pins = (m_pins & 0xf8) | ((val & 0x3) << 1) | (m_pins & 1);
  }

  // OUTPUT a, b, c, d
  inline bool a() const { return getBit<3>(m_pins); }
  inline bool b() const { return getBit<4>(m_pins); }
  inline bool c() const { return getBit<5>(m_pins); }
  inline bool d() const { return getBit<6>(m_pins); }

  DMux4Way() { computeOutput(); }

  inline void computeOutput() {
    m_dm1.set_in(in());
    m_dm1.set_sel(getBit<2>(m_pins));
    m_dm1.computeOutput();

    m_dm2.set_in(m_dm1.a());
    m_dm2.set_sel(getBit<1>(m_pins));
    m_dm2.computeOutput();

    m_dm3.set_in(m_dm1.b());
    m_dm3.set_sel(getBit<1>(m_pins));
    m_dm3.computeOutput();

    set_a(m_dm2.a());
    set_b(m_dm2.b());
    set_c(m_dm3.a());
    set_d(m_dm3.b());
  }

private:
  // { in, sel[0..1], a, b, c, d }
  uint8_t m_pins = 0;

  DMux m_dm1;
  DMux m_dm2;
  DMux m_dm3;

  inline void set_a(bool val) { setBit<3>(m_pins, val); }
  inline void set_b(bool val) { setBit<4>(m_pins, val); }
  inline void set_c(bool val) { setBit<5>(m_pins, val); }
  inline void set_d(bool val) { setBit<6>(m_pins, val); }
};

