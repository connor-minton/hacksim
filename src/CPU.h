#pragma once

#include <cstdint>
#include "Bits.h"

#include "ISequentialCircuit.h"
#include "Register.h"
#include "PC.h"
#include "Not.h"
#include "And.h"
#include "Or.h"
#include "ALU.h"
#include "Mux16.h"

class CPU : public ISequentialCircuit {
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

