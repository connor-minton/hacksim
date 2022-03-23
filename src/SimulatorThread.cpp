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
  td->m_computer = new Computer(td->m_screenMem, &td->m_kbd, &td->m_clk);
  td->m_computer->set_rom(td->m_rom);
  td->m_bm.SetScreenMem(td->m_screenMem);
  while (true) {
    td->m_kbd = td->m_km.GetScanCode();
    td->updateClock();
    td->m_computer->tick();
    td->m_computer->tock();
    td->outputScreen();
  }

  td->m_result = 0;
  return 0;
}

void SimulatorThread::outputScreen() {
  Clock::duration durationSinceLastRender =
    Clock::now() - m_lastRender;

  if (durationSinceLastRender >= m_frameDuration) {
    m_bm.TriggerRender(m_hwnd);
    m_lastRender = Clock::now();
  }
}

void SimulatorThread::updateClock() {
  using std::chrono::duration_cast;
  using std::chrono::milliseconds;

  if (m_clk == 0) return;

  if (!m_clkRunning) {
    m_clkRunning = true;
    m_clkStart = m_clk;
    m_clkStartTime = Clock::now();
  }
  else {
    Clock::duration durSinceClockStart = Clock::now() - m_clkStartTime;
    uint32_t msSinceClockStart = duration_cast<milliseconds>(durSinceClockStart).count();
    if (msSinceClockStart >= m_clkStart) {
      m_clkRunning = false;
      m_clk = 0;
    }
    else {
      m_clk = m_clkStart - msSinceClockStart;
    }
  }
}
