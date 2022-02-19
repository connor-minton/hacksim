#pragma once

#include <cstring>
#include <bitset>

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
    m_nots[0].set_in(getBit<0>(m_in));
    m_nots[1].set_in(getBit<1>(m_in));
    m_nots[2].set_in(getBit<2>(m_in));
    m_nots[3].set_in(getBit<3>(m_in));
    m_nots[4].set_in(getBit<4>(m_in));
    m_nots[5].set_in(getBit<5>(m_in));
    m_nots[6].set_in(getBit<6>(m_in));
    m_nots[7].set_in(getBit<7>(m_in));
    m_nots[8].set_in(getBit<8>(m_in));
    m_nots[9].set_in(getBit<9>(m_in));
    m_nots[10].set_in(getBit<10>(m_in));
    m_nots[11].set_in(getBit<11>(m_in));
    m_nots[12].set_in(getBit<12>(m_in));
    m_nots[13].set_in(getBit<13>(m_in));
    m_nots[14].set_in(getBit<14>(m_in));
    m_nots[15].set_in(getBit<15>(m_in));

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

    setBit<0>(m_out, m_nots[0].out());
    setBit<1>(m_out, m_nots[1].out());
    setBit<2>(m_out, m_nots[2].out());
    setBit<3>(m_out, m_nots[3].out());
    setBit<4>(m_out, m_nots[4].out());
    setBit<5>(m_out, m_nots[5].out());
    setBit<6>(m_out, m_nots[6].out());
    setBit<7>(m_out, m_nots[7].out());
    setBit<8>(m_out, m_nots[8].out());
    setBit<9>(m_out, m_nots[9].out());
    setBit<10>(m_out, m_nots[10].out());
    setBit<11>(m_out, m_nots[11].out());
    setBit<12>(m_out, m_nots[12].out());
    setBit<13>(m_out, m_nots[13].out());
    setBit<14>(m_out, m_nots[14].out());
    setBit<15>(m_out, m_nots[15].out());
  }

private:
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  Not m_nots[16];
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
    m_ands[0].set_a( getBit<0>(m_a));
    m_ands[1].set_a( getBit<1>(m_a));
    m_ands[2].set_a( getBit<2>(m_a));
    m_ands[3].set_a( getBit<3>(m_a));
    m_ands[4].set_a( getBit<4>(m_a));
    m_ands[5].set_a( getBit<5>(m_a));
    m_ands[6].set_a( getBit<6>(m_a));
    m_ands[7].set_a( getBit<7>(m_a));
    m_ands[8].set_a( getBit<8>(m_a));
    m_ands[9].set_a( getBit<9>(m_a));
    m_ands[10].set_a(getBit<10>(m_a));
    m_ands[11].set_a(getBit<11>(m_a));
    m_ands[12].set_a(getBit<12>(m_a));
    m_ands[13].set_a(getBit<13>(m_a));
    m_ands[14].set_a(getBit<14>(m_a));
    m_ands[15].set_a(getBit<15>(m_a));

    m_ands[0].set_b( getBit<0>(m_b));
    m_ands[1].set_b( getBit<1>(m_b));
    m_ands[2].set_b( getBit<2>(m_b));
    m_ands[3].set_b( getBit<3>(m_b));
    m_ands[4].set_b( getBit<4>(m_b));
    m_ands[5].set_b( getBit<5>(m_b));
    m_ands[6].set_b( getBit<6>(m_b));
    m_ands[7].set_b( getBit<7>(m_b));
    m_ands[8].set_b( getBit<8>(m_b));
    m_ands[9].set_b( getBit<9>(m_b));
    m_ands[10].set_b(getBit<10>(m_b));
    m_ands[11].set_b(getBit<11>(m_b));
    m_ands[12].set_b(getBit<12>(m_b));
    m_ands[13].set_b(getBit<13>(m_b));
    m_ands[14].set_b(getBit<14>(m_b));
    m_ands[15].set_b(getBit<15>(m_b));

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

    setBit<0>(m_out, m_ands[0].out());
    setBit<1>(m_out, m_ands[1].out());
    setBit<2>(m_out, m_ands[2].out());
    setBit<3>(m_out, m_ands[3].out());
    setBit<4>(m_out, m_ands[4].out());
    setBit<5>(m_out, m_ands[5].out());
    setBit<6>(m_out, m_ands[6].out());
    setBit<7>(m_out, m_ands[7].out());
    setBit<8>(m_out, m_ands[8].out());
    setBit<9>(m_out, m_ands[9].out());
    setBit<10>(m_out, m_ands[10].out());
    setBit<11>(m_out, m_ands[11].out());
    setBit<12>(m_out, m_ands[12].out());
    setBit<13>(m_out, m_ands[13].out());
    setBit<14>(m_out, m_ands[14].out());
    setBit<15>(m_out, m_ands[15].out());
  }

