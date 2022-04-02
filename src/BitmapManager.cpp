#include "BitmapManager.h"

void BitmapManager::UpdateBitmap() {
  // pixels in B8-G8-R8-A8 format (little-endian)
  // alpha is ignored
  const uint32_t WHITE = 0x00ffffff, BLACK = 0;

  // for each 16-bit Hack word in the screen memory map...
  for (int i = 0; i < 8192; i++) {
    uint16_t hackWord = m_screenMem[i];

    // each Hack word in the screen memory map represents 16 pixels.
    // 0 => white
    // 1 => black

    // for each bit in the Hack word...
    for (int j = 0; j < 16; j++) {
      // set the corresponding pixel in the bitmap buffer
      m_destMem[i*16 + j] = (hackWord & 1) ? BLACK : WHITE;
      hackWord = hackWord >> 1;
    }
  }
}

void BitmapManager::TriggerRender(HWND hwnd) {
  std::lock_guard scopeLock(m_mtx);
  m_ready = true;
  InvalidateRect(hwnd, NULL, TRUE);
}

void BitmapManager::TriggerRenderHandled() {
  m_ready = false;
}