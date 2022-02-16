#pragma once

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

class Nand {
public:
  bool a;
  bool b;
  bool out;

  Nand() : a(false), b(false) { computeOutput(); }

  inline void computeOutput() {
    out = !(a && b);
  }
};

class Not {
public:
  bool in;
  bool out;

  Not() : in(false) { computeOutput(); }

  inline void computeOutput() {
    m_nand.a = in;
    m_nand.b = in;
    m_nand.computeOutput();
    out = m_nand.out;
  }

private:
  Nand m_nand;
};

class And {
public:
  bool a;
  bool b;
  bool out;

  And() : a(false), b(false) { computeOutput(); }

  inline void computeOutput() {
    m_nand.a = a;
    m_nand.b = b;
    m_nand.computeOutput();
    m_not.in = m_nand.out;
    m_not.computeOutput();
    out = m_not.out;
  }

private:
  Nand m_nand;
  Not m_not;
};

class Or {
public:
  bool a;
  bool b;
  bool out;

  Or() : a(false), b(false) { computeOutput(); }

  inline void computeOutput() {
    m_notA.in = a;
    m_notB.in = b;
    m_notA.computeOutput();
    m_notB.computeOutput();
    m_nand.a = m_notA.out;
    m_nand.b = m_notB.out;
    m_nand.computeOutput();
    out = m_nand.out;
  }

private:
  Not m_notA;
  Not m_notB;
  Nand m_nand;
};

class Xor {
public:
  bool a;
  bool b;
  bool out;

  Xor() : a(false), b(false) { computeOutput(); }

  inline void computeOutput() {
    m_notA.in = a;
    m_notA.computeOutput();
    m_notB.in = b;
    m_notB.computeOutput();
    m_nand1.a = a;
    m_nand1.b = m_notB.out;
    m_nand1.computeOutput();
    m_nand2.a = m_notA.out;
    m_nand2.b = b;
    m_nand2.computeOutput();
    m_nandOut.a = m_nand1.out;
    m_nandOut.b = m_nand2.out;
    m_nandOut.computeOutput();
    out = m_nandOut.out;
  }

private:
  Not m_notA;
  Not m_notB;
  Nand m_nand1;
  Nand m_nand2;
  Nand m_nandOut;
};

class Mux {
public:
  bool a;
  bool b;
  bool sel;
  bool out;

  Mux() : a(false), b(false), sel(false) { computeOutput(); }

  inline void computeOutput() {
    m_notSel.in = sel;
    m_notSel.computeOutput();
    m_andA.a = a;
    m_andA.b = m_notSel.out;
    m_andA.computeOutput();
    m_andB.a = b;
    m_andB.b = sel;
    m_andB.computeOutput();
    m_orOut.a = m_andA.out;
    m_orOut.b = m_andB.out;
    m_orOut.computeOutput();
    out = m_orOut.out;
  }

private:
  Not m_notSel;
  And m_andA;
  And m_andB;
  Or m_orOut;
};

class DMux {
public:
  bool in;
  bool sel;
  bool a;
  bool b;

  DMux() : in(false), b(false) { computeOutput(); }

  inline void computeOutput() {
    m_notSel.in = sel;
    m_notSel.computeOutput();
    m_andA.a = in;
    m_andA.b = m_notSel.out;
    m_andA.computeOutput();
    m_andB.a = in;
    m_andB.b = sel;
    m_andB.computeOutput();
    a = m_andA.out;
    b = m_andB.out;
  }

private:
  Not m_notSel;
  And m_andA;
  And m_andB;
};

class Not16 {
public:
  bool in[16];
  bool out[16];

  Not16() : in{0}, out{0} { computeOutput(); }

