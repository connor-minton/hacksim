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