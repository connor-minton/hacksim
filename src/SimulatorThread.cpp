#include "SimulatorThread.h"

#include "Chips.h"
#include "BitmapManager.h"
#include "KeyboardManager.h"

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

