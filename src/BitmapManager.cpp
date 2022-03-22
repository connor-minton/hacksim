#include "BitmapManager.h"

void BitmapManager::UpdateBitmap() {
  const uint32_t WHITE = 0x00ffffff, BLACK = 0;
  for (int i = 0; i < 8192; i++) {
    uint16_t hackWord = m_screenMem[i];
    for (int j = 0; j < 16; j++) {
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