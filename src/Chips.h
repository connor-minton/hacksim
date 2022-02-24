#pragma once

#include "Bits.h"
#include "ShallowChips.h"
#include "FileUtils.h"

class Nand {
public:
  // INPUT
  inline bool a() const { return getBit<0>(pins); }
  inline bool b() const { return getBit<1>(pins); }

  inline void set_a(bool val) { setBit<0>(pins, val); }
  inline void set_b(bool val) { setBit<1>(pins, val); }

  // OUTPUT
  inline bool out() const { return (pins & 0x4) != 0; }

  Nand() { computeOutput(); }

  inline void computeOutput() {
    set_out(!(a() && b()));
  }

private:
  // { a, b, out }
  uint8_t pins = 0;

  inline void set_out(bool val) { setBit<2>(pins, val); }
};

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

class Not {
public:
  // INPUT
  inline bool in() const { return getBit<0>(pins); }

  inline void set_in(bool val) {
    if (val) pins |= 0x1;
    else     pins &= ~0x1;
  }

  // OUTPUT
  inline bool out() const { return getBit<1>(pins); }

  Not() { computeOutput(); }

  inline void computeOutput() {
    m_nand.set_a(in());
    m_nand.set_b(in());
    m_nand.computeOutput();
    set_out(m_nand.out());
  }

private:
  // { in, out }
  uint8_t pins = 0;

  Nand m_nand;

  inline void set_out(bool val) { setBit<1>(pins, val); }
};

class And {
public:
  // INPUT
  inline bool a() const { return getBit<0>(pins); }
  inline bool b() const { return getBit<1>(pins); }

  inline void set_a(bool val) { setBit<0>(pins, val); }
  inline void set_b(bool val) { setBit<1>(pins, val); }

  // OUTPUT
  inline bool out() const { return getBit<2>(pins); }

  And() { computeOutput(); }

  inline void computeOutput() {
    m_nand.set_a(a());
    m_nand.set_b(b());
    m_nand.computeOutput();
    m_not.set_in(m_nand.out());
    m_not.computeOutput();
    set_out(m_not.out());
  }

private:
  // {a, b, out}
  uint8_t pins = 0;

  Nand m_nand;
  Not m_not;

  inline void set_out(bool val) { setBit<2>(pins, val); }
};

class Or {
public:
  // INPUT
  inline bool a() const { return getBit<0>(pins); }
  inline bool b() const { return getBit<1>(pins); }

  inline void set_a(bool val) { setBit<0>(pins, val); }
  inline void set_b(bool val) { setBit<1>(pins, val); }

  // OUTPUT
  inline bool out() const { return getBit<2>(pins); }

  Or() { computeOutput(); }

  inline void computeOutput() {
    m_notA.set_in(a());
    m_notB.set_in(b());
    m_notA.computeOutput();
    m_notB.computeOutput();
    m_nand.set_a(m_notA.out());
    m_nand.set_b(m_notB.out());
    m_nand.computeOutput();
    set_out(m_nand.out());
  }

private:
  // {a, b, out}
  uint8_t pins = 0;

  Not m_notA;
  Not m_notB;
  Nand m_nand;

  inline void set_out(bool val) { setBit<2>(pins, val); }
};

class Xor {
public:
  // INPUT
  inline bool a() const { return getBit<0>(pins); }
  inline bool b() const { return getBit<1>(pins); }

  inline void set_a(bool val) { setBit<0>(pins, val); }
  inline void set_b(bool val) { setBit<1>(pins, val); }

  // OUTPUT
  inline bool out() const { return getBit<2>(pins); }

  Xor() { computeOutput(); }

  inline void computeOutput() {
    m_notA.set_in(a());
    m_notA.computeOutput();
    m_notB.set_in(b());
    m_notB.computeOutput();
    m_nand1.set_a(a());
    m_nand1.set_b(m_notB.out());
    m_nand1.computeOutput();
    m_nand2.set_a(m_notA.out());
    m_nand2.set_b(b());
    m_nand2.computeOutput();
    m_nandOut.set_a(m_nand1.out());
    m_nandOut.set_b(m_nand2.out());
    m_nandOut.computeOutput();
    set_out(m_nandOut.out());
  }

private:
  // {a, b, out}
  uint8_t pins = 0;

  Not m_notA;
  Not m_notB;
  Nand m_nand1;
  Nand m_nand2;
  Nand m_nandOut;

  inline void set_out(bool val) { setBit<2>(pins, val); }
};

class Mux {
public:
  // INPUT
  inline bool a() const { return getBit<0>(pins); }
  inline bool b() const { return getBit<1>(pins); }
  inline bool sel() const { return getBit<2>(pins); }

  inline void set_a(bool val) { setBit<0>(pins, val); }
  inline void set_b(bool val) { setBit<1>(pins, val); }
  inline void set_sel(bool val) { setBit<2>(pins, val); }

  // OUTPUT
  inline bool out() const { return getBit<3>(pins); }

  Mux() { computeOutput(); }

  inline void computeOutput() {
    m_notSel.set_in(sel());
    m_notSel.computeOutput();
    m_andA.set_a(a());
    m_andA.set_b(m_notSel.out());
    m_andA.computeOutput();
    m_andB.set_a(b());
    m_andB.set_b(sel());
    m_andB.computeOutput();
    m_orOut.set_a(m_andA.out());
    m_orOut.set_b(m_andB.out());
    m_orOut.computeOutput();
    set_out(m_orOut.out());
  }

private:
  // {a, b, sel, out}
  uint8_t pins = 0;

  Not m_notSel;
  And m_andA;
  And m_andB;
  Or m_orOut;

  inline void set_out(bool val) { setBit<3>(pins, val); }
};

class DMux {
public:
  // INPUT
  inline bool in() const { return getBit<0>(pins); }
  inline bool sel() const { return getBit<1>(pins); }

  inline void set_in(bool val) { setBit<0>(pins, val); }
  inline void set_sel(bool val) { setBit<1>(pins, val); }

  // OUTPUT
  inline bool a() const { return getBit<2>(pins); }
  inline bool b() const { return getBit<3>(pins); }

  DMux() { computeOutput(); }

  inline void computeOutput() {
    m_notSel.set_in(sel());
    m_notSel.computeOutput();
    m_andA.set_a(in());
    m_andA.set_b(m_notSel.out());
    m_andA.computeOutput();
    m_andB.set_a(in());
    m_andB.set_b(sel());
    m_andB.computeOutput();
    set_a(m_andA.out());
    set_b(m_andB.out());
  }

private:
  // {in, sel, a, b}
  uint8_t pins = 0;

