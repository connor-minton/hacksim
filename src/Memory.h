#pragma once

#include <cstdint>
#include "Bits.h"

#include "RAM16K.h"
#include "Mux16.h"
#include "DMux.h"
#include "ShallowChips.h"

class Memory {
public:
  static const uint16_t SCREEN = 0x4000;
  static const uint16_t KBD = 0x6000;

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
  inline uint16_t peek(uint16_t offset) {
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

