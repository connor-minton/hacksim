#pragma once

#include <cstring>
#include <bitset>

// class Chip {
// public:
//   virtual ~Chip() = default;
//   virtual void computeOutput() = 0;
// };

// class CombinationalChip : public Chip {
// public:
//   virtual ~CombinationalChip() = default;
// };

// class SequentialChip : public Chip {
// public:
//   virtual ~SequentialChip() = default;
// };

template<uint8_t N, typename T>
static inline void setBit(T& bits, bool value) {
  constexpr uint64_t mask = (1 << N);
  if (value) bits |= mask;
  else       bits &= ~mask;
}

template<uint8_t N, typename T>
static inline bool getBit(T bits) {
  constexpr uint64_t mask = (1 << N);
  return (bits & mask) != 0;
}

class Nand {
public:
  // INPUT
  inline bool a() { return getBit<0>(pins); }
  inline bool b() { return getBit<1>(pins); }

  inline void set_a(bool val) { setBit<0>(pins, val); }
  inline void set_b(bool val) { setBit<1>(pins, val); }

  // OUTPUT
  inline bool out() { return (pins & 0x4) != 0; }

  Nand() { computeOutput(); }

  inline void computeOutput() {
    set_out(!(a() && b()));
  }

private:
  // { a, b, out }
  uint8_t pins = 0;

  inline void set_out(bool val) { setBit<2>(pins, val); }
};

class Not {
public:
  // INPUT
  inline bool in() { return getBit<0>(pins); }

  inline void set_in(bool val) {
    if (val) pins |= 0x1;
    else     pins &= ~0x1;
  }

  // OUTPUT
  inline bool out() { return getBit<1>(pins); }

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
  inline bool a() { return getBit<0>(pins); }
  inline bool b() { return getBit<1>(pins); }

  inline void set_a(bool val) { setBit<0>(pins, val); }
  inline void set_b(bool val) { setBit<1>(pins, val); }

  // OUTPUT
  inline bool out() { return getBit<2>(pins); }

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
  inline bool a() { return getBit<0>(pins); }
  inline bool b() { return getBit<1>(pins); }

  inline void set_a(bool val) { setBit<0>(pins, val); }
  inline void set_b(bool val) { setBit<1>(pins, val); }

  // OUTPUT
  inline bool out() { return getBit<2>(pins); }

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
  inline bool a() { return getBit<0>(pins); }
  inline bool b() { return getBit<1>(pins); }

  inline void set_a(bool val) { setBit<0>(pins, val); }
  inline void set_b(bool val) { setBit<1>(pins, val); }

  // OUTPUT
  inline bool out() { return getBit<2>(pins); }

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
  inline bool a() { return getBit<0>(pins); }
  inline bool b() { return getBit<1>(pins); }
  inline bool sel() { return getBit<2>(pins); }

  inline void set_a(bool val) { setBit<0>(pins, val); }
  inline void set_b(bool val) { setBit<1>(pins, val); }
  inline void set_sel(bool val) { setBit<2>(pins, val); }

  // OUTPUT
  inline bool out() { return getBit<3>(pins); }

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
  inline bool in() { return getBit<0>(pins); }
  inline bool sel() { return getBit<1>(pins); }

  inline void set_in(bool val) { setBit<0>(pins, val); }
  inline void set_sel(bool val) { setBit<1>(pins, val); }

  // OUTPUT
  inline bool a() { return getBit<2>(pins); }
  inline bool b() { return getBit<3>(pins); }

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
  bool in[16];
  bool out[16];

  Not16() : in{0}, out{0} { computeOutput(); }

