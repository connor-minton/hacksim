#pragma once

#include <cstdint>
#include "Bits.h"

#include "Or.h"

class Or8Way {
public:
  // INPUT in[8]
  inline uint8_t in() const { return m_in; }

  inline void set_in(uint8_t val) { m_in = val; }

  // OUTPUT out
  inline bool out() const { return getBit<0>(m_pins); }

  Or8Way() { computeOutput(); }

  inline void computeOutput() {
    m_or01.set_a(getBit<0>(m_in));
    m_or01.set_b(getBit<1>(m_in));
    m_or01.computeOutput();

    m_or23.set_a(getBit<2>(m_in));
    m_or23.set_b(getBit<3>(m_in));
    m_or23.computeOutput();

    m_or45.set_a(getBit<4>(m_in));
    m_or45.set_b(getBit<5>(m_in));
    m_or45.computeOutput();

    m_or67.set_a(getBit<6>(m_in));
    m_or67.set_b(getBit<7>(m_in));
    m_or67.computeOutput();

    m_or0123.set_a(m_or01.out());
    m_or0123.set_b(m_or23.out());
    m_or0123.computeOutput();

    m_or4567.set_a(m_or45.out());
    m_or4567.set_b(m_or67.out());
    m_or4567.computeOutput();

    m_orOut.set_a(m_or0123.out());
    m_orOut.set_b(m_or4567.out());
    m_orOut.computeOutput();

    set_out(m_orOut.out());
  }

private:
  // { out }
  uint8_t m_pins = 0;
  uint8_t m_in = 0;

  Or m_or01;
  Or m_or23;
  Or m_or45;
  Or m_or67;

  Or m_or0123;
  Or m_or4567;

  Or m_orOut;

  inline void set_out(bool val) { setBit<0>(m_pins, val); }
};