private:
  uint16_t m_a = 0;
  uint16_t m_b = 0;
  uint16_t m_out = 0;

  And m_ands[16];

  inline void set_out(uint16_t val) { m_out = val; }
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
    m_ors[0].set_a( getBit<0>(m_a));
    m_ors[1].set_a( getBit<1>(m_a));
    m_ors[2].set_a( getBit<2>(m_a));
    m_ors[3].set_a( getBit<3>(m_a));
    m_ors[4].set_a( getBit<4>(m_a));
    m_ors[5].set_a( getBit<5>(m_a));
    m_ors[6].set_a( getBit<6>(m_a));
    m_ors[7].set_a( getBit<7>(m_a));
    m_ors[8].set_a( getBit<8>(m_a));
    m_ors[9].set_a( getBit<9>(m_a));
    m_ors[10].set_a(getBit<10>(m_a));
    m_ors[11].set_a(getBit<11>(m_a));
    m_ors[12].set_a(getBit<12>(m_a));
    m_ors[13].set_a(getBit<13>(m_a));
    m_ors[14].set_a(getBit<14>(m_a));
    m_ors[15].set_a(getBit<15>(m_a));

    m_ors[0].set_b( getBit<0>(m_b));
    m_ors[1].set_b( getBit<1>(m_b));
    m_ors[2].set_b( getBit<2>(m_b));
    m_ors[3].set_b( getBit<3>(m_b));
    m_ors[4].set_b( getBit<4>(m_b));
    m_ors[5].set_b( getBit<5>(m_b));
    m_ors[6].set_b( getBit<6>(m_b));
    m_ors[7].set_b( getBit<7>(m_b));
    m_ors[8].set_b( getBit<8>(m_b));
    m_ors[9].set_b( getBit<9>(m_b));
    m_ors[10].set_b(getBit<10>(m_b));
    m_ors[11].set_b(getBit<11>(m_b));
    m_ors[12].set_b(getBit<12>(m_b));
    m_ors[13].set_b(getBit<13>(m_b));
    m_ors[14].set_b(getBit<14>(m_b));
    m_ors[15].set_b(getBit<15>(m_b));

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

    setBit<0>(m_out, m_ors[0].out());
    setBit<1>(m_out, m_ors[1].out());
    setBit<2>(m_out, m_ors[2].out());
    setBit<3>(m_out, m_ors[3].out());
    setBit<4>(m_out, m_ors[4].out());
    setBit<5>(m_out, m_ors[5].out());
    setBit<6>(m_out, m_ors[6].out());
    setBit<7>(m_out, m_ors[7].out());
    setBit<8>(m_out, m_ors[8].out());
    setBit<9>(m_out, m_ors[9].out());
    setBit<10>(m_out, m_ors[10].out());
    setBit<11>(m_out, m_ors[11].out());
    setBit<12>(m_out, m_ors[12].out());
    setBit<13>(m_out, m_ors[13].out());
    setBit<14>(m_out, m_ors[14].out());
    setBit<15>(m_out, m_ors[15].out());
  }