  inline void computeOutput() {
    m_nots[0].set_in(in[0]);
    m_nots[1].set_in(in[1]);
    m_nots[2].set_in(in[2]);
    m_nots[3].set_in(in[3]);
    m_nots[4].set_in(in[4]);
    m_nots[5].set_in(in[5]);
    m_nots[6].set_in(in[6]);
    m_nots[7].set_in(in[7]);
    m_nots[8].set_in(in[8]);
    m_nots[9].set_in(in[9]);
    m_nots[10].set_in(in[10]);
    m_nots[11].set_in(in[11]);
    m_nots[12].set_in(in[12]);
    m_nots[13].set_in(in[13]);
    m_nots[14].set_in(in[14]);
    m_nots[15].set_in(in[15]);

    m_nots[0].computeOutput();
    m_nots[1].computeOutput();
    m_nots[2].computeOutput();
    m_nots[3].computeOutput();
    m_nots[4].computeOutput();
    m_nots[5].computeOutput();
    m_nots[6].computeOutput();
    m_nots[7].computeOutput();
    m_nots[8].computeOutput();
    m_nots[9].computeOutput();
    m_nots[10].computeOutput();
    m_nots[11].computeOutput();
    m_nots[12].computeOutput();
    m_nots[13].computeOutput();
    m_nots[14].computeOutput();
    m_nots[15].computeOutput();

    out[0] = m_nots[0].out();
    out[1] = m_nots[1].out();
    out[2] = m_nots[2].out();
    out[3] = m_nots[3].out();
    out[4] = m_nots[4].out();
    out[5] = m_nots[5].out();
    out[6] = m_nots[6].out();
    out[7] = m_nots[7].out();
    out[8] = m_nots[8].out();
    out[9] = m_nots[9].out();
    out[10] = m_nots[10].out();
    out[11] = m_nots[11].out();
    out[12] = m_nots[12].out();
    out[13] = m_nots[13].out();
    out[14] = m_nots[14].out();
    out[15] = m_nots[15].out();
  }

private:
  Not m_nots[16];
};

class And16 {
public:
  bool a[16];
  bool b[16];
  bool out[16];

  And16() : a{0}, b{0} { computeOutput(); }

  inline void computeOutput() {
    m_ands[0].set_a(a[0]);
    m_ands[1].set_a(a[1]);
    m_ands[2].set_a(a[2]);
    m_ands[3].set_a(a[3]);
    m_ands[4].set_a(a[4]);
    m_ands[5].set_a(a[5]);
    m_ands[6].set_a(a[6]);
    m_ands[7].set_a(a[7]);
    m_ands[8].set_a(a[8]);
    m_ands[9].set_a(a[9]);
    m_ands[10].set_a(a[10]);
    m_ands[11].set_a(a[11]);
    m_ands[12].set_a(a[12]);
    m_ands[13].set_a(a[13]);
    m_ands[14].set_a(a[14]);
    m_ands[15].set_a(a[15]);

    m_ands[0].set_b(b[0]);
    m_ands[1].set_b(b[1]);
    m_ands[2].set_b(b[2]);
    m_ands[3].set_b(b[3]);
    m_ands[4].set_b(b[4]);
    m_ands[5].set_b(b[5]);
    m_ands[6].set_b(b[6]);
    m_ands[7].set_b(b[7]);
    m_ands[8].set_b(b[8]);
    m_ands[9].set_b(b[9]);
    m_ands[10].set_b(b[10]);
    m_ands[11].set_b(b[11]);
    m_ands[12].set_b(b[12]);
    m_ands[13].set_b(b[13]);
    m_ands[14].set_b(b[14]);
    m_ands[15].set_b(b[15]);

    m_ands[0].computeOutput();
    m_ands[1].computeOutput();
    m_ands[2].computeOutput();
    m_ands[3].computeOutput();
    m_ands[4].computeOutput();
    m_ands[5].computeOutput();
    m_ands[6].computeOutput();
    m_ands[7].computeOutput();
    m_ands[8].computeOutput();
    m_ands[9].computeOutput();
    m_ands[10].computeOutput();
    m_ands[11].computeOutput();
    m_ands[12].computeOutput();
    m_ands[13].computeOutput();
    m_ands[14].computeOutput();
    m_ands[15].computeOutput();

    out[0] = m_ands[0].out();
    out[1] = m_ands[1].out();
    out[2] = m_ands[2].out();
    out[3] = m_ands[3].out();
    out[4] = m_ands[4].out();
    out[5] = m_ands[5].out();
    out[6] = m_ands[6].out();
    out[7] = m_ands[7].out();
    out[8] = m_ands[8].out();
    out[9] = m_ands[9].out();
    out[10] = m_ands[10].out();
    out[11] = m_ands[11].out();
    out[12] = m_ands[12].out();
    out[13] = m_ands[13].out();
    out[14] = m_ands[14].out();
    out[15] = m_ands[15].out();
  }

private:
  And m_ands[16];
};

class Or16 {
public:
  bool a[16];
  bool b[16];
  bool out[16];

  Or16() : a{0}, b{0} { computeOutput(); }

