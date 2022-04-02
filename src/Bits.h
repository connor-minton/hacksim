#pragma once

#include <cstdint>

/**
 * Sets bit N of bits to 1 if value is true and 0 if value is false.
 * Returns true if bit N has been changed as a result and false otherwise.
 *
 * Type T should be an integral type.
 */
template<uint8_t N, typename T>
inline bool setBit(T& bits, bool value);

/**
 * Returns true if bit N of bits is 1 and false if it is 0.
 *
 * Type T should be an integral type.
 */
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
