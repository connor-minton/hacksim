#include "SimulatorThread.h"

#include <memory>

#include "AppConfig.h"
#include "Chips.h"
#include "BitmapManager.h"
#include "KeyboardManager.h"

DWORD SimulatorThread::Run() {
  // Initialize screen memory
  m_screenMem = new uint16_t[shallow::Screen::SCREEN_SIZE];
  for (int i = 0; i < shallow::Screen::SCREEN_SIZE; i++) {
    m_screenMem[i] = 0;
  }

  std::unique_ptr<IMemory> memory;
  std::unique_ptr<ICPU> cpu_;

  if (AppConfig::shallowMemory()) {
    // optimized simulation
    memory = std::make_unique<shallow::Memory>(m_screenMem, &m_kbd, &m_clk);
  }
  else {
    // deep simulation
    memory = std::make_unique<Memory>(m_screenMem, &m_kbd, &m_clk);
  }

  if (AppConfig::shallowCPU()) {
    // optimized simulation
    cpu_ = std::make_unique<shallow::CPU>();
  }
  else {
    // deep simulation
    cpu_ = std::make_unique<CPU>();
  }

  // initialize computer
  m_computer = new BaseComputer(memory.get(), cpu_.get());
  m_computer->set_rom(m_rom);

  // initialize BitmapManager
  m_bm.SetScreenMem(m_screenMem);

  // MAIN SIMULATION LOOP
  while (true) {
    m_kbd = m_km.GetScanCode();
    updateClock();
    m_computer->tick();
    m_computer->tock();
    outputScreen();
  }

  m_result = 0;
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

  // If timer is nonzero, we need to calculate its new value

  if (!m_clkRunning) {
    // The Hack program has set the timer to a nonzero value in the previous iteration.
    // Start the timer.
    m_clkRunning = true;
    m_clkStart = m_clk;
    m_clkStartTime = Clock::now();
  }
  else {
    // The Hack program is likely waiting on the timer to read zero.
    // Adjust the timer.
    // We can't simply decrement the timer by the number of milliseconds that have passed,
    // because that number might round down to zero if the performance is high enough.
    // This strategy calculates the number to subtract as the difference between now and
    // when the timer was started.
    Clock::duration durSinceClockStart = Clock::now() - m_clkStartTime;
    long long msSinceClockStart = duration_cast<milliseconds>(durSinceClockStart).count();
    if (msSinceClockStart >= m_clkStart) {
      // The requested time to wait has elapsed, so reset the timer.
      m_clkRunning = false;
      m_clk = 0;
    }
    else {
      // The timer should not yet equal zero.
      m_clk = m_clkStart - msSinceClockStart;
    }
  }
}