  Not m_notSel;
  And m_andA;
  And m_andB;

  inline void set_a(bool val) { setBit<2>(pins, val); }
  inline void set_b(bool val) { setBit<3>(pins, val); }
};

class Not16 {
public:
  // INPUT
  inline uint16_t in() const { return m_in; }

  inline void set_in(uint16_t val) { m_in = val; }

  // OUTPUT
  inline uint16_t out() const { return m_out; }

  Not16() { computeOutput(); }

  inline void computeOutput() {
    m_nand.set_a(in());
    m_nand.set_b(in());
    m_nand.computeOutput();
    m_out = m_nand.out();
  }

private:
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  Nand16 m_nand;
};

class And16 {
public:
  // INPUT
  inline uint16_t a() const { return m_a; }
  inline uint16_t b() const { return m_b; }

  inline void set_a(uint16_t val) { m_a = val; }
  inline void set_b(uint16_t val) { m_b = val; }

  // OUTPUT
  inline uint16_t out() const { return m_out; }

  And16() { computeOutput(); }

  inline void computeOutput() {
    m_nand.set_a(a());
    m_nand.set_b(b());
    m_nand.computeOutput();
    m_not.set_in(m_nand.out());
    m_not.computeOutput();
    m_out = m_not.out();
  }

private:
  uint16_t m_a = 0;
  uint16_t m_b = 0;
  uint16_t m_out = 0;

  Nand16 m_nand;
  Not16 m_not;
};

class Or16 {
public:
  // INPUT
  inline uint16_t a() const { return m_a; }
  inline uint16_t b() const { return m_b; }

  inline void set_a(uint16_t val) { m_a = val; }
  inline void set_b(uint16_t val) { m_b = val; }

  // OUTPUT
  inline uint16_t out() const { return m_out; }

  Or16() { computeOutput(); }

  inline void computeOutput() {
    m_notA.set_in(a());
    m_notA.computeOutput();
    m_notB.set_in(b());
    m_notB.computeOutput();

    m_nand.set_a(m_notA.out());
    m_nand.set_b(m_notB.out());
    m_nand.computeOutput();

    m_out = m_nand.out();
  }

private:
  uint16_t m_a = 0;
  uint16_t m_b = 0;
  uint16_t m_out = 0;

  Not16 m_notA;
  Not16 m_notB;
  Nand16 m_nand;
};

class Mux16 {
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

class Mux8Way16 {
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

