#pragma once
#include <mutex>

class BitmapManager {
public:
  bool drawResults = false;

  std::mutex & mtx() { return m_mtx; }

  void SetScreenMem(uint16_t* screenMem) { m_screenMem = screenMem; }
  void SetDestMem(uint32_t* destMem) { m_destMem = destMem; }

  void UpdateBitmap() {
    const uint32_t WHITE = 0x00ffffff, BLACK = 0;
    for (int i = 0; i < 8192; i++) {
      uint16_t hackWord = m_screenMem[i];
      for (int j = 0; j < 16; j++) {
        m_destMem[i*16 + j] = (hackWord & 1) ? BLACK : WHITE;
        hackWord = hackWord >> 1;
      }
    }
  }

private:
  std::mutex m_mtx;
  uint16_t* m_screenMem = nullptr;
  uint32_t* m_destMem = nullptr;
};

