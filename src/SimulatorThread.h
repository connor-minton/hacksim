#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include <Windows.h>

#include "Thread.h"
#include "Chips.h"
#include "ShallowChips.h"
#include "BitmapManager.h"

class SimulatorThread : public Thread<SimulatorThread> {
public:
  SimulatorThread(std::vector<uint16_t> rom, BitmapManager & bm, HWND hwnd) 
    : Thread<SimulatorThread>(), m_rom(rom), m_bm(bm), m_hwnd(hwnd)
  { }

  static DWORD WINAPI Run(void* data);

private:
  Computer* m_computer = nullptr;
  uint16_t* m_screenMem = nullptr;
  uint16_t m_kbd = 0;
  std::vector<uint16_t> m_rom;
  BitmapManager & m_bm;
  HWND m_hwnd;
};

DWORD WINAPI SimulatorThread::Run(void* data) {
  SimulatorThread* td = (SimulatorThread*)data;
  //td->m_screenMem = new uint16_t[shallow::Screen::SCREEN_SIZE];
  //td->m_computer = new Computer(td->m_screenMem, &td->m_kbd);
  Sleep(5000);
  td->m_bm.doGradient = true;
  InvalidateRect(td->m_hwnd, NULL, TRUE);
  td->m_result = 0;
  return 0;
}