  inline void computeOutput() {
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

class DMux4Way {
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

class DMux8Way {
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

  inline void computeOutput() {
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

class HalfAdder {
public:
  // INPUT a, b
  inline bool a() const { return getBit<0>(m_pins); }
  inline bool b() const { return getBit<1>(m_pins); }

  inline void set_a(bool val) { setBit<0>(m_pins, val); }
  inline void set_b(bool val) { setBit<1>(m_pins, val); }

  // OUTPUT sum, carry
  inline bool sum() const { return getBit<2>(m_pins); }
  inline bool carry() const { return getBit<3>(m_pins); }

  HalfAdder() { computeOutput(); }

  inline void computeOutput() {
    m_xor.set_a(a());
    m_xor.set_b(b());
    m_xor.computeOutput();
    set_sum(m_xor.out());

    m_and.set_a(a());
    m_and.set_b(b());
    m_and.computeOutput();
    set_carry(m_and.out());
  }

private:
  // { a, b, sum, carry }
  uint8_t m_pins = 0;

  Xor m_xor;
  And m_and;

  inline void set_sum(bool val) { setBit<2>(m_pins, val); }
  inline void set_carry(bool val) { setBit<3>(m_pins, val); }
};

class FullAdder {
public:
  // INPUT a, b, c
  inline bool a() const { return getBit<0>(m_pins); }
  inline bool b() const { return getBit<1>(m_pins); }
  inline bool c() const { return getBit<2>(m_pins); }

  inline void set_a(bool val) { setBit<0>(m_pins, val); }
  inline void set_b(bool val) { setBit<1>(m_pins, val); }
  inline void set_c(bool val) { setBit<2>(m_pins, val); }

  // OUTPUT sum, carry
  inline bool sum() const { return getBit<3>(m_pins); }
  inline bool carry() const { return getBit<4>(m_pins); }

  FullAdder() { computeOutput(); }

  inline void computeOutput() {
    m_axb.set_a(a());
    m_axb.set_b(b());
    m_axb.computeOutput();

    m_xSum.set_a(m_axb.out());
    m_xSum.set_b(c());
    m_xSum.computeOutput();

    m_axbnc.set_a(m_axb.out());
    m_axbnc.set_b(c());
    m_axbnc.computeOutput();

    m_anb.set_a(a());
    m_anb.set_b(b());
    m_anb.computeOutput();

    m_orCarry.set_a(m_axbnc.out());
    m_orCarry.set_b(m_anb.out());
    m_orCarry.computeOutput();

    set_sum(m_xSum.out());
    set_carry(m_orCarry.out());
  }

private:
  // { a, b, c, sum, carry }
  uint8_t m_pins = 0;

  Xor m_axb;
  Xor m_xSum;

  And m_axbnc;
  And m_anb;

  Or m_orCarry;

  inline void set_sum(bool val) { setBit<3>(m_pins, val); }
  inline void set_carry(bool val) { setBit<4>(m_pins, val); }
};

class Add16 {
public:
  // INPUT a[16], b[16]
  inline uint16_t a() const { return m_a; }
  inline uint16_t b() const { return m_b; }

  inline void set_a(uint16_t val) { m_a = val; }
  inline void set_b(uint16_t val) { m_b = val; }

  // OUTPUT out[16]
  inline uint16_t out() const { return m_out; }

  Add16() { computeOutput(); }

  inline void computeOutput() {
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
  uint16_t m_a = 0;
  uint16_t m_b = 0;
  uint16_t m_out = 0;

  HalfAdder m_ha;
  FullAdder m_fa[15];
};

class Inc16 {
public:
  // INPUT in[16]
  inline uint16_t in() const { return m_in; }

  inline void set_in(uint16_t val) { m_in = val; }

  // OUTPUT out[16]
  inline uint16_t out() const { return m_out; }

  Inc16() { computeOutput(); }

  inline void computeOutput() {
    m_add.set_a(m_in);
    m_add.set_b(1);
    m_add.computeOutput();
    m_out = m_add.out();
  }

private:
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  Add16 m_add;
};

class ALU {
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

class DFF {
public:
  // INPUT in
  inline bool in() const { return getBit<0>(m_pins); }

  inline void set_in(bool val) { setBit<0>(m_pins, val); }

  // OUTPUT out
  inline bool out() const { return getBit<1>(m_pins); }

  inline void poke(bool val) { set_out(val); }

  DFF() { tock(); }

  inline void tock() {
    set_out(in());
  }

private:
  // { in, out }
  uint8_t m_pins = 0;

  inline void set_out(bool val) { setBit<1>(m_pins, val); }
};

class Bit {
public:
  // INPUT in, load
  inline bool in() const { return getBit<0>(m_pins); }
  inline bool load() const { return getBit<1>(m_pins); }

  inline void set_in(bool val) { setBit<0>(m_pins, val); }
  inline void set_load(bool val) { setBit<1>(m_pins, val); }

  // OUTPUT out
  inline bool out() const { return getBit<2>(m_pins); }

  // Bypass the clocking system to forcefully set the underlying value
  // to `val`
  inline void poke(bool val) { m_dff.poke(val); set_out(val); }

  Bit() { tock(); }

  inline void tick() {
    m_mux.set_a(m_dff.out());
    m_mux.set_b(in());
    m_mux.set_sel(load());
    m_mux.computeOutput();

    m_dff.set_in(m_mux.out());
  }

  inline void tock() {
    m_dff.tock();
    set_out(m_dff.out());
  }

private:
  // { in, load, out }
  uint8_t m_pins = 0;

  Mux m_mux;
  DFF m_dff;

  inline void set_out(bool val) { setBit<2>(m_pins, val); }
};

class Register {
public:
  // INPUT in[16], load
  inline uint16_t in() const { return m_in; }
  inline bool load() const { return getBit<0>(m_pins); }

  inline void set_in(uint16_t val) { m_in = val; }
  inline void set_load(bool val) { setBit<0>(m_pins, val); }

  // OUTPUT out[16]
  inline uint16_t out() const { return m_out; }

  // Bypass the clocking system to forcefully set the underlying value
  // to `val`
  inline void poke(uint16_t val) {
    m_bits[0].poke(getBit<0>(val));
    m_bits[1].poke(getBit<1>(val));
    m_bits[2].poke(getBit<2>(val));
    m_bits[3].poke(getBit<3>(val));
    m_bits[4].poke(getBit<4>(val));
    m_bits[5].poke(getBit<5>(val));
    m_bits[6].poke(getBit<6>(val));
    m_bits[7].poke(getBit<7>(val));
    m_bits[8].poke(getBit<8>(val));
    m_bits[9].poke(getBit<9>(val));
    m_bits[10].poke(getBit<10>(val));
    m_bits[11].poke(getBit<11>(val));
    m_bits[12].poke(getBit<12>(val));
    m_bits[13].poke(getBit<13>(val));
    m_bits[14].poke(getBit<14>(val));
    m_bits[15].poke(getBit<15>(val));

    setBit<0>(m_out, m_bits[0].out());
    setBit<1>(m_out, m_bits[1].out());
    setBit<2>(m_out, m_bits[2].out());
    setBit<3>(m_out, m_bits[3].out());
    setBit<4>(m_out, m_bits[4].out());
    setBit<5>(m_out, m_bits[5].out());
    setBit<6>(m_out, m_bits[6].out());
    setBit<7>(m_out, m_bits[7].out());
    setBit<8>(m_out, m_bits[8].out());
    setBit<9>(m_out, m_bits[9].out());
    setBit<10>(m_out, m_bits[10].out());
    setBit<11>(m_out, m_bits[11].out());
    setBit<12>(m_out, m_bits[12].out());
    setBit<13>(m_out, m_bits[13].out());
    setBit<14>(m_out, m_bits[14].out());
    setBit<15>(m_out, m_bits[15].out());
  }

  Register() { tock(); }

  inline void tick() {
    m_bits[0].set_in(getBit<0>(m_in));
    m_bits[1].set_in(getBit<1>(m_in));
    m_bits[2].set_in(getBit<2>(m_in));
    m_bits[3].set_in(getBit<3>(m_in));
    m_bits[4].set_in(getBit<4>(m_in));
    m_bits[5].set_in(getBit<5>(m_in));
    m_bits[6].set_in(getBit<6>(m_in));
    m_bits[7].set_in(getBit<7>(m_in));
    m_bits[8].set_in(getBit<8>(m_in));
    m_bits[9].set_in(getBit<9>(m_in));
    m_bits[10].set_in(getBit<10>(m_in));
    m_bits[11].set_in(getBit<11>(m_in));
    m_bits[12].set_in(getBit<12>(m_in));
    m_bits[13].set_in(getBit<13>(m_in));
    m_bits[14].set_in(getBit<14>(m_in));
    m_bits[15].set_in(getBit<15>(m_in));

    m_bits[0].set_load(load());
    m_bits[1].set_load(load());
    m_bits[2].set_load(load());
    m_bits[3].set_load(load());
    m_bits[4].set_load(load());
    m_bits[5].set_load(load());
    m_bits[6].set_load(load());
    m_bits[7].set_load(load());
    m_bits[8].set_load(load());
    m_bits[9].set_load(load());
    m_bits[10].set_load(load());
    m_bits[11].set_load(load());
    m_bits[12].set_load(load());
    m_bits[13].set_load(load());
    m_bits[14].set_load(load());
    m_bits[15].set_load(load());

    m_bits[0].tick();
    m_bits[1].tick();
    m_bits[2].tick();
    m_bits[3].tick();
    m_bits[4].tick();
    m_bits[5].tick();
    m_bits[6].tick();
    m_bits[7].tick();
    m_bits[8].tick();
    m_bits[9].tick();
    m_bits[10].tick();
    m_bits[11].tick();
    m_bits[12].tick();
    m_bits[13].tick();
    m_bits[14].tick();
    m_bits[15].tick();
  }

  inline void tock() {
    m_bits[0].tock();
    m_bits[1].tock();
    m_bits[2].tock();
    m_bits[3].tock();
    m_bits[4].tock();
    m_bits[5].tock();
    m_bits[6].tock();
    m_bits[7].tock();
    m_bits[8].tock();
    m_bits[9].tock();
    m_bits[10].tock();
    m_bits[11].tock();
    m_bits[12].tock();
    m_bits[13].tock();
    m_bits[14].tock();
    m_bits[15].tock();

    setBit<0>(m_out, m_bits[0].out());
    setBit<1>(m_out, m_bits[1].out());
    setBit<2>(m_out, m_bits[2].out());
    setBit<3>(m_out, m_bits[3].out());
    setBit<4>(m_out, m_bits[4].out());
    setBit<5>(m_out, m_bits[5].out());
    setBit<6>(m_out, m_bits[6].out());
    setBit<7>(m_out, m_bits[7].out());
    setBit<8>(m_out, m_bits[8].out());
    setBit<9>(m_out, m_bits[9].out());
    setBit<10>(m_out, m_bits[10].out());
    setBit<11>(m_out, m_bits[11].out());
    setBit<12>(m_out, m_bits[12].out());
    setBit<13>(m_out, m_bits[13].out());
    setBit<14>(m_out, m_bits[14].out());
    setBit<15>(m_out, m_bits[15].out());
  }

private:
  // { load }
  uint8_t m_pins = 0;
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  Bit m_bits[16];
};

class PC {
public:
  // INPUT in[16], load, inc, reset
  inline uint16_t in() const { return m_in; }
  inline bool load() const { return getBit<0>(m_pins); }
  inline bool inc() const { return getBit<1>(m_pins); }
  inline bool reset() const { return getBit<2>(m_pins); }

  inline void set_in(uint16_t val) { m_in = val; }
  inline void set_load(bool val) { setBit<0>(m_pins, val); }
  inline void set_inc(bool val) { setBit<1>(m_pins, val); }
  inline void set_reset(bool val) { setBit<2>(m_pins, val); }

  // OUTPUT out[16]
  inline uint16_t out() const { return m_out; }

  PC() { m_reg.set_load(true); }

  inline void tick() {
    m_inc.set_in(m_reg.out());
    m_inc.computeOutput();

    m_incMux.set_a(m_reg.out());
    m_incMux.set_b(m_inc.out());
    m_incMux.set_sel(inc());
    m_incMux.computeOutput();

    m_loadMux.set_a(m_incMux.out());
    m_loadMux.set_b(in());
    m_loadMux.set_sel(load());
    m_loadMux.computeOutput();

    m_resetMux.set_a(m_loadMux.out());
    m_resetMux.set_b(0);
    m_resetMux.set_sel(reset());
    m_resetMux.computeOutput();

    m_reg.set_in(m_resetMux.out());
    m_reg.tick();
  }

  inline void tock() {
    m_reg.tock();
    m_out = m_reg.out();
  }

private:
  // { load, inc, reset }
  uint8_t m_pins = 0;
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  Inc16 m_inc;
  Mux16 m_incMux;
  Mux16 m_loadMux;
  Mux16 m_resetMux;
  Register m_reg;
};

class RAM8 {
public:
  // INPUT in[16], load, address[3]
  inline uint16_t in() const { return m_in; }
  inline bool load() const { return getBit<0>(m_pins); }
  inline uint8_t address() const { return (m_pins & 0xe) >> 1; }

  inline void set_in(uint16_t val) { m_in = val; }
  inline void set_load(bool val) { setBit<0>(m_pins, val); }
  inline void set_address(uint8_t val) {
    m_pins = (m_pins & 0xf0) | ((val & 0x7) << 1) | (m_pins & 0x1);
  }

  // OUTPUT out[16]
  inline uint16_t out() const { return m_out; }

  // Bypass the clocking system to examine the value at `offset`
  inline uint16_t peek(uint16_t offset) {
    return m_regs[offset & 0x7].out();
  }

  // Bypass the clocking system to forcefully set the underlying value
  // at `offset` to `val`
  inline void poke(uint16_t offset, uint16_t val) {
    m_regs[offset & 0x7].poke(val);
  }

  inline void tick() {
    m_dmux.set_in(load());
    m_dmux.set_sel(address());
    m_dmux.computeOutput();

    m_regs[0].set_in(in());
    m_regs[1].set_in(in());
    m_regs[2].set_in(in());
    m_regs[3].set_in(in());
    m_regs[4].set_in(in());
    m_regs[5].set_in(in());
    m_regs[6].set_in(in());
    m_regs[7].set_in(in());

    m_regs[0].set_load(m_dmux.a());
    m_regs[1].set_load(m_dmux.b());
    m_regs[2].set_load(m_dmux.c());
    m_regs[3].set_load(m_dmux.d());
    m_regs[4].set_load(m_dmux.e());
    m_regs[5].set_load(m_dmux.f());
    m_regs[6].set_load(m_dmux.g());
    m_regs[7].set_load(m_dmux.h());

    // optimization: only tick-tock the selected register
    m_regs[m_dmux.sel()].tick();

    m_mux.set_sel(address());
    m_mux.set_a(m_regs[0].out());
    m_mux.set_b(m_regs[1].out());
    m_mux.set_c(m_regs[2].out());
    m_mux.set_d(m_regs[3].out());
    m_mux.set_e(m_regs[4].out());
    m_mux.set_f(m_regs[5].out());
    m_mux.set_g(m_regs[6].out());
    m_mux.set_h(m_regs[7].out());
    m_mux.computeOutput();

    m_out = m_mux.out();
  }

  inline void tock() {
    m_regs[m_dmux.sel()].tock();

    // duplicate logic from tick, but necessary after tocking the reg
    m_mux.set_sel(address());
    m_mux.set_a(m_regs[0].out());
    m_mux.set_b(m_regs[1].out());
    m_mux.set_c(m_regs[2].out());
    m_mux.set_d(m_regs[3].out());
    m_mux.set_e(m_regs[4].out());
    m_mux.set_f(m_regs[5].out());
    m_mux.set_g(m_regs[6].out());
    m_mux.set_h(m_regs[7].out());
    m_mux.computeOutput();

    m_out = m_mux.out();
  }

private:
  // { load, address[0..2] }
  uint8_t m_pins = 0;
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  DMux8Way m_dmux;
  Mux8Way16 m_mux;
  Register m_regs[8];
};

class RAM64 {
public:
  // INPUT in[16], load, address[6]
  inline uint16_t in() const { return m_in; }
  inline bool load() const { return getBit<0>(m_pins); }
  inline uint8_t address() const { return (m_pins & 0x7e) >> 1; }

  inline void set_in(uint16_t val) { m_in = val; }
  inline void set_load(bool val) { setBit<0>(m_pins, val); }
  inline void set_address(uint8_t val) {
    m_pins = (m_pins & 0x80) | ((val & 0x3f) << 1) | (m_pins & 0x1);
  }

  // OUTPUT out[16]
  inline uint16_t out() const { return m_out; }

  // Bypass the clocking system to examine the value at `offset`
  inline uint16_t peek(uint16_t offset) {
    return m_rams[(offset & 0x38) >> 3].peek(offset & 0x7);
  }

  // Bypass the clocking system to forcefully set the underlying value
  // at `offset` to `val`
  inline void poke(uint16_t offset, uint16_t val) {
    m_rams[(offset & 0x38) >> 3].poke(offset & 0x7, val);
  }

  inline void tick() {
    m_dmux.set_in(load());
    m_dmux.set_sel((address() & 0x38) >> 3);
    m_dmux.computeOutput();

    m_rams[0].set_in(in());
    m_rams[1].set_in(in());
    m_rams[2].set_in(in());
    m_rams[3].set_in(in());
    m_rams[4].set_in(in());
    m_rams[5].set_in(in());
    m_rams[6].set_in(in());
    m_rams[7].set_in(in());

    m_rams[0].set_load(m_dmux.a());
    m_rams[1].set_load(m_dmux.b());
    m_rams[2].set_load(m_dmux.c());
    m_rams[3].set_load(m_dmux.d());
    m_rams[4].set_load(m_dmux.e());
    m_rams[5].set_load(m_dmux.f());
    m_rams[6].set_load(m_dmux.g());
    m_rams[7].set_load(m_dmux.h());

    m_rams[0].set_address(address() & 0x7);
    m_rams[1].set_address(address() & 0x7);
    m_rams[2].set_address(address() & 0x7);
    m_rams[3].set_address(address() & 0x7);
    m_rams[4].set_address(address() & 0x7);
    m_rams[5].set_address(address() & 0x7);
    m_rams[6].set_address(address() & 0x7);
    m_rams[7].set_address(address() & 0x7);

    // optimization: only tick() the selected ram module
    m_rams[m_dmux.sel()].tick();

    m_mux.set_sel((address() & 0x38) >> 3);
    m_mux.set_a(m_rams[0].out());
    m_mux.set_b(m_rams[1].out());
    m_mux.set_c(m_rams[2].out());
    m_mux.set_d(m_rams[3].out());
    m_mux.set_e(m_rams[4].out());
    m_mux.set_f(m_rams[5].out());
    m_mux.set_g(m_rams[6].out());
    m_mux.set_h(m_rams[7].out());
    m_mux.computeOutput();

    m_out = m_mux.out();
  }

  inline void tock() {
    // optimization: only tock() the selected ram module
    m_rams[m_dmux.sel()].tock();

    m_mux.set_sel((address() & 0x38) >> 3);
    m_mux.set_a(m_rams[0].out());
    m_mux.set_b(m_rams[1].out());
    m_mux.set_c(m_rams[2].out());
    m_mux.set_d(m_rams[3].out());
    m_mux.set_e(m_rams[4].out());
    m_mux.set_f(m_rams[5].out());
    m_mux.set_g(m_rams[6].out());
    m_mux.set_h(m_rams[7].out());
    m_mux.computeOutput();

    m_out = m_mux.out();
  }

private:
  // { load, address[0..5] }
  uint8_t m_pins = 0;
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  DMux8Way m_dmux;
  Mux8Way16 m_mux;
  RAM8 m_rams[8];
};

class RAM512 {
public:
  // INPUT in[16], load, address[9]
  inline uint16_t in() const { return m_in; }
  inline bool load() const { return getBit<0>(m_pins); }
  inline uint16_t address() const { return (m_pins & 0x3fe) >> 1; }

  inline void set_in(uint16_t val) { m_in = val; }
  inline void set_load(bool val) { setBit<0>(m_pins, val); }
  inline void set_address(uint16_t val) {
    m_pins = (m_pins & 0xfc00) | ((val & 0x1ff) << 1) | (m_pins & 0x1);
  }

  // OUTPUT out[16]
  inline uint16_t out() const { return m_out; }

  // Bypass the clocking system to examine the value at `offset`
  inline uint16_t peek(uint16_t offset) {
    return m_rams[(offset & 0x1c0) >> 6].peek(offset & 0x3f);
  }

  // Bypass the clocking system to forcefully set the underlying value
  // at `offset` to `val`
  inline void poke(uint16_t offset, uint16_t val) {
    m_rams[(offset & 0x1c0) >> 6].poke(offset & 0x3f, val);
  }

  inline void tick() {
    m_dmux.set_in(load());
    m_dmux.set_sel((address() & 0x1c0) >> 6);
    m_dmux.computeOutput();

    m_rams[0].set_in(in());
    m_rams[1].set_in(in());
    m_rams[2].set_in(in());
    m_rams[3].set_in(in());
    m_rams[4].set_in(in());
    m_rams[5].set_in(in());
    m_rams[6].set_in(in());
    m_rams[7].set_in(in());

    m_rams[0].set_load(m_dmux.a());
    m_rams[1].set_load(m_dmux.b());
    m_rams[2].set_load(m_dmux.c());
    m_rams[3].set_load(m_dmux.d());
    m_rams[4].set_load(m_dmux.e());
    m_rams[5].set_load(m_dmux.f());
    m_rams[6].set_load(m_dmux.g());
    m_rams[7].set_load(m_dmux.h());

    m_rams[0].set_address(address() & 0x3f);
    m_rams[1].set_address(address() & 0x3f);
    m_rams[2].set_address(address() & 0x3f);
    m_rams[3].set_address(address() & 0x3f);
    m_rams[4].set_address(address() & 0x3f);
    m_rams[5].set_address(address() & 0x3f);
    m_rams[6].set_address(address() & 0x3f);
    m_rams[7].set_address(address() & 0x3f);

    // optimization: only tick() the selected ram module
    m_rams[m_dmux.sel()].tick();

    m_mux.set_sel((address() & 0x1c0) >> 6);
    m_mux.set_a(m_rams[0].out());
    m_mux.set_b(m_rams[1].out());
    m_mux.set_c(m_rams[2].out());
    m_mux.set_d(m_rams[3].out());
    m_mux.set_e(m_rams[4].out());
    m_mux.set_f(m_rams[5].out());
    m_mux.set_g(m_rams[6].out());
    m_mux.set_h(m_rams[7].out());
    m_mux.computeOutput();

    m_out = m_mux.out();
  }

  inline void tock() {
    // optimization: only tock() the selected ram module
    m_rams[m_dmux.sel()].tock();

    m_mux.set_sel((address() & 0x1c0) >> 6);
    m_mux.set_a(m_rams[0].out());
    m_mux.set_b(m_rams[1].out());
    m_mux.set_c(m_rams[2].out());
    m_mux.set_d(m_rams[3].out());
    m_mux.set_e(m_rams[4].out());
    m_mux.set_f(m_rams[5].out());
    m_mux.set_g(m_rams[6].out());
    m_mux.set_h(m_rams[7].out());
    m_mux.computeOutput();

    m_out = m_mux.out();
  }

private:
  // { load, address[0..8] }
  uint16_t m_pins = 0;
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  DMux8Way m_dmux;
  Mux8Way16 m_mux;
  RAM64 m_rams[8];
};

class RAM4K {
public:
  // INPUT in[16], load, address[12]
  inline uint16_t in() const { return m_in; }
  inline bool load() const { return getBit<0>(m_pins); }
  inline uint16_t address() const { return (m_pins & 0x1ffe) >> 1; }

  inline void set_in(uint16_t val) { m_in = val; }
  inline void set_load(bool val) { setBit<0>(m_pins, val); }
  inline void set_address(uint16_t val) {
    m_pins = (m_pins & 0xe000) | ((val & 0xfff) << 1) | (m_pins & 0x1);
  }

  // OUTPUT out[16]
  inline uint16_t out() const { return m_out; }

  // Bypass the clocking system to examine the value at `offset`
  inline uint16_t peek(uint16_t offset) {
    return m_rams[(offset & 0xe00) >> 9].peek(offset & 0x1ff);
  }

  // Bypass the clocking system to forcefully set the underlying value
  // at `offset` to `val`
  inline void poke(uint16_t offset, uint16_t val) {
    m_rams[(offset & 0xe00) >> 9].poke(offset & 0x1ff, val);
  }

  inline void tick() {
    m_dmux.set_in(load());
    m_dmux.set_sel((address() & 0xe00) >> 9);
    m_dmux.computeOutput();

    m_rams[0].set_in(in());
    m_rams[1].set_in(in());
    m_rams[2].set_in(in());
    m_rams[3].set_in(in());
    m_rams[4].set_in(in());
    m_rams[5].set_in(in());
    m_rams[6].set_in(in());
    m_rams[7].set_in(in());

    m_rams[0].set_load(m_dmux.a());
    m_rams[1].set_load(m_dmux.b());
    m_rams[2].set_load(m_dmux.c());
    m_rams[3].set_load(m_dmux.d());
    m_rams[4].set_load(m_dmux.e());
    m_rams[5].set_load(m_dmux.f());
    m_rams[6].set_load(m_dmux.g());
    m_rams[7].set_load(m_dmux.h());

    m_rams[0].set_address(address() & 0x1ff);
    m_rams[1].set_address(address() & 0x1ff);
    m_rams[2].set_address(address() & 0x1ff);
    m_rams[3].set_address(address() & 0x1ff);
    m_rams[4].set_address(address() & 0x1ff);
    m_rams[5].set_address(address() & 0x1ff);
    m_rams[6].set_address(address() & 0x1ff);
    m_rams[7].set_address(address() & 0x1ff);

    // optimization: only tick() the selected ram module
    m_rams[m_dmux.sel()].tick();

    m_mux.set_sel((address() & 0xe00) >> 9);
    m_mux.set_a(m_rams[0].out());
    m_mux.set_b(m_rams[1].out());
    m_mux.set_c(m_rams[2].out());
    m_mux.set_d(m_rams[3].out());
    m_mux.set_e(m_rams[4].out());
    m_mux.set_f(m_rams[5].out());
    m_mux.set_g(m_rams[6].out());
    m_mux.set_h(m_rams[7].out());
    m_mux.computeOutput();

    m_out = m_mux.out();
  }

  inline void tock() {
    // optimization: only tock() the selected ram module
    m_rams[m_dmux.sel()].tock();

    m_mux.set_sel((address() & 0xe00) >> 9);
    m_mux.set_a(m_rams[0].out());
    m_mux.set_b(m_rams[1].out());
    m_mux.set_c(m_rams[2].out());
    m_mux.set_d(m_rams[3].out());
    m_mux.set_e(m_rams[4].out());
    m_mux.set_f(m_rams[5].out());
    m_mux.set_g(m_rams[6].out());
    m_mux.set_h(m_rams[7].out());
    m_mux.computeOutput();

    m_out = m_mux.out();
  }

private:
  // { load, address[0..11] }
  uint16_t m_pins = 0;
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  DMux8Way m_dmux;
  Mux8Way16 m_mux;
  RAM512 m_rams[8];
};

class RAM16K {
public:
  // INPUT in[16], load, address[14]
  inline uint16_t in() const { return m_in; }
  inline bool load() const { return getBit<0>(m_pins); }
  inline uint16_t address() const { return (m_pins & 0x7ffe) >> 1; }

  inline void set_in(uint16_t val) { m_in = val; }
  inline void set_load(bool val) { setBit<0>(m_pins, val); }
  inline void set_address(uint16_t val) {
    m_pins = (m_pins & 0x8000) | ((val & 0x3fff) << 1) | (m_pins & 0x1);
  }

  // OUTPUT out[16]
  inline uint16_t out() const { return m_out; }

  // Bypass the clocking system to examine the value at `offset`
  inline uint16_t peek(uint16_t offset) {
    return m_rams[(offset & 0x3000) >> 12].peek(offset & 0xfff);
  }

  // Bypass the clocking system to forcefully set the underlying value
  // at `offset` to `val`
  inline void poke(uint16_t offset, uint16_t val) {
    m_rams[(offset & 0x3000) >> 12].poke(offset & 0xfff, val);
  }

  inline void tick() {
    m_dmux.set_in(load());
    m_dmux.set_sel((address() & 0x3000) >> 12);
    m_dmux.computeOutput();

    m_rams[0].set_in(in());
    m_rams[1].set_in(in());
    m_rams[2].set_in(in());
    m_rams[3].set_in(in());

    m_rams[0].set_load(m_dmux.a());
    m_rams[1].set_load(m_dmux.b());
    m_rams[2].set_load(m_dmux.c());
    m_rams[3].set_load(m_dmux.d());

    m_rams[0].set_address(address() & 0xfff);
    m_rams[1].set_address(address() & 0xfff);
    m_rams[2].set_address(address() & 0xfff);
    m_rams[3].set_address(address() & 0xfff);

    // optimization: only tick() the selected ram module
    m_rams[m_dmux.sel()].tick();

    m_mux.set_sel((address() & 0x3000) >> 12);
    m_mux.set_a(m_rams[0].out());
    m_mux.set_b(m_rams[1].out());
    m_mux.set_c(m_rams[2].out());
    m_mux.set_d(m_rams[3].out());
    m_mux.computeOutput();

    m_out = m_mux.out();
  }

  inline void tock() {
    // optimization: only tock() the selected ram module
    m_rams[m_dmux.sel()].tock();

    m_mux.set_sel((address() & 0x3000) >> 12);
    m_mux.set_a(m_rams[0].out());
    m_mux.set_b(m_rams[1].out());
    m_mux.set_c(m_rams[2].out());
    m_mux.set_d(m_rams[3].out());
    m_mux.computeOutput();

    m_out = m_mux.out();
  }

private:
  // { load, address[0..13] }
  uint16_t m_pins = 0;
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  DMux4Way m_dmux;
  Mux4Way16 m_mux;
  RAM4K m_rams[4];
};

class Memory {
public:
  static const uint16_t SCREEN = 0x4000;
  static const uint16_t KBD = 0x6000;

  // IN in[16], load, address[15]
  inline uint16_t in() const { return m_in; }
  inline bool load() const { return getBit<0>(m_pins); }
  inline uint16_t address() const { return (m_pins & 0xfffe) >> 1; }

  inline void set_in(uint16_t val) { m_in = val; }
  inline void set_load(bool val) { setBit<0>(m_pins, val); }
  inline void set_address(uint16_t val) {
    m_pins = ((val & 0x7fff) << 1) | (m_pins & 0x1);
  }

  // OUT out[16]
  inline uint16_t out() const { return m_out; }

  Memory(uint16_t* screen, uint16_t* kbd)
    : m_screen(screen), m_kbd(kbd) { }

  // Bypass the clocking system to examine memory at `offset`
  inline uint16_t peek(uint16_t offset) {
    if (offset < SCREEN) return m_ram.peek(offset);
    if (offset < KBD)    return m_screen.peek(offset - KBD);
    return *m_kbd;
  }

  // Bypass the clocking system to set memory at `offset` to `val`
  inline void poke(uint16_t offset, uint16_t val) {
    if (offset < SCREEN)   m_ram.poke(offset, val);
    else if (offset < KBD) m_screen.poke(offset - KBD, val);
    else                   *m_kbd = val;
  }

  inline void tick() {
    m_demuxRAM.set_in(load());
    m_demuxRAM.set_sel(getBit<14>(address()));
    m_demuxRAM.computeOutput();

    m_demuxScreenKbd.set_in(m_demuxRAM.b());
    m_demuxScreenKbd.set_sel(getBit<13>(address()));
    m_demuxScreenKbd.computeOutput();

    m_ram.set_in(in());
    m_ram.set_load(m_demuxRAM.a());
    m_ram.set_address(address() & 0x3fff);

    // optimization: tick ram ONLY if it is being accessed
    if (!getBit<14>(address()))
      m_ram.tick();

    // screen is a shallow chip, doesn't need optimization
    m_screen.set_in(in());
    m_screen.set_load(m_demuxScreenKbd.a());
    m_screen.set_address(address() & 0x1fff);
    m_screen.tick();

    m_muxScreenKbdOut.set_sel(getBit<13>(address()));
    m_muxScreenKbdOut.set_a(m_screen.out());
    m_muxScreenKbdOut.set_b(*m_kbd);
    m_muxScreenKbdOut.computeOutput();

    m_muxOut.set_sel(getBit<14>(address()));
    m_muxOut.set_a(m_ram.out());
    m_muxOut.set_b(m_muxScreenKbdOut.out());
    m_muxOut.computeOutput();

    m_out = m_muxOut.out();
  }

  inline void tock() {
    // optimization: tock ram ONLY if it is being accessed
    if (!getBit<14>(address()))
      m_ram.tock();

    // screen is a shallow chip, doesn't need optimization
    m_screen.tock();

    m_muxScreenKbdOut.set_sel(getBit<13>(address()));
    m_muxScreenKbdOut.set_a(m_screen.out());
    m_muxScreenKbdOut.set_b(*m_kbd);
    m_muxScreenKbdOut.computeOutput();

    m_muxOut.set_sel(getBit<14>(address()));
    m_muxOut.set_a(m_ram.out());
    m_muxOut.set_b(m_muxScreenKbdOut.out());
    m_muxOut.computeOutput();

    m_out = m_muxOut.out();
  }

private:
  // { load, address[0..14] }
  uint16_t m_pins = 0;
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  uint16_t* m_kbd;

  RAM16K m_ram;
  shallow::Screen m_screen;
  DMux m_demuxRAM;
  DMux m_demuxScreenKbd;
  Mux16 m_muxScreenKbdOut;
  Mux16 m_muxOut;
};

class CPU {
public:
  // IN inM[16], instruction[16], reset
  inline uint16_t inM() const { return m_inM; }
  inline uint16_t instruction() const { return m_instruction; }
  inline bool reset() const { return getBit<31>(m_pins); }

  inline void set_inM(uint16_t val) { m_inM = val; }
  inline void set_instruction(uint16_t val) { m_instruction = val; }
  inline void set_reset(bool val) { setBit<31>(m_pins, val); }

  // OUT outM[16], addressM[15], writeM, pc[15]
  inline uint16_t outM() const { return m_outM; }
  inline uint16_t addressM() const { return m_pins & 0x7fff; }
  inline bool writeM() const { return getBit<15>(m_pins); }
  inline uint16_t pc() const { return (m_pins & 0x7fff0000) >> 16; }

  // Performs computations for combinational outputs (outM and writeM)
  void tick() {
    // ALU computations are instantaneous and happen first.
    // On the very first clock cycle, the A-register and D-register
    // will already be set to 0 by the constructors, but the register
    // values on the first clock cycle should be treated by the assembly
    // programmer as undefined. A typical first instruction loads the
    // A-register with an immediate value.

    // ALU input x
    m_alu.set_x(m_regD.out());

    // ALU input y
    m_regAorM.set_a(m_regA.out());
    m_regAorM.set_b(inM());
    m_regAorM.set_sel(getBit<12>(instruction()));
    m_regAorM.computeOutput();
    m_alu.set_y(m_regAorM.out());

    // ALU input control bits
    m_alu.set_zx(getBit<11>(instruction()));
    m_alu.set_nx(getBit<10>(instruction()));
    m_alu.set_zy(getBit<9>(instruction()));
    m_alu.set_ny(getBit<8>(instruction()));
    m_alu.set_f(getBit<7>(instruction()));
    m_alu.set_no(getBit<6>(instruction()));

    // ALU compute
    m_alu.computeOutput();

    // OUT outM[16]
    m_outM = m_alu.out();

    // writeM
    m_writeM.set_a(getBit<3>(instruction()));
    m_writeM.set_b(getBit<15>(instruction()));
    m_writeM.computeOutput();

    // OUT writeM
    set_writeM(m_writeM.out());

    // D-register
    m_regDLoad.set_a(getBit<4>(instruction()));
    m_regDLoad.set_b(getBit<15>(instruction()));
    m_regDLoad.computeOutput();
    m_regD.set_load(m_regDLoad.out());
    m_regD.set_in(m_alu.out());
    m_regD.tick();

    // PC
    m_notNG.set_in(m_alu.ng());
    m_notNG.computeOutput();
    m_notZR.set_in(m_alu.zr());
    m_notZR.computeOutput();
    m_positive.set_a(m_notNG.out());
    m_positive.set_b(m_notZR.out());
    m_positive.computeOutput();
    m_jumpLT.set_a(getBit<2>(instruction()));
    m_jumpLT.set_b(m_alu.ng());
    m_jumpLT.computeOutput();
    m_jumpEQ.set_a(getBit<1>(instruction()));
    m_jumpEQ.set_b(m_alu.zr());
    m_jumpEQ.computeOutput();
    m_jumpGT.set_a(getBit<0>(instruction()));
    m_jumpGT.set_b(m_positive.out());
    m_jumpGT.computeOutput();
    m_jumpLE.set_a(m_jumpLT.out());
    m_jumpLE.set_b(m_jumpEQ.out());
    m_jumpLE.computeOutput();
    m_doJump.set_a(m_jumpLE.out());
    m_doJump.set_b(m_jumpGT.out());
    m_doJump.computeOutput();
    m_pcLoad.set_a(m_doJump.out());
    m_pcLoad.set_b(getBit<15>(instruction()));
    m_pcLoad.computeOutput();
    m_pcInc.set_in(m_pcLoad.out());
    m_pcInc.computeOutput();

    m_pc.set_load(m_pcLoad.out());
    m_pc.set_reset(reset());
    m_pc.set_inc(m_pcInc.out());
    m_pc.set_in(m_regA.out());
    m_pc.tick();

    // A-register
    m_isAInstruction.set_in(getBit<15>(instruction()));
    m_isAInstruction.computeOutput();
    m_regAIn.set_a(m_alu.out());
    m_regAIn.set_b(instruction());
    m_regAIn.set_sel(m_isAInstruction.out());
    m_regAIn.computeOutput();
    m_regA.set_in(m_regAIn.out());
    m_regALoad.set_a(getBit<5>(instruction()));
    m_regALoad.set_b(m_isAInstruction.out());
    m_regALoad.computeOutput();
    m_regA.set_load(m_regALoad.out());
    m_regA.tick();
  }

  // Performs clocked flows (outputs addressM and pc)
  inline void tock() {
    m_regA.tock();
    m_regD.tock();
    m_pc.tock();

    // OUT pc
    set_pc(m_pc.out());

    // OUT addressM[15]
    set_addressM(m_regA.out());
  }

private:
  // { addressM[0..14], writeM, pc[0..14], reset }
  uint32_t m_pins = 0;
  uint16_t m_inM = 0;
  uint16_t m_outM = 0;
  uint16_t m_instruction = 0;

  // Registers
  Register m_regA;
  Register m_regD;

  // PC stuff
  PC m_pc;
  Not m_notNG;
  Not m_notZR;
  And m_positive;
  And m_jumpLT;
  And m_jumpEQ;
  And m_jumpGT;
  Or m_jumpLE;
  Or m_doJump;
  And m_pcLoad;
  Not m_pcInc;

  // ALU stuff
  ALU m_alu;
  Not m_isAInstruction;
  Mux16 m_regAIn;
  Mux16 m_regAorM;
  Or m_regALoad;
  And m_regDLoad;

  // writeM
  And m_writeM;

  inline void set_writeM(bool val) { setBit<15>(m_pins, val); }
  inline void set_addressM(uint16_t val) {
    m_pins = (m_pins & 0xffff8000) | (val & 0x7fff);
  }
  inline void set_pc(uint16_t val) {
    m_pins = (m_pins & 0x80000000) | ((uint32_t)(val & 0x7fff) << 16) | (m_pins & 0xffff);
  }
};

class Computer {
public:
  // IN reset
  inline bool reset() const { return m_reset; }
  inline void set_reset(bool val) { m_reset = val; }

  Computer(uint16_t* screen, uint16_t* kbd)
    : m_mem(screen, kbd) { }

  inline void set_rom(const std::vector<uint16_t>& instructions)
    { m_rom.set_rom(instructions); }

  inline Memory& mem() { return m_mem; }

  uint16_t curInstruction() const;
  uint16_t nextInstruction() const;

  std::string curInstrDbg() const;
  std::string nextInstrDbg() const;

  uint16_t curPC() const;
  uint16_t nextPC() const;

  inline void tock() {
    m_rom.set_address(m_cpu.pc());

    m_mem.set_address(m_cpu.addressM());
    m_mem.tick();

    m_cpu.set_reset(reset());
    m_cpu.set_instruction(m_rom.instruction());
    m_cpu.set_inM(m_mem.out());
    m_cpu.tick();

    // outM and writeM are updated by the CPU tick()
    m_mem.set_in(m_cpu.outM());
    m_mem.set_load(m_cpu.writeM());
    m_mem.tick();

    m_mem.tock();
    m_cpu.tock();
  }

private:
  bool m_reset = false;

  shallow::ROM32K m_rom;
  Memory          m_mem;
  CPU             m_cpu;
};

uint16_t Computer::curInstruction() const { return m_rom.instruction(); }
uint16_t Computer::nextInstruction() const
  { return m_rom.instructionAt(m_cpu.pc()); }

std::string Computer::curInstrDbg() const
  { return FileUtils::disassemble(curInstruction()); }

std::string Computer::nextInstrDbg() const
  { return FileUtils::disassemble(nextInstruction()); }

uint16_t Computer::curPC() const { return m_rom.address(); }
uint16_t Computer::nextPC() const { return m_cpu.pc(); }
