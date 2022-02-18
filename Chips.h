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
  // INPUT
  inline uint16_t in() { return m_in; }

  inline void set_in(uint16_t val) { m_in = val; }

  // OUTPUT
  inline uint16_t out() { return m_out; }

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
  inline uint16_t a() { return m_a; }
  inline uint16_t b() { return m_b; }

  inline void set_a(uint16_t val) { m_a = val; }
  inline void set_b(uint16_t val) { m_b = val; }

  // OUTPUT
  inline uint16_t out() { return m_out; }

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
  inline uint16_t a() { return m_a; }
  inline uint16_t b() { return m_b; }

  inline void set_a(uint16_t val) { m_a = val; }
  inline void set_b(uint16_t val) { m_b = val; }

  // OUTPUT
  inline uint16_t out() { return m_out; }

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
  inline uint16_t a() { return m_a; }
  inline uint16_t b() { return m_b; }
  inline bool sel() { return getBit<0>(m_pins); }

  inline void set_a(uint16_t val) { m_a = val; }
  inline void set_b(uint16_t val) { m_b = val; }
  inline void set_sel(bool val) { setBit<0>(m_pins, val); }

  // OUTPUT
  inline uint16_t out() { return m_out; }

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
  // INPUT a[16], b[16], c[16], d[16], sel[2]
  inline uint16_t a() { return m_a; }
  inline uint16_t b() { return m_b; }
  inline uint16_t c() { return m_c; }
  inline uint16_t d() { return m_d; }
  inline uint16_t sel() { return m_pins & 0x3; }

  inline void set_a(uint16_t val) { m_a = val; }
  inline void set_b(uint16_t val) { m_b = val; }
  inline void set_c(uint16_t val) { m_c = val; }
  inline void set_d(uint16_t val) { m_d = val; }
  inline void set_sel(uint8_t val) {
    m_pins = (m_pins & 0xfc) | (val & 0x3);
  }

  // OUTPUT out[16]
  inline uint16_t out() { return m_out; }

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
  inline uint16_t a() { return m_a; }
  inline uint16_t b() { return m_b; }
  inline uint16_t c() { return m_c; }
  inline uint16_t d() { return m_d; }
  inline uint16_t e() { return m_e; }
  inline uint16_t f() { return m_f; }
  inline uint16_t g() { return m_g; }
  inline uint16_t h() { return m_h; }
  inline uint8_t sel() { return (m_pins & 0x7); }

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
  inline uint16_t out() { return m_out; }

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
