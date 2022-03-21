#pragma once
#include <mutex>

class BitmapManager {
public:
  bool drawResults = false;

  std::mutex & mtx() { return m_mtx; }

  void SetScreenMem(uint16_t* screenMem) { m_screenMem = screenMem; }
  void SetDestMem(uint32_t* destMem) { m_destMem = destMem; }

  void UpdateBitmap();

private:
  std::mutex m_mtx;
  uint16_t* m_screenMem = nullptr;
  uint32_t* m_destMem = nullptr;
};

