#include "Computer.h"

uint16_t BaseComputer::curInstruction() const { return m_rom.instruction(); }
uint16_t BaseComputer::nextInstruction() const
  { return m_rom.instructionAt(m_cpu->pc()); }

std::string BaseComputer::curInstrDbg() const
  { return FileUtils::disassemble(curInstruction()); }

std::string BaseComputer::nextInstrDbg() const
  { return FileUtils::disassemble(nextInstruction()); }

uint16_t BaseComputer::curPC() const { return m_rom.address(); }
uint16_t BaseComputer::nextPC() const { return m_cpu->pc(); }