  inline void computeOutput() {
    m_ors[0].set_a(a[0]);
    m_ors[1].set_a(a[1]);
    m_ors[2].set_a(a[2]);
    m_ors[3].set_a(a[3]);
    m_ors[4].set_a(a[4]);
    m_ors[5].set_a(a[5]);
    m_ors[6].set_a(a[6]);
    m_ors[7].set_a(a[7]);
    m_ors[8].set_a(a[8]);
    m_ors[9].set_a(a[9]);
    m_ors[10].set_a(a[10]);
    m_ors[11].set_a(a[11]);
    m_ors[12].set_a(a[12]);
    m_ors[13].set_a(a[13]);
    m_ors[14].set_a(a[14]);
    m_ors[15].set_a(a[15]);

    m_ors[0].set_b(b[0]);
    m_ors[1].set_b(b[1]);
    m_ors[2].set_b(b[2]);
    m_ors[3].set_b(b[3]);
    m_ors[4].set_b(b[4]);
    m_ors[5].set_b(b[5]);
    m_ors[6].set_b(b[6]);
    m_ors[7].set_b(b[7]);
    m_ors[8].set_b(b[8]);
    m_ors[9].set_b(b[9]);
    m_ors[10].set_b(b[10]);
    m_ors[11].set_b(b[11]);
    m_ors[12].set_b(b[12]);
    m_ors[13].set_b(b[13]);
    m_ors[14].set_b(b[14]);
    m_ors[15].set_b(b[15]);

    m_ors[0].computeOutput();
    m_ors[1].computeOutput();
    m_ors[2].computeOutput();
    m_ors[3].computeOutput();
    m_ors[4].computeOutput();
    m_ors[5].computeOutput();
    m_ors[6].computeOutput();
    m_ors[7].computeOutput();
    m_ors[8].computeOutput();
    m_ors[9].computeOutput();
    m_ors[10].computeOutput();
    m_ors[11].computeOutput();
    m_ors[12].computeOutput();
    m_ors[13].computeOutput();
    m_ors[14].computeOutput();
    m_ors[15].computeOutput();

    out[0] = m_ors[0].out();
    out[1] = m_ors[1].out();
    out[2] = m_ors[2].out();
    out[3] = m_ors[3].out();
    out[4] = m_ors[4].out();
    out[5] = m_ors[5].out();
    out[6] = m_ors[6].out();
    out[7] = m_ors[7].out();
    out[8] = m_ors[8].out();
    out[9] = m_ors[9].out();
    out[10] = m_ors[10].out();
    out[11] = m_ors[11].out();
    out[12] = m_ors[12].out();
    out[13] = m_ors[13].out();
    out[14] = m_ors[14].out();
    out[15] = m_ors[15].out();
  }

private:
  Or m_ors[16];
};

class Mux16 {
public:
  bool a[16];
  bool b[16];
  bool sel;
  bool out[16];

  Mux16() : a{0}, b{0}, sel(false) { computeOutput(); }

