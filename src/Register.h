#pragma once

#include <cstdint>
#include "Bits.h"

#include "ISequentialCircuit.h"
#include "Bit.h"

class IRegister : public ISequentialCircuit {
public:
  // INPUT in[16], load
  virtual uint16_t in() const = 0;
  virtual bool load() const = 0;

  virtual void set_in(uint16_t val) = 0;
  virtual void set_load(bool val) = 0;

  virtual void poke(uint16_t val) = 0;

  // OUTPUT out[16]
  virtual uint16_t out() const = 0;

  virtual ~IRegister() { }
};

class Register : public IRegister {
public:
  // INPUT in[16], load
  inline uint16_t in() const { return m_in; }
  inline bool load() const { return getBit<0>(m_pins); }

  inline void set_in(uint16_t val) { m_in = val; }
  inline void set_load(bool val) { setBit<0>(m_pins, val); }

  // OUTPUT out[16]
  inline uint16_t out() const { return m_out; }

  // Bypass the clocking system to forcefully set the underlying value
  // to `val`
  void poke(uint16_t val) {
    m_bits[0].poke(getBit<0>(val));
    m_bits[1].poke(getBit<1>(val));
    m_bits[2].poke(getBit<2>(val));
    m_bits[3].poke(getBit<3>(val));
    m_bits[4].poke(getBit<4>(val));
    m_bits[5].poke(getBit<5>(val));
    m_bits[6].poke(getBit<6>(val));
    m_bits[7].poke(getBit<7>(val));
    m_bits[8].poke(getBit<8>(val));
    m_bits[9].poke(getBit<9>(val));
    m_bits[10].poke(getBit<10>(val));
    m_bits[11].poke(getBit<11>(val));
    m_bits[12].poke(getBit<12>(val));
    m_bits[13].poke(getBit<13>(val));
    m_bits[14].poke(getBit<14>(val));
    m_bits[15].poke(getBit<15>(val));

    setBit<0>(m_out, m_bits[0].out());
    setBit<1>(m_out, m_bits[1].out());
    setBit<2>(m_out, m_bits[2].out());
    setBit<3>(m_out, m_bits[3].out());
    setBit<4>(m_out, m_bits[4].out());
    setBit<5>(m_out, m_bits[5].out());
    setBit<6>(m_out, m_bits[6].out());
    setBit<7>(m_out, m_bits[7].out());
    setBit<8>(m_out, m_bits[8].out());
    setBit<9>(m_out, m_bits[9].out());
    setBit<10>(m_out, m_bits[10].out());
    setBit<11>(m_out, m_bits[11].out());
    setBit<12>(m_out, m_bits[12].out());
    setBit<13>(m_out, m_bits[13].out());
    setBit<14>(m_out, m_bits[14].out());
    setBit<15>(m_out, m_bits[15].out());
  }

  Register() { tock(); }

  void tick() {
    m_bits[0].set_in(getBit<0>(m_in));
    m_bits[1].set_in(getBit<1>(m_in));
    m_bits[2].set_in(getBit<2>(m_in));
    m_bits[3].set_in(getBit<3>(m_in));
    m_bits[4].set_in(getBit<4>(m_in));
    m_bits[5].set_in(getBit<5>(m_in));
    m_bits[6].set_in(getBit<6>(m_in));
    m_bits[7].set_in(getBit<7>(m_in));
    m_bits[8].set_in(getBit<8>(m_in));
    m_bits[9].set_in(getBit<9>(m_in));
    m_bits[10].set_in(getBit<10>(m_in));
    m_bits[11].set_in(getBit<11>(m_in));
    m_bits[12].set_in(getBit<12>(m_in));
    m_bits[13].set_in(getBit<13>(m_in));
    m_bits[14].set_in(getBit<14>(m_in));
    m_bits[15].set_in(getBit<15>(m_in));

    m_bits[0].set_load(load());
    m_bits[1].set_load(load());
    m_bits[2].set_load(load());
    m_bits[3].set_load(load());
    m_bits[4].set_load(load());
    m_bits[5].set_load(load());
    m_bits[6].set_load(load());
    m_bits[7].set_load(load());
    m_bits[8].set_load(load());
    m_bits[9].set_load(load());
    m_bits[10].set_load(load());
    m_bits[11].set_load(load());
    m_bits[12].set_load(load());
    m_bits[13].set_load(load());
    m_bits[14].set_load(load());
    m_bits[15].set_load(load());

    m_bits[0].tick();
    m_bits[1].tick();
    m_bits[2].tick();
    m_bits[3].tick();
    m_bits[4].tick();
    m_bits[5].tick();
    m_bits[6].tick();
    m_bits[7].tick();
    m_bits[8].tick();
    m_bits[9].tick();
    m_bits[10].tick();
    m_bits[11].tick();
    m_bits[12].tick();
    m_bits[13].tick();
    m_bits[14].tick();
    m_bits[15].tick();
  }

  void tock() {
    m_bits[0].tock();
    m_bits[1].tock();
    m_bits[2].tock();
    m_bits[3].tock();
    m_bits[4].tock();
    m_bits[5].tock();
    m_bits[6].tock();
    m_bits[7].tock();
    m_bits[8].tock();
    m_bits[9].tock();
    m_bits[10].tock();
    m_bits[11].tock();
    m_bits[12].tock();
    m_bits[13].tock();
    m_bits[14].tock();
    m_bits[15].tock();

    setBit<0>(m_out, m_bits[0].out());
    setBit<1>(m_out, m_bits[1].out());
    setBit<2>(m_out, m_bits[2].out());
    setBit<3>(m_out, m_bits[3].out());
    setBit<4>(m_out, m_bits[4].out());
    setBit<5>(m_out, m_bits[5].out());
    setBit<6>(m_out, m_bits[6].out());
    setBit<7>(m_out, m_bits[7].out());
    setBit<8>(m_out, m_bits[8].out());
    setBit<9>(m_out, m_bits[9].out());
    setBit<10>(m_out, m_bits[10].out());
    setBit<11>(m_out, m_bits[11].out());
    setBit<12>(m_out, m_bits[12].out());
    setBit<13>(m_out, m_bits[13].out());
    setBit<14>(m_out, m_bits[14].out());
    setBit<15>(m_out, m_bits[15].out());
  }

private:
  // { load }
  uint8_t m_pins = 0;
  uint16_t m_in = 0;
  uint16_t m_out = 0;

  Bit m_bits[16];
};

namespace shallow {

class Register : public IRegister {
public:
  // INPUT in[16], load
  inline uint16_t in() const { return m_in; }
  inline bool load() const { return m_load; }

  inline void set_in(uint16_t val) { m_in = val; }
  inline void set_load(bool val) { m_load = val; }

  inline void poke(uint16_t val) { m_out = val; }

  // OUTPUT out[16]
  inline uint16_t out() const { return m_out; }

  inline void tick() { }

  inline void tock() {
    if (m_load) {
      m_out = m_in;
    }
  }

private:
  uint16_t m_in = 0;
  uint16_t m_out = 0;
  bool m_load = false;
};

}

