#pragma once

#include <cstdint>

// Returns true if there was a change to `bits` and false if no change
template<uint8_t N, typename T>
inline bool setBit(T& bits, bool value);

template<uint8_t N, typename T>
inline bool getBit(T bits);

// ===============================================

template<uint8_t N, typename T>
inline bool setBit(T& bits, bool value) {
  constexpr uint64_t mask = (1UL << N);
  T old = bits;
  if (value) bits |= mask;
  else       bits &= ~mask;
  return old != bits;
}

template<uint8_t N, typename T>
inline bool getBit(T bits) {
  constexpr uint64_t mask = (1UL << N);
  return (bits & mask) != 0;
}
