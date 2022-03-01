#pragma once
class BitmapManager {
public:
  bool drawResults = false;

  void SetScreenMem(uint16_t* screenMem) { m_screenMem = screenMem; }

  void FillBitmap(uint32_t* destMem) {
    const uint32_t WHITE = 0x00ffffff, BLACK = 0;
    for (int i = 0; i < 8192; i++) {
      uint16_t hackWord = m_screenMem[i];
      for (int j = 0; j < 16; j++) {
        destMem[i*16 + j] = (hackWord & 1) ? BLACK : WHITE;
        hackWord = hackWord >> 1;
      }
    }
  }

private:
  uint16_t* m_screenMem = nullptr;
};