  inline void computeOutput() {
    m_nots[0].in = in[0];
    m_nots[1].in = in[1];
    m_nots[2].in = in[2];
    m_nots[3].in = in[3];
    m_nots[4].in = in[4];
    m_nots[5].in = in[5];
    m_nots[6].in = in[6];
    m_nots[7].in = in[7];
    m_nots[8].in = in[8];
    m_nots[9].in = in[9];
    m_nots[10].in = in[10];
    m_nots[11].in = in[11];
    m_nots[12].in = in[12];
    m_nots[13].in = in[13];
    m_nots[14].in = in[14];
    m_nots[15].in = in[15];

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

    out[0] = m_nots[0].out;
    out[1] = m_nots[1].out;
    out[2] = m_nots[2].out;
    out[3] = m_nots[3].out;
    out[4] = m_nots[4].out;
    out[5] = m_nots[5].out;
    out[6] = m_nots[6].out;
    out[7] = m_nots[7].out;
    out[8] = m_nots[8].out;
    out[9] = m_nots[9].out;
    out[10] = m_nots[10].out;
    out[11] = m_nots[11].out;
    out[12] = m_nots[12].out;
    out[13] = m_nots[13].out;
    out[14] = m_nots[14].out;
    out[15] = m_nots[15].out;
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
    m_ands[0].a = a[0];
    m_ands[1].a = a[1];
    m_ands[2].a = a[2];
    m_ands[3].a = a[3];
    m_ands[4].a = a[4];
    m_ands[5].a = a[5];
    m_ands[6].a = a[6];
    m_ands[7].a = a[7];
    m_ands[8].a = a[8];
    m_ands[9].a = a[9];
    m_ands[10].a = a[10];
    m_ands[11].a = a[11];
    m_ands[12].a = a[12];
    m_ands[13].a = a[13];
    m_ands[14].a = a[14];
    m_ands[15].a = a[15];

    m_ands[0].b = b[0];
    m_ands[1].b = b[1];
    m_ands[2].b = b[2];
    m_ands[3].b = b[3];
    m_ands[4].b = b[4];
    m_ands[5].b = b[5];
    m_ands[6].b = b[6];
    m_ands[7].b = b[7];
    m_ands[8].b = b[8];
    m_ands[9].b = b[9];
    m_ands[10].b = b[10];
    m_ands[11].b = b[11];
    m_ands[12].b = b[12];
    m_ands[13].b = b[13];
    m_ands[14].b = b[14];
    m_ands[15].b = b[15];

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

    out[0] = m_ands[0].out;
    out[1] = m_ands[1].out;
    out[2] = m_ands[2].out;
    out[3] = m_ands[3].out;
    out[4] = m_ands[4].out;
    out[5] = m_ands[5].out;
    out[6] = m_ands[6].out;
    out[7] = m_ands[7].out;
    out[8] = m_ands[8].out;
    out[9] = m_ands[9].out;
    out[10] = m_ands[10].out;
    out[11] = m_ands[11].out;
    out[12] = m_ands[12].out;
    out[13] = m_ands[13].out;
    out[14] = m_ands[14].out;
    out[15] = m_ands[15].out;
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
    m_ors[0].a = a[0];
    m_ors[1].a = a[1];
    m_ors[2].a = a[2];
    m_ors[3].a = a[3];
    m_ors[4].a = a[4];
    m_ors[5].a = a[5];
    m_ors[6].a = a[6];
    m_ors[7].a = a[7];
    m_ors[8].a = a[8];
    m_ors[9].a = a[9];
    m_ors[10].a = a[10];
    m_ors[11].a = a[11];
    m_ors[12].a = a[12];
    m_ors[13].a = a[13];
    m_ors[14].a = a[14];
    m_ors[15].a = a[15];

    m_ors[0].b = b[0];
    m_ors[1].b = b[1];
    m_ors[2].b = b[2];
    m_ors[3].b = b[3];
    m_ors[4].b = b[4];
    m_ors[5].b = b[5];
    m_ors[6].b = b[6];
    m_ors[7].b = b[7];
    m_ors[8].b = b[8];
    m_ors[9].b = b[9];
    m_ors[10].b = b[10];
    m_ors[11].b = b[11];
    m_ors[12].b = b[12];
    m_ors[13].b = b[13];
    m_ors[14].b = b[14];
    m_ors[15].b = b[15];

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

    out[0] = m_ors[0].out;
    out[1] = m_ors[1].out;
    out[2] = m_ors[2].out;
    out[3] = m_ors[3].out;
    out[4] = m_ors[4].out;
    out[5] = m_ors[5].out;
    out[6] = m_ors[6].out;
    out[7] = m_ors[7].out;
    out[8] = m_ors[8].out;
    out[9] = m_ors[9].out;
    out[10] = m_ors[10].out;
    out[11] = m_ors[11].out;
    out[12] = m_ors[12].out;
    out[13] = m_ors[13].out;
    out[14] = m_ors[14].out;
    out[15] = m_ors[15].out;
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
    m_muxs[0].a = a[0];
    m_muxs[1].a = a[1];
    m_muxs[2].a = a[2];
    m_muxs[3].a = a[3];
    m_muxs[4].a = a[4];
    m_muxs[5].a = a[5];
    m_muxs[6].a = a[6];
    m_muxs[7].a = a[7];
    m_muxs[8].a = a[8];
    m_muxs[9].a = a[9];
    m_muxs[10].a = a[10];
    m_muxs[11].a = a[11];
    m_muxs[12].a = a[12];
    m_muxs[13].a = a[13];
    m_muxs[14].a = a[14];
    m_muxs[15].a = a[15];

    m_muxs[0].b = b[0];
    m_muxs[1].b = b[1];
    m_muxs[2].b = b[2];
    m_muxs[3].b = b[3];
    m_muxs[4].b = b[4];
    m_muxs[5].b = b[5];
    m_muxs[6].b = b[6];
    m_muxs[7].b = b[7];
    m_muxs[8].b = b[8];
    m_muxs[9].b = b[9];
    m_muxs[10].b = b[10];
    m_muxs[11].b = b[11];
    m_muxs[12].b = b[12];
    m_muxs[13].b = b[13];
    m_muxs[14].b = b[14];
    m_muxs[15].b = b[15];

    m_muxs[0].sel =  sel;
    m_muxs[1].sel =  sel;
    m_muxs[2].sel =  sel;
    m_muxs[3].sel =  sel;
    m_muxs[4].sel =  sel;
    m_muxs[5].sel =  sel;
    m_muxs[6].sel =  sel;
    m_muxs[7].sel =  sel;
    m_muxs[8].sel =  sel;
    m_muxs[9].sel =  sel;
    m_muxs[10].sel = sel;
    m_muxs[11].sel = sel;
    m_muxs[12].sel = sel;
    m_muxs[13].sel = sel;
    m_muxs[14].sel = sel;
    m_muxs[15].sel = sel;

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

    out[0] = m_muxs[0].out;
    out[1] = m_muxs[1].out;
    out[2] = m_muxs[2].out;
    out[3] = m_muxs[3].out;
    out[4] = m_muxs[4].out;
    out[5] = m_muxs[5].out;
    out[6] = m_muxs[6].out;
    out[7] = m_muxs[7].out;
    out[8] = m_muxs[8].out;
    out[9] = m_muxs[9].out;
    out[10] = m_muxs[10].out;
    out[11] = m_muxs[11].out;
    out[12] = m_muxs[12].out;
    out[13] = m_muxs[13].out;
    out[14] = m_muxs[14].out;
    out[15] = m_muxs[15].out;
  }

private:
  Mux m_muxs[16];
};