#pragma once

#include <cstdint>
#include "Bits.h"

#include "ICombinationalCircuit.h"
#include "Mux16.h"
#include "Not16.h"
#include "And16.h"
#include "Add16.h"
#include "Or8Way.h"
#include "Or.h"

class ALU : public ICombinationalCircuit {
public:
  // INPUT x[16], y[16],
  //       zx, // zero the x input
  //       nx, // negate the x input
  //       zy, // zero the y input
  //       ny, // negate the y input
  //       f,  // calculate x + y if f=1 or x & y if f=0
  //       no  // negate the output
  inline uint16_t x() const { return m_x; }
  inline uint16_t y() const { return m_y; }
  inline bool zx() const { return getBit<0>(m_pins); }
  inline bool nx() const { return getBit<1>(m_pins); }
  inline bool zy() const { return getBit<2>(m_pins); }
  inline bool ny() const { return getBit<3>(m_pins); }
  inline bool f() const { return getBit<4>(m_pins); }
  inline bool no() const { return getBit<5>(m_pins); }

  inline void set_x(uint16_t val) { m_x = val; }
  inline void set_y(uint16_t val) { m_y = val; }
  inline void set_zx(bool val) { setBit<0>(m_pins, val); }
  inline void set_nx(bool val) { setBit<1>(m_pins, val); }
  inline void set_zy(bool val) { setBit<2>(m_pins, val); }
  inline void set_ny(bool val) { setBit<3>(m_pins, val); }
  inline void set_f(bool val) { setBit<4>(m_pins, val); }
  inline void set_no(bool val) { setBit<5>(m_pins, val); }

  // OUTPUT out[16],
  //        zr, // true iff out = 0
  //        ng  // true iff out < 0
  inline uint16_t out() const { return m_out; }
  inline bool zr() const { return getBit<6>(m_pins); }
  inline bool ng() const { return getBit<7>(m_pins); }

  ALU() { computeOutput(); }

  inline void computeOutput() {
    // zero x?
    m_xx.set_a(x());
    m_xx.set_b(0);
    m_xx.set_sel(zx());
    m_xx.computeOutput();

    // zero y?
    m_yy.set_a(y());
    m_yy.set_b(0);
    m_yy.set_sel(zy());
    m_yy.computeOutput();

    // negate x
    m_notxx.set_in(m_xx.out());
    m_notxx.computeOutput();

    // negate y
    m_notyy.set_in(m_yy.out());
    m_notyy.computeOutput();

    // negate x?
    m_xxx.set_a(m_xx.out());
    m_xxx.set_b(m_notxx.out());
    m_xxx.set_sel(nx());
    m_xxx.computeOutput();

    // negate y?
    m_yyy.set_a(m_yy.out());
    m_yyy.set_b(m_notyy.out());
    m_yyy.set_sel(ny());
    m_yyy.computeOutput();

    // x & y
    m_xxxANDyyy.set_a(m_xxx.out());
    m_xxxANDyyy.set_b(m_yyy.out());
    m_xxxANDyyy.computeOutput();

    // x + y
    m_xxxPLUSyyy.set_a(m_xxx.out());
    m_xxxPLUSyyy.set_b(m_yyy.out());
    m_xxxPLUSyyy.computeOutput();

    // (x & y) or (x + y)?
    m_fout.set_a(m_xxxANDyyy.out());
    m_fout.set_b(m_xxxPLUSyyy.out());
    m_fout.set_sel(f());
    m_fout.computeOutput();

    // negate fout
    m_notfout.set_in(m_fout.out());
    m_notfout.computeOutput();

    // negate fout?
    m_outbits.set_a(m_fout.out());
    m_outbits.set_b(m_notfout.out());
    m_outbits.set_sel(no());
    m_outbits.computeOutput();

    // set zr
    m_orLSB.set_in(m_outbits.out() & 0x00ff);
    m_orLSB.computeOutput();
    m_orMSB.set_in((m_outbits.out() & 0xff00) >> 8);
    m_orMSB.computeOutput();
    m_notzero.set_a(m_orLSB.out());
    m_notzero.set_b(m_orMSB.out());
    m_notzero.computeOutput();
    m_zero.set_in(m_notzero.out());
    m_zero.computeOutput();

    // assign outputs
    m_out = m_outbits.out();
    set_zr(m_zero.out());
    set_ng(getBit<15>(m_out));
  }

private:
  // { zx, nx, zy, ny, f, no, zr, ng }
  uint8_t m_pins = 0;
  uint16_t m_x = 0;
  uint16_t m_y = 0;
  uint16_t m_out = 0;

  Mux16  m_xx;
  Mux16  m_yy;

  Not16  m_notxx;
  Not16  m_notyy;

  Mux16  m_xxx;
  Mux16  m_yyy;

  And16  m_xxxANDyyy;
  Add16  m_xxxPLUSyyy;

  Mux16  m_fout;

  Not16  m_notfout;

  Mux16  m_outbits;

  Or8Way m_orLSB;
  Or8Way m_orMSB;
  Or     m_notzero;
  Not    m_zero;

  inline void set_zr(bool val) { setBit<6>(m_pins, val); }
  inline void set_ng(bool val) { setBit<7>(m_pins, val); }
};

