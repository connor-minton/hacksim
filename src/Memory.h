#pragma once

#include <cstdint>
#include "Bits.h"

#include "ISequentialCircuit.h"
#include "RAM16K.h"
#include "Mux16.h"
#include "DMux.h"
#include "ShallowChips.h"

/**
 * IMemory
 *
 * IN  in[16], load, address[15]
 * OUT out[16]
 *
 * The complete address space of the Hack computer's memory,
 * including RAM and memory-mapped I/O.
 * The chip facilitates read and write operations, as follows:
 *     Read:  out(t) = Memory[address(t)](t)
 *     Write: if load(t-1) then Memory[address(t-1)](t) = in(t-1)
 * In words: the chip always outputs the value stored at the memory
 * location specified by address. If load==1, the in value is loaded
 * into the memory location specified by address. This value becomes
 * available through the out output from the next time step onward.
 * Address space rules:
 * Only the upper 16K+8K+2 words of the Memory chip are used.
 * Access to address>0x6001 is invalid. Access to any address in
 * the range 0x4000-0x5FFF results in accessing the screen memory
 * map. Access to address 0x6000 results in accessing the keyboard
 * memory map. Access to the address 0x6001 results in accessing the timer
 * memory map.
 *
 * The HackSim project extends the Hack memory map by adding a timer
 * at address 0x6001 (IMemory::CLK). This register starts at zero.
 * If a Hack instruction modifies this register to a nonzero value,
 * then the simulation environment interprets this value as the number
 * of milliseconds until the register should read zero. Each clock cycle,
 * the simulation environment keeps the value of the register up to date
 * until the register reads zero. A Hack program can use this register to
 * implement a wait() system call.
 */

class IMemory : public ISequentialCircuit {
public:
  /**
   * The base address of the screen memory map, which occupies the range
   * 0x4000 - 0x5fff.
   */
  static const uint16_t SCREEN = 0x4000;

  /**
   * The base address of the keyboard memory map.
   */
  static const uint16_t KBD = 0x6000;

  /**
   * The base address of the timer memory map.
   */
  static const uint16_t CLK = 0x6001;

  // IN in[16], load, address[15]
  virtual uint16_t in() const = 0;
  virtual bool load() const = 0;
  virtual uint16_t address() const = 0;

  virtual void set_in(uint16_t val) = 0;
  virtual void set_load(bool val) = 0;
  virtual void set_address(uint16_t val) = 0;

  // OUT out[16]
  virtual uint16_t out() const = 0;

  /**
   * Bypass the clocking system to observe the value of memory at `offset`.
   */
  virtual uint16_t peek(uint16_t offset) const = 0;

  /**
   * Bypass the clocking system to set the value of memory at `offset` to `val`.
   */
  virtual void poke(uint16_t offset, uint16_t val) = 0;

  virtual ~IMemory() { }
};

/**
 * Memory
 *
 * Deeply simulated memory. See IMemory for interface documentation.
 */
class Memory : public IMemory {
public:
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

  /**
   * Create a new Memory that writes to screen, kbd, and clk.
   *
   * screen
   *   An 8192-element array of uint16_t.
   *
   * kbd
   *   A pointer to a single uint16_t. Holds the scan code of the keyboard.
   *
   * clk
   *   A pointer to a single uint16_t. Holds the value of the timer.
   */
  Memory(uint16_t* screen, uint16_t* kbd, uint16_t* clk)
    : m_screen(screen), m_kbd(kbd), m_clk(clk) { }

  // Bypass the clocking system to examine memory at `offset`
  inline uint16_t peek(uint16_t offset) const {
    if (offset < SCREEN) return m_ram.peek(offset);
    if (offset < KBD)    return m_screen.peek(offset - KBD);
    if (offset == KBD)   return *m_kbd;
    return *m_clk;
  }

  // Bypass the clocking system to set memory at `offset` to `val`
  inline void poke(uint16_t offset, uint16_t val) {
    if (offset < SCREEN)    m_ram.poke(offset, val);
    else if (offset < KBD)  m_screen.poke(offset - KBD, val);
    else if (offset == KBD) *m_kbd = val;
    else                    *m_clk = val;
  }

  void tick() {
    m_demuxRAM.set_in(load());
    m_demuxRAM.set_sel(getBit<14>(address()));
    m_demuxRAM.computeOutput();

    m_demuxScreenKbd.set_in(m_demuxRAM.b());
    m_demuxScreenKbd.set_sel(getBit<13>(address()));
    m_demuxScreenKbd.computeOutput();

    m_demuxKbdClk.set_in(m_demuxScreenKbd.b());
    m_demuxKbdClk.set_sel(getBit<0>(address()));
    m_demuxKbdClk.computeOutput();

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

    // same with hardware timer
    tickClock();

    m_muxKbdClkOut.set_sel(getBit<0>(address()));
    m_muxKbdClkOut.set_a(*m_kbd);
    m_muxKbdClkOut.set_b(*m_clk);
    m_muxKbdClkOut.computeOutput();

    m_muxScreenKbdOut.set_sel(getBit<13>(address()));
    m_muxScreenKbdOut.set_a(m_screen.out());
    m_muxScreenKbdOut.set_b(m_muxKbdClkOut.out());
    m_muxScreenKbdOut.computeOutput();

    m_muxOut.set_sel(getBit<14>(address()));
    m_muxOut.set_a(m_ram.out());
    m_muxOut.set_b(m_muxScreenKbdOut.out());
    m_muxOut.computeOutput();

    m_out = m_muxOut.out();
  }

