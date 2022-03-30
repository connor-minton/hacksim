#pragma once

#include <cstdint>
#include <string>
#include "Bits.h"
#include "Exceptions.h"

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

  void computeOutput() {
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

namespace shallow {

class ALU : ICombinationalCircuit {
public:
  inline uint16_t x() { return m_x; }
  inline uint16_t y() { return m_y; }
  inline uint16_t instruction() { return m_instr; }

  inline void set_x(uint16_t val) { m_x = val; }
  inline void set_y(uint16_t val) { m_y = val; }
  inline void set_instruction(uint16_t val) { m_instr = val; }

  inline uint16_t out() { return m_out; }
  inline bool zr() { return m_zr; }
  inline bool ng() { return m_ng; }

  void computeOutput() {
    uint16_t compCode = (m_instr & 0xfc0) >> 6;

    switch (compCode) {
    case (0x2a): // 101010
      m_out = 0;
      break;
    case (0x3f): // 111111
      m_out = 1;
      break;
    case (0x3a): // 111010
      m_out = -1;
      break;
    case (0x0c): // 001100
      m_out = m_x;
      break;
    case (0x30): // 110000
      m_out = m_y;
      break;
    case (0x0d): // 001101
      m_out = ~m_x;
      break;
    case (0x31): // 110001
      m_out = ~m_y;
      break;
    case (0x0f): // 001111
      m_out = -m_x;
      break;
    case (0x33): // 110011
      m_out = -m_y;
      break;
    case (0x1f): // 011111
      m_out = m_x + 1;
      break;
    case (0x37): // 110111
      m_out = m_y + 1;
      break;
    case (0x0e): // 001110
      m_out = m_x - 1;
      break;
    case (0x32): // 110010
      m_out = m_y - 1;
      break;
    case (0x02): // 000010
      m_out = m_x + m_y;
      break;
    case (0x13): // 010011
      m_out = m_x - m_y;
      break;
    case (0x07): // 000111
      m_out = m_y - m_x;
      break;
    case (0x00): // 000000
      m_out = m_x & m_y;
      break;
    case (0x15): // 010101
      m_out = m_x | m_y;
      break;
    default:
      if (getBit<15>(m_instr))
        throw Error("Unknown C-instruction computation code: " + std::to_string(compCode));
    }

    m_zr = m_out == 0;
    m_ng = getBit<15>(m_out);
  }

private:
  uint16_t m_x = 0;
  uint16_t m_y = 0;
  uint16_t m_instr = 0;

  uint16_t m_out = 0;
  bool m_zr = false;
  bool m_ng = false;
};

}