  inline void computeOutput() {
    m_muxs[0].set_a(a[0]);
    m_muxs[1].set_a(a[1]);
    m_muxs[2].set_a(a[2]);
    m_muxs[3].set_a(a[3]);
    m_muxs[4].set_a(a[4]);
    m_muxs[5].set_a(a[5]);
    m_muxs[6].set_a(a[6]);
    m_muxs[7].set_a(a[7]);
    m_muxs[8].set_a(a[8]);
    m_muxs[9].set_a(a[9]);
    m_muxs[10].set_a(a[10]);
    m_muxs[11].set_a(a[11]);
    m_muxs[12].set_a(a[12]);
    m_muxs[13].set_a(a[13]);
    m_muxs[14].set_a(a[14]);
    m_muxs[15].set_a(a[15]);

    m_muxs[0].set_b(b[0]);
    m_muxs[1].set_b(b[1]);
    m_muxs[2].set_b(b[2]);
    m_muxs[3].set_b(b[3]);
    m_muxs[4].set_b(b[4]);
    m_muxs[5].set_b(b[5]);
    m_muxs[6].set_b(b[6]);
    m_muxs[7].set_b(b[7]);
    m_muxs[8].set_b(b[8]);
    m_muxs[9].set_b(b[9]);
    m_muxs[10].set_b(b[10]);
    m_muxs[11].set_b(b[11]);
    m_muxs[12].set_b(b[12]);
    m_muxs[13].set_b(b[13]);
    m_muxs[14].set_b(b[14]);
    m_muxs[15].set_b(b[15]);

    m_muxs[0].set_sel(sel);
    m_muxs[1].set_sel(sel);
    m_muxs[2].set_sel(sel);
    m_muxs[3].set_sel(sel);
    m_muxs[4].set_sel(sel);
    m_muxs[5].set_sel(sel);
    m_muxs[6].set_sel(sel);
    m_muxs[7].set_sel(sel);
    m_muxs[8].set_sel(sel);
    m_muxs[9].set_sel(sel);
    m_muxs[10].set_sel(sel);
    m_muxs[11].set_sel(sel);
    m_muxs[12].set_sel(sel);
    m_muxs[13].set_sel(sel);
    m_muxs[14].set_sel(sel);
    m_muxs[15].set_sel(sel);

    m_muxs[0].computeOutput();
    m_muxs[1].computeOutput();
    m_muxs[2].computeOutput();
    m_muxs[3].computeOutput();
    m_muxs[4].computeOutput();
    m_muxs[5].computeOutput();
    m_muxs[6].computeOutput();
    m_muxs[7].computeOutput();
    m_muxs[8].computeOutput();
    m_muxs[9].computeOutput();
    m_muxs[10].computeOutput();
    m_muxs[11].computeOutput();
    m_muxs[12].computeOutput();
    m_muxs[13].computeOutput();
    m_muxs[14].computeOutput();
    m_muxs[15].computeOutput();

    out[0] = m_muxs[0].out();
    out[1] = m_muxs[1].out();
    out[2] = m_muxs[2].out();
    out[3] = m_muxs[3].out();
    out[4] = m_muxs[4].out();
    out[5] = m_muxs[5].out();
    out[6] = m_muxs[6].out();
    out[7] = m_muxs[7].out();
    out[8] = m_muxs[8].out();
    out[9] = m_muxs[9].out();
    out[10] = m_muxs[10].out();
    out[11] = m_muxs[11].out();
    out[12] = m_muxs[12].out();
    out[13] = m_muxs[13].out();
    out[14] = m_muxs[14].out();
    out[15] = m_muxs[15].out();
  }

private:
  Mux m_muxs[16];
};

class Or8Way {
public:
  bool in[8];
  bool out;

  Or8Way() : in{0} { computeOutput(); }

  inline void computeOutput() {
    m_or01.set_a(in[0]);
    m_or01.set_b(in[1]);
    m_or01.computeOutput();

    m_or23.set_a(in[2]);
    m_or23.set_b(in[3]);
    m_or23.computeOutput();

    m_or45.set_a(in[4]);
    m_or45.set_b(in[5]);
    m_or45.computeOutput();

    m_or67.set_a(in[6]);
    m_or67.set_b(in[7]);
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

    out = m_orOut.out();
  }

private:
  Or m_or01;
  Or m_or23;
  Or m_or45;
  Or m_or67;

  Or m_or0123;
  Or m_or4567;

  Or m_orOut;
};

class Mux4Way16 {
public:
  bool a[16];
  bool b[16];
  bool c[16];
  bool d[16];
  bool sel[2];
  bool out[16];

  Mux4Way16() : a{0}, b{0}, c{0}, d{0}, sel{0} { computeOutput(); }

  inline void computeOutput() {
    memcpy(m_ab.a, a, sizeof(a));
    memcpy(m_ab.b, b, sizeof(b));
    m_ab.sel = sel[0];
    m_ab.computeOutput();

    memcpy(m_cd.a, c, sizeof(c));
    memcpy(m_cd.b, d, sizeof(d));
    m_cd.sel = sel[0];
    m_cd.computeOutput();

    memcpy(m_muxOut.a, m_ab.out, sizeof(m_ab.out));
    memcpy(m_muxOut.b, m_cd.out, sizeof(m_cd.out));
    m_muxOut.sel = sel[1];
    m_muxOut.computeOutput();

    memcpy(out, m_muxOut.out, sizeof(out));
  }

private:
  Mux16 m_ab;
  Mux16 m_cd;
  Mux16 m_muxOut;
};

class Mux8Way16 {
public:
  bool a[16];
  bool b[16];
  bool c[16];
  bool d[16];
  bool e[16];
  bool f[16];
  bool g[16];
  bool h[16];
  bool sel[3];
  bool out[16];

  Mux8Way16() : a{0}, b{0}, c{0}, d{0},
                e{0}, f{0}, g{0}, h{0}, sel{0} { computeOutput(); }

