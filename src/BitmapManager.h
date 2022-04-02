#pragma once
#include <mutex>

#include <Windows.h>

/**
 * BitmapManager
 *
 * Transforms the screen data from the Hack computer (owned by the simulator thread)
 * into a pixel format recognized by the Direct2D Bitmap (owned by the UI thread).
 * The BitmapManager has a mutex to help synchronize access between the two threads.
 */
class BitmapManager {
public:
  /**
   * Gets the mutex that should be used to synchronize access to
   * the BitmapManager.
   */
  std::mutex & mtx() { return m_mtx; }

  /**
   * If this flag is true, then the UI thread should copy the data from
   * the `destMem` to the ID2D1Bitmap and call TriggerRenderHandled() when
   * finished.
   */
  bool Ready() { return m_ready; }

  /**
   * Sets the buffer that is being used by the Hack computer as screen memory.
   *
   * screenMem
   *   A pointer to the 8192-element array that is simultaneously being used by
   *   the Hack computer as screen memory. When it is time to refresh the bitmap,
   *   the BitmapManager reads this array. The BitmapManager never writes to this
   *   array.
   */
  void SetScreenMem(uint16_t* screenMem) { m_screenMem = screenMem; }

  /**
   * Sets the bitmap buffer that the BitmapManager should write to.
   *
   * destMem
   *   A 128K-element array of uint32_t, each element representing a B8-G8-R8-A8
   *   formatted pixel. The alpha is ignored.
   */
  void SetDestMem(uint32_t* destMem) { m_destMem = destMem; }

  /**
   * A method to be called by the simulator thread when it wants the UI thread to
   * render a new frame. Should be called without obtaining the lock on mtx().
   *
   * Performs two actions atomically:
   *   1. Sets m_ready to `true` so that Ready() will return `true`.
   *   2. Calls InvalidateRect() on the window so that a WM_PAINT message will
   *      be sent to the window with the handle `hwnd`.
   */
  void TriggerRender(HWND hwnd);

  /**
   * A method to be called by the UI thread after it has called UpdateBitmap().
   * The UI thread MUST obtain the lock on mtx() before calling this method.
   *
   * Simply sets m_ready to false.
   */
  void TriggerRenderHandled();

  /**
   * Copies the screen data (screenMem) to the bitmap buffer (destMem) while transforming
   * the data into the B8-G8-R8-A8 pixel format. The UI thread MUST obtain the lock on
   * mtx() and check the value of Ready() before calling this method.
   */
  void UpdateBitmap();

private:
  bool m_ready = false;

  std::mutex m_mtx;
  uint16_t* m_screenMem = nullptr; // screen memory (input)
  uint32_t* m_destMem = nullptr;   // bitmap buffer (output)
};

