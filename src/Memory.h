#pragma once

#include <cstdint>
#include "Bits.h"

#include "ISequentialCircuit.h"
#include "RAM16K.h"
#include "Mux16.h"
#include "DMux.h"
#include "ShallowChips.h"

class IMemory : public ISequentialCircuit {
public:
  static const uint16_t SCREEN = 0x4000;
  static const uint16_t KBD = 0x6000;
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

  virtual uint16_t peek(uint16_t offset) const = 0;
  virtual void poke(uint16_t offset, uint16_t val) = 0;

  virtual ~IMemory() { }
};

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

  Memory(uint16_t* screen, uint16_t* kbd)
    : m_screen(screen), m_kbd(kbd) { }

  // Bypass the clocking system to examine memory at `offset`
  inline uint16_t peek(uint16_t offset) const {
    if (offset < SCREEN) return m_ram.peek(offset);
    if (offset < KBD)    return m_screen.peek(offset - KBD);
    return *m_kbd;
  }

  // Bypass the clocking system to set memory at `offset` to `val`
  inline void poke(uint16_t offset, uint16_t val) {
    if (offset < SCREEN)   m_ram.poke(offset, val);
    else if (offset < KBD) m_screen.poke(offset - KBD, val);
    else                   *m_kbd = val;
  }

  inline void tick() {
    m_demuxRAM.set_in(load());
    m_demuxRAM.set_sel(getBit<14>(address()));
    m_demuxRAM.computeOutput();

    m_demuxScreenKbd.set_in(m_demuxRAM.b());
    m_demuxScreenKbd.set_sel(getBit<13>(address()));
    m_demuxScreenKbd.computeOutput();

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

    m_muxScreenKbdOut.set_sel(getBit<13>(address()));
    m_muxScreenKbdOut.set_a(m_screen.out());
    m_muxScreenKbdOut.set_b(*m_kbd);
    m_muxScreenKbdOut.computeOutput();

    m_muxOut.set_sel(getBit<14>(address()));
    m_muxOut.set_a(m_ram.out());
    m_muxOut.set_b(m_muxScreenKbdOut.out());
    m_muxOut.computeOutput();

    m_out = m_muxOut.out();
  }

  inline void tock() {
    // optimization: tock ram ONLY if it is being accessed
    if (!getBit<14>(address()))
      m_ram.tock();

    // screen is a shallow chip, doesn't need optimization
    m_screen.tock();

    m_muxScreenKbdOut.set_sel(getBit<13>(address()));
    m_muxScreenKbdOut.set_a(m_screen.out());
    m_muxScreenKbdOut.set_b(*m_kbd);
    m_muxScreenKbdOut.computeOutput();

    m_muxOut.set_sel(getBit<14>(address()));
    m_muxOut.set_a(m_ram.out());
    m_muxOut.set_b(m_muxScreenKbdOut.out());
    m_muxOut.computeOutput();

    m_out = m_muxOut.out();
  }

private:
  // { load, address[0..14] }
  uint16_t m_pins = 0;
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  uint16_t* m_kbd;

  RAM16K m_ram;
  shallow::Screen m_screen;
  DMux m_demuxRAM;
  DMux m_demuxScreenKbd;
  Mux16 m_muxScreenKbdOut;
  Mux16 m_muxOut;
};

namespace shallow {

class Memory : public IMemory {
public:
  Memory(uint16_t* screen, uint16_t* kbd, uint16_t* clk)
    : m_screen(screen), m_kbd(kbd), m_clk(clk), m_ram(16384) { }

  uint16_t in() const { return m_in; }
  bool load() const { return m_load; }
  uint16_t address() const { return m_address; }

  void set_in(uint16_t val) { m_in = val; }
  void set_load(bool val) { m_load = val; }
  void set_address(uint16_t val) { m_address = val & 0x7fff; }

  // OUT out[16]
  uint16_t out() const { return m_out; }

  uint16_t peek(uint16_t offset) const {
    if (offset < SCREEN) return m_ram[offset];
    if (offset < KBD)    return m_screen.peek(offset - SCREEN);
    if (offset == KBD)   return *m_kbd;
    return *m_clk;
  }

  void poke(uint16_t offset, uint16_t val) {
    if (offset < SCREEN) m_ram[offset] = val;
    if (offset < KBD)    m_screen.poke(offset - SCREEN, val);
    if (offset == KBD)   *m_kbd = val;
    *m_clk = val;
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
  uint16_t m_in = 0;
  uint16_t m_out = 0;
  uint16_t m_address = 0;
  bool m_load = false;

  std::vector<uint16_t> m_ram;
  shallow::Screen m_screen;
  uint16_t* m_kbd;
  uint16_t* m_clk;
};

}
