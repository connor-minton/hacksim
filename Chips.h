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