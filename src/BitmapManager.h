#pragma once
#include <mutex>

#include <Windows.h>

class BitmapManager {
public:
  std::mutex & mtx() { return m_mtx; }
  bool Ready() { return m_ready; }

  void SetScreenMem(uint16_t* screenMem) { m_screenMem = screenMem; }
  void SetDestMem(uint32_t* destMem) { m_destMem = destMem; }

  void TriggerRender(HWND hwnd);
  void TriggerRenderHandled();
  void UpdateBitmap();

private:
  bool m_ready = false;

  std::mutex m_mtx;
  uint16_t* m_screenMem = nullptr;
  uint32_t* m_destMem = nullptr;
};

