#pragma once

#include <vector>
#include <mutex>
#include <chrono>

#include <Windows.h>

#include "Thread.h"

class BitmapManager;
class KeyboardManager;
class Computer;

class SimulatorThread : public Thread<SimulatorThread> {
public:
  SimulatorThread(std::vector<uint16_t> rom, 
                  BitmapManager & bm, 
                  KeyboardManager & km, 
                  HWND hwnd) 
    : Thread<SimulatorThread>(), 
      m_rom(rom), 
      m_bm(bm), 
      m_km(km), 
      m_hwnd(hwnd)
  { }

  ~SimulatorThread() {
    delete m_computer;
    delete[] m_screenMem;
  }

  static DWORD WINAPI Run(void* data);

private:
  using Clock = std::chrono::steady_clock;
  using TimePoint = std::chrono::time_point<Clock>;

  void outputScreen();

  Computer* m_computer = nullptr;
  uint16_t* m_screenMem = nullptr;
  uint16_t m_kbd = 0;
  std::vector<uint16_t> m_rom;
  KeyboardManager & m_km;
  HWND m_hwnd;

  // Screen rendering
  const Clock::duration m_frameDuration =
    std::chrono::microseconds(16667); // average of about 60 FPS
  BitmapManager & m_bm;
  TimePoint m_lastRender;
};

