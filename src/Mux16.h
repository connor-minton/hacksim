#pragma once

#include <cstdint>
#include "Bits.h"

#include "ICombinationalCircuit.h"
#include "Not.h"
#include "And16.h"
#include "Or16.h"

class Mux16 : public ICombinationalCircuit {
public:
  // INPUT
  inline uint16_t a() const { return m_a; }
  inline uint16_t b() const { return m_b; }
  inline bool sel() const { return getBit<0>(m_pins); }

  inline void set_a(uint16_t val) { m_a = val; }
  inline void set_b(uint16_t val) { m_b = val; }
  inline void set_sel(bool val) { setBit<0>(m_pins, val); }

  // OUTPUT
  inline uint16_t out() const { return m_out; }

  Mux16() { computeOutput(); }

  inline void computeOutput() {
    m_notSel.set_in(sel());
    m_notSel.computeOutput();

    m_aOut.set_a(a());
    m_aOut.set_b(m_notSel.out() * 0xffff);
    m_aOut.computeOutput();

    m_bOut.set_a(b());
    m_bOut.set_b(sel() * 0xffff);
    m_bOut.computeOutput();

    m_orOut.set_a(m_aOut.out());
    m_orOut.set_b(m_bOut.out());
    m_orOut.computeOutput();

    m_out = m_orOut.out();
  }

private:
  // extra pins
  // { m_sel }
  uint8_t m_pins = 0;
  uint16_t m_a = 0;
  uint16_t m_b = 0;
  uint16_t m_out = 0;

  Not m_notSel;
  And16 m_aOut;
  And16 m_bOut;
  Or16 m_orOut;
};