  void tock() {
    // optimization: tock ram ONLY if it is being accessed
    if (!getBit<14>(address()))
      m_ram.tock();

    // screen is a shallow chip, doesn't need optimization
    m_screen.tock();

    // same for hardware timer
    tockClock();

    m_muxKbdClkOut.set_sel(getBit<0>(address()));
    m_muxKbdClkOut.set_a(*m_kbd);
    m_muxKbdClkOut.set_b(*m_clk);
    m_muxKbdClkOut.computeOutput();

    m_muxScreenKbdOut.set_sel(getBit<13>(address()));
    m_muxScreenKbdOut.set_a(m_screen.out());
    m_muxScreenKbdOut.set_b(m_muxKbdClkOut.out());
    m_muxScreenKbdOut.computeOutput();

    m_muxOut.set_sel(getBit<14>(address()));
    m_muxOut.set_a(m_ram.out());
    m_muxOut.set_b(m_muxScreenKbdOut.out());
    m_muxOut.computeOutput();

    m_out = m_muxOut.out();
  }

private:
  // pins

  // layout of m_pins
  //   0     1..15
  // { load, address[0..14] }
  uint16_t m_pins = 0;
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  // internal components

  uint16_t* m_kbd;

  uint16_t* m_clk;
  uint16_t m_clkIn = 0;
  bool m_clkLoad = false;

  RAM16K m_ram;
  shallow::Screen m_screen;
  DMux m_demuxRAM;
  DMux m_demuxScreenKbd;
  DMux m_demuxKbdClk;
  Mux16 m_muxKbdClkOut;
  Mux16 m_muxScreenKbdOut;
  Mux16 m_muxOut;

  void tickClock() {
    m_clkIn = in();
    m_clkLoad = m_demuxKbdClk.b();
  }

  void tockClock() {
    if (m_clkLoad) {
      *m_clk = m_clkIn;
    }
  }
};

namespace shallow {

/**
 * shallow::Memory
 *
 * Efficient memory unit. Not deeply simulated. See IMemory for interface documentation.
 */
class Memory : public IMemory {
public:
  /**
   * Create a new shallow::Memory that writes to screen, kbd, and clk.
   *
   * screen
   *   An 8192-element array of uint16_t.
   *
   * kbd
   *   A pointer to a single uint16_t. Holds the scan code of the keyboard.
   *
   * clk
   *   A pointer to a single uint16_t. Holds the value of the timer.
   */
  Memory(uint16_t* screen, uint16_t* kbd, uint16_t* clk)
    : m_screen(screen), m_kbd(kbd), m_clk(clk), m_ram(16384) { }

  inline uint16_t in() const { return m_in; }
  inline bool load() const { return m_load; }
  inline uint16_t address() const { return m_address; }

  inline void set_in(uint16_t val) { m_in = val; }
  inline void set_load(bool val) { m_load = val; }
  inline void set_address(uint16_t val) { m_address = val & 0x7fff; }

  // OUT out[16]
  uint16_t out() const { return m_out; }

  inline uint16_t peek(uint16_t offset) const {
    if (offset < SCREEN) return m_ram[offset];
    if (offset < KBD)    return m_screen.peek(offset - SCREEN);
    if (offset == KBD)   return *m_kbd;
    return *m_clk;
  }

  inline void poke(uint16_t offset, uint16_t val) {
    if (offset < SCREEN) m_ram[offset] = val;
    if (offset < KBD)    m_screen.poke(offset - SCREEN, val);
    if (offset == KBD)   *m_kbd = val;
    else                 *m_clk = val;
  }

  void tick() {
    m_screen.set_in(m_in);
    m_screen.set_address(m_address & 0x1fff);

    if (m_address < SCREEN) {
      m_screen.set_load(false);
      m_screen.tick();
      m_out = m_ram[m_address];
    }
    else if (m_address < KBD) {
      m_screen.set_load(m_load);
      m_screen.tick();
      m_out = m_screen.out();
    }
    else if (m_address == KBD) {
      m_screen.set_load(false);
      m_screen.tick();
      m_out = *m_kbd;
    }
    else {
      m_screen.set_load(false);
      m_screen.tick();
      m_out = *m_clk;
    }
  }

  void tock() {
    m_screen.tock();
    if (m_address < SCREEN) {
      if (m_load)
        m_ram[m_address] = m_in;
      m_out = m_ram[m_address];
    }
    else if (m_address < KBD) {
      m_out = m_screen.out();
    }
    else if (m_address == KBD) {
      m_out = *m_kbd;
    }
    else {
      if (m_load) {
        *m_clk = m_in;
      }
      m_out = *m_clk;
    }
  }

private:
  // pins
  uint16_t m_in = 0;
  uint16_t m_out = 0;
  uint16_t m_address = 0;
  bool m_load = false;

  // internal components
  std::vector<uint16_t> m_ram;
  shallow::Screen m_screen;
  uint16_t* m_kbd;
  uint16_t* m_clk;
};

}