  inline void computeOutput() {
    memcpy(m_abcd.a, a, sizeof(a));
    memcpy(m_abcd.b, b, sizeof(b));
    memcpy(m_abcd.c, c, sizeof(c));
    memcpy(m_abcd.d, d, sizeof(d));
    m_abcd.sel[0] = sel[0];
    m_abcd.sel[1] = sel[1];
    m_abcd.computeOutput();

    memcpy(m_efgh.a, e, sizeof(e));
    memcpy(m_efgh.b, f, sizeof(f));
    memcpy(m_efgh.c, g, sizeof(g));
    memcpy(m_efgh.d, h, sizeof(h));
    m_efgh.sel[0] = sel[0];
    m_efgh.sel[1] = sel[1];
    m_efgh.computeOutput();

    memcpy(m_muxOut.a, m_abcd.out, sizeof(m_abcd.out));
    memcpy(m_muxOut.b, m_efgh.out, sizeof(m_efgh.out));
    m_muxOut.sel = sel[2];
    m_muxOut.computeOutput();

    memcpy(out, m_muxOut.out, sizeof(out));
  }

private:
  Mux4Way16 m_abcd;
  Mux4Way16 m_efgh;
  Mux16 m_muxOut;
};

class DMux4Way {
public:
  bool in;
  bool sel[2];
  bool a;
  bool b;
  bool c;
  bool d;

  DMux4Way() : in(false), sel{0} { computeOutput(); }

  inline void computeOutput() {
    m_dm1.set_in(in);
    m_dm1.set_sel(sel[1]);
    m_dm1.computeOutput();

    m_dm2.set_in(m_dm1.a());
    m_dm2.set_sel(sel[0]);
    m_dm2.computeOutput();

    m_dm3.set_in(m_dm1.b());
    m_dm3.set_sel(sel[0]);
    m_dm3.computeOutput();

    a = m_dm2.a();
    b = m_dm2.b();
    c = m_dm3.a();
    d = m_dm3.b();
  }

private:
  DMux m_dm1;
  DMux m_dm2;
  DMux m_dm3;
};

class DMux8Way {
public:
  DMux8Way() { computeOutput(); }

  // INPUT
  inline bool in() { return pins[0]; }
  inline bool sel(int i) { return pins[1+i]; }

  inline void set_in(bool val)         { pins[0] = val; }
  inline void set_sel(int i, bool val) { pins[1+i] = val; }

  // OUTPUT
  inline bool a() { return pins[4]; }
  inline bool b() { return pins[5]; }
  inline bool c() { return pins[6]; }
  inline bool d() { return pins[7]; }
  inline bool e() { return pins[8]; }
  inline bool f() { return pins[9]; }
  inline bool g() { return pins[10]; }
  inline bool h() { return pins[11]; }

  inline void computeOutput() {
    m_mux.set_in(in());
    m_mux.set_sel(sel(2));
    m_mux.computeOutput();

    m_mux4abcd.in = m_mux.a();
    m_mux4abcd.sel[0] = sel(0);
    m_mux4abcd.sel[1] = sel(1);
    m_mux4abcd.computeOutput();

    m_mux4efgh.in = m_mux.b();
    m_mux4efgh.sel[0] = sel(0);
    m_mux4efgh.sel[1] = sel(1);
    m_mux4efgh.computeOutput();

    set_a(m_mux4abcd.a);
    set_b(m_mux4abcd.b);
    set_c(m_mux4abcd.c);
    set_d(m_mux4abcd.d);
    set_e(m_mux4efgh.a);
    set_f(m_mux4efgh.b);
    set_g(m_mux4efgh.c);
    set_h(m_mux4efgh.d);
  }

private:
  // { in, sel[0..2], a, b, ..., g, h }
  std::bitset<12> pins;

  DMux m_mux;
  DMux4Way m_mux4abcd;
  DMux4Way m_mux4efgh;

  inline void set_a(bool val) { pins[4] = val; }
  inline void set_b(bool val) { pins[5] = val; }
  inline void set_c(bool val) { pins[6] = val; }
  inline void set_d(bool val) { pins[7] = val; }
  inline void set_e(bool val) { pins[8] = val; }
  inline void set_f(bool val) { pins[9] = val; }
  inline void set_g(bool val) { pins[10] = val; }
  inline void set_h(bool val) { pins[11] = val; }
};
