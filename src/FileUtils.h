#pragma once

#include <vector>
#include <string>

/**
 * FileUtils
 *
 * A collection of static utility functions for working with *.hack files.
 */
class FileUtils {
public:
  /**
   * Reads a *.hack file and returns its contents in binary form appropriate
   * for ROM32K. Throws an exception of type Error if there is a problem reading
   * the file or if the file is not formatted correctly.
   */
  static std::vector<uint16_t> readHackFile(const std::string& filename);

  /**
   * Reads a *.hack file and returns its contents in binary form appropriate
   * for ROM32K. Throws an exception of type Error if there is a problem reading
   * the file or if the file is not formatted correctly.
   */
  static std::vector<uint16_t> readHackFile(const std::wstring& filename);

  /**
   * Converts an instruction in binary form to its mnemonic assembly language form.
   * Useful for debugging.
   */
  static std::string disassemble(uint16_t instr);

private:
  /**
   * Disassembles the destination part of a Hack C-instruction.
   */
  static std::string disassembleDest(uint16_t instr);

  /**
   * Disassembles the computation part of a Hack C-instruction.
   */
  static std::string disassembleComp(uint16_t instr);

  /**
   * Disassembles the jump part of a Hack C-instruction.
   */
  static std::string disassembleJump(uint16_t instr);
};
