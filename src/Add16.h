#pragma once

#include <cstdint>
#include "Bits.h"

#include "ICombinationalCircuit.h"
#include "HalfAdder.h"
#include "FullAdder.h"

/**
 * Adds two 16-bit values.
 * The most significant carry bit is ignored.
 *
 * IN  a[16], b[16]
 * OUT out[16]
 */
class Add16 : public ICombinationalCircuit {
public:
  // INPUT a[16], b[16]
  inline uint16_t a() const { return m_a; }
  inline uint16_t b() const { return m_b; }

  inline void set_a(uint16_t val) { m_a = val; }
  inline void set_b(uint16_t val) { m_b = val; }

  // OUTPUT out[16]
  inline uint16_t out() const { return m_out; }

  Add16() { computeOutput(); }

  void computeOutput() {
    m_ha.set_a(getBit<0>(m_a));
    m_fa[0].set_a(getBit<1>(m_a));
    m_fa[1].set_a(getBit<2>(m_a));
    m_fa[2].set_a(getBit<3>(m_a));
    m_fa[3].set_a(getBit<4>(m_a));
    m_fa[4].set_a(getBit<5>(m_a));
    m_fa[5].set_a(getBit<6>(m_a));
    m_fa[6].set_a(getBit<7>(m_a));
    m_fa[7].set_a(getBit<8>(m_a));
    m_fa[8].set_a(getBit<9>(m_a));
    m_fa[9].set_a(getBit<10>(m_a));
    m_fa[10].set_a(getBit<11>(m_a));
    m_fa[11].set_a(getBit<12>(m_a));
    m_fa[12].set_a(getBit<13>(m_a));
    m_fa[13].set_a(getBit<14>(m_a));
    m_fa[14].set_a(getBit<15>(m_a));

    m_ha.set_b(getBit<0>(m_b));
    m_fa[0].set_b(getBit<1>(m_b));
    m_fa[1].set_b(getBit<2>(m_b));
    m_fa[2].set_b(getBit<3>(m_b));
    m_fa[3].set_b(getBit<4>(m_b));
    m_fa[4].set_b(getBit<5>(m_b));
    m_fa[5].set_b(getBit<6>(m_b));
    m_fa[6].set_b(getBit<7>(m_b));
    m_fa[7].set_b(getBit<8>(m_b));
    m_fa[8].set_b(getBit<9>(m_b));
    m_fa[9].set_b(getBit<10>(m_b));
    m_fa[10].set_b(getBit<11>(m_b));
    m_fa[11].set_b(getBit<12>(m_b));
    m_fa[12].set_b(getBit<13>(m_b));
    m_fa[13].set_b(getBit<14>(m_b));
    m_fa[14].set_b(getBit<15>(m_b));

    m_ha.computeOutput();
    m_fa[0].set_c(m_ha.carry());
    m_fa[0].computeOutput();
    m_fa[1].set_c(m_fa[0].carry());
    m_fa[1].computeOutput();
    m_fa[2].set_c(m_fa[1].carry());
    m_fa[2].computeOutput();
    m_fa[3].set_c(m_fa[2].carry());
    m_fa[3].computeOutput();
    m_fa[4].set_c(m_fa[3].carry());
    m_fa[4].computeOutput();
    m_fa[5].set_c(m_fa[4].carry());
    m_fa[5].computeOutput();
    m_fa[6].set_c(m_fa[5].carry());
    m_fa[6].computeOutput();
    m_fa[7].set_c(m_fa[6].carry());
    m_fa[7].computeOutput();
    m_fa[8].set_c(m_fa[7].carry());
    m_fa[8].computeOutput();
    m_fa[9].set_c(m_fa[8].carry());
    m_fa[9].computeOutput();
    m_fa[10].set_c(m_fa[9].carry());
    m_fa[10].computeOutput();
    m_fa[11].set_c(m_fa[10].carry());
    m_fa[11].computeOutput();
    m_fa[12].set_c(m_fa[11].carry());
    m_fa[12].computeOutput();
    m_fa[13].set_c(m_fa[12].carry());
    m_fa[13].computeOutput();
    m_fa[14].set_c(m_fa[13].carry());
    m_fa[14].computeOutput();

    setBit<0>(m_out, m_ha.sum());
    setBit<1>(m_out, m_fa[0].sum());
    setBit<2>(m_out, m_fa[1].sum());
    setBit<3>(m_out, m_fa[2].sum());
    setBit<4>(m_out, m_fa[3].sum());
    setBit<5>(m_out, m_fa[4].sum());
    setBit<6>(m_out, m_fa[5].sum());
    setBit<7>(m_out, m_fa[6].sum());
    setBit<8>(m_out, m_fa[7].sum());
    setBit<9>(m_out, m_fa[8].sum());
    setBit<10>(m_out, m_fa[9].sum());
    setBit<11>(m_out, m_fa[10].sum());
    setBit<12>(m_out, m_fa[11].sum());
    setBit<13>(m_out, m_fa[12].sum());
    setBit<14>(m_out, m_fa[13].sum());
    setBit<15>(m_out, m_fa[14].sum());
  }

private:
  // pins
  uint16_t m_a = 0;
  uint16_t m_b = 0;
  uint16_t m_out = 0;

  // internal components
  HalfAdder m_ha;
  FullAdder m_fa[15];
};

