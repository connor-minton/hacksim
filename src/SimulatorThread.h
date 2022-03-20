#pragma once

#include <stdexcept>
#include <string>
#include <vector>
#include <mutex>

#include <Windows.h>

#include "Thread.h"
#include "Chips.h"
#include "ShallowChips.h"
#include "BitmapManager.h"
#include "KeyboardManager.h"

class SimulatorThread : public Thread<SimulatorThread> {
public:
  SimulatorThread(std::vector<uint16_t> rom, BitmapManager & bm, KeyboardManager & km, HWND hwnd) 
    : Thread<SimulatorThread>(), m_rom(rom), m_bm(bm), m_km(km), m_hwnd(hwnd)
  { }

  ~SimulatorThread() {
    delete m_computer;
    delete[] m_screenMem;
  }

  static DWORD WINAPI Run(void* data);

private:
  Computer* m_computer = nullptr;
  uint16_t* m_screenMem = nullptr;
  uint16_t m_kbd = 0;
  std::vector<uint16_t> m_rom;
  BitmapManager & m_bm;
  KeyboardManager & m_km;
  HWND m_hwnd;
};

DWORD WINAPI SimulatorThread::Run(void* data) {
  SimulatorThread* td = (SimulatorThread*)data;
  td->m_screenMem = new uint16_t[shallow::Screen::SCREEN_SIZE];
  for (int i = 0; i < shallow::Screen::SCREEN_SIZE; i++) {
    td->m_screenMem[i] = 0;
  }
  td->m_computer = new Computer(td->m_screenMem, &td->m_kbd);
  td->m_computer->set_rom(td->m_rom);
  td->m_bm.SetScreenMem(td->m_screenMem);
  uint32_t renderCycles = 0;
  while (td->m_computer->nextPC() != 31643) {
    td->m_kbd = td->m_km.GetScanCode();
    td->m_computer->tock();
    if (renderCycles >= 420) { // idk
      std::lock_guard lck(td->m_bm.mtx());
      td->m_bm.drawResults = true;
      InvalidateRect(td->m_hwnd, NULL, TRUE);
      renderCycles = 0;
    }
    else {
      renderCycles++;
    }
  }
  td->m_result = 0;
  return 0;
}