private:
  uint16_t m_a = 0;
  uint16_t m_b = 0;
  uint16_t m_out = 0;

  Or m_ors[16];
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
    m_muxs[0].set_a( getBit<0>(m_a));
    m_muxs[1].set_a( getBit<1>(m_a));
    m_muxs[2].set_a( getBit<2>(m_a));
    m_muxs[3].set_a( getBit<3>(m_a));
    m_muxs[4].set_a( getBit<4>(m_a));
    m_muxs[5].set_a( getBit<5>(m_a));
    m_muxs[6].set_a( getBit<6>(m_a));
    m_muxs[7].set_a( getBit<7>(m_a));
    m_muxs[8].set_a( getBit<8>(m_a));
    m_muxs[9].set_a( getBit<9>(m_a));
    m_muxs[10].set_a(getBit<10>(m_a));
    m_muxs[11].set_a(getBit<11>(m_a));
    m_muxs[12].set_a(getBit<12>(m_a));
    m_muxs[13].set_a(getBit<13>(m_a));
    m_muxs[14].set_a(getBit<14>(m_a));
    m_muxs[15].set_a(getBit<15>(m_a));

    m_muxs[0].set_b( getBit<0>(m_b));
    m_muxs[1].set_b( getBit<1>(m_b));
    m_muxs[2].set_b( getBit<2>(m_b));
    m_muxs[3].set_b( getBit<3>(m_b));
    m_muxs[4].set_b( getBit<4>(m_b));
    m_muxs[5].set_b( getBit<5>(m_b));
    m_muxs[6].set_b( getBit<6>(m_b));
    m_muxs[7].set_b( getBit<7>(m_b));
    m_muxs[8].set_b( getBit<8>(m_b));
    m_muxs[9].set_b( getBit<9>(m_b));
    m_muxs[10].set_b(getBit<10>(m_b));
    m_muxs[11].set_b(getBit<11>(m_b));
    m_muxs[12].set_b(getBit<12>(m_b));
    m_muxs[13].set_b(getBit<13>(m_b));
    m_muxs[14].set_b(getBit<14>(m_b));
    m_muxs[15].set_b(getBit<15>(m_b));

    m_muxs[0].set_sel(sel());
    m_muxs[1].set_sel(sel());
    m_muxs[2].set_sel(sel());
    m_muxs[3].set_sel(sel());
    m_muxs[4].set_sel(sel());
    m_muxs[5].set_sel(sel());
    m_muxs[6].set_sel(sel());
    m_muxs[7].set_sel(sel());
    m_muxs[8].set_sel(sel());
    m_muxs[9].set_sel(sel());
    m_muxs[10].set_sel(sel());
    m_muxs[11].set_sel(sel());
    m_muxs[12].set_sel(sel());
    m_muxs[13].set_sel(sel());
    m_muxs[14].set_sel(sel());
    m_muxs[15].set_sel(sel());

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

    setBit<0>(m_out, m_muxs[0].out());
    setBit<1>(m_out, m_muxs[1].out());
    setBit<2>(m_out, m_muxs[2].out());
    setBit<3>(m_out, m_muxs[3].out());
    setBit<4>(m_out, m_muxs[4].out());
    setBit<5>(m_out, m_muxs[5].out());
    setBit<6>(m_out, m_muxs[6].out());
    setBit<7>(m_out, m_muxs[7].out());
    setBit<8>(m_out, m_muxs[8].out());
    setBit<9>(m_out, m_muxs[9].out());
    setBit<10>(m_out, m_muxs[10].out());
    setBit<11>(m_out, m_muxs[11].out());
    setBit<12>(m_out, m_muxs[12].out());
    setBit<13>(m_out, m_muxs[13].out());
    setBit<14>(m_out, m_muxs[14].out());
    setBit<15>(m_out, m_muxs[15].out());
  }

private:
  // extra pins
  // { m_sel }
  uint8_t m_pins = 0;
  uint16_t m_a = 0;
  uint16_t m_b = 0;
  uint16_t m_out = 0;

  Mux m_muxs[16];
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
  inline uint8_t sel() const { return m_pins & 0x6; }

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
  inline bool in() { return getBit<0>(m_pins); }

  inline void set_in(bool val) { setBit<0>(m_pins, val); }

  // OUTPUT out
  inline bool out() { return getBit<1>(m_pins); }

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
  inline bool in() { return getBit<0>(m_pins); }
  inline bool load() { return getBit<1>(m_pins); }

  inline void set_in(bool val) { setBit<0>(m_pins, val); }
  inline void set_load(bool val) { setBit<1>(m_pins, val); }

  // OUTPUT out
  inline bool out() { return getBit<2>(m_pins); }

  Bit() { tock(); }

  inline void tock() {
    m_mux.set_a(m_dff.out());
    m_mux.set_b(in());
    m_mux.set_sel(load());
    m_mux.computeOutput();

    m_dff.set_in(m_mux.out());
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
  inline uint16_t in() { return m_in; }
  inline bool load() { return getBit<0>(m_pins); }

  inline void set_in(uint16_t val) { m_in = val; }
  inline void set_load(bool val) { setBit<0>(m_pins, val); }

  // OUTPUT out[16]
  inline uint16_t out() { return m_out; }

  Register() { tock(); }

  inline void tock() {
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