#pragma once

#include <vector>
#include <mutex>
#include <chrono>

#include <Windows.h>

#include "Thread.h"

class BitmapManager;
class KeyboardManager;
class IComputer;

/**
 * SimulatorThread
 *
 * Thread that runs the Hack computer loop.
 */
class SimulatorThread : public Thread<SimulatorThread> {
public:
  /**
   * rom
   *   The Hack program chosen by the user.
   *
   * hwnd
   *   The handle of the main window.
   */
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

  HWND m_hwnd;
  std::vector<uint16_t> m_rom;
  KeyboardManager & m_km;
  IComputer* m_computer = nullptr;
  uint16_t* m_screenMem = nullptr;
  uint16_t m_kbd = 0;
  uint16_t m_clk = 0;

  // Records the timer value when it is set to be a nonzero value
  // by the simulated program. This value is updated by the simulator
  // until it becomes zero, at which point m_clkStart is reset to zero.
  uint16_t m_clkStart = 0;

  // The time at which the timer was set to nonzero by the simulated program.
  TimePoint m_clkStartTime;

  // Is the simulator decreasing m_clk?
  bool m_clkRunning = false;

  // Screen rendering

  // minimum time between sending WM_PAINT messages to main window
  const Clock::duration m_frameDuration =
    std::chrono::microseconds(16667); // average of about 60 FPS
  BitmapManager & m_bm;

  // time of last render
  TimePoint m_lastRender;

  /**
   * Triggers a render via the BitmapManager, but only if m_frameDuration time
   * has passed since the last render. Can be safely called every simulation iteration.
   */
  void outputScreen();

  /**
   * Updates the timer register (m_clk) based on whether the timer is running and
   * how long has elapsed since it started running. Handles all of the logic for
   * managing the state of the timer. Should be called every simulation iteration.
   */
  void updateClock();
};

