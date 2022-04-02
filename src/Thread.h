/**
 * Thread.h
 *
 * Simple threading library for Windows.
 */

#pragma once

#include <stdexcept>
#include <string>

#include <Windows.h>

/**
 * Thread
 *
 * Represents a thread along with its entry point function.
 */
template <typename T>
class Thread {
public:
  /**
   * Calls the Windows CreateThread() function with reasonable defaults to
   * run the thread.
   */
  void Create();

  /**
   * Calls the Windows TerminateThread() function. Is NOT called by the destructor.
   */
  void Terminate();

  DWORD GetTID() const { return m_tid; }
  HANDLE GetHandle() const { return m_handle; }
  DWORD GetResult() const { return m_result; }

  /**
   * Convenience for implicit casting a thread to a HANDLE when passing a thread
   * to the Windows API.
   */
  operator HANDLE() const { return m_handle; }

  /**
   * Asks Windows to release the thread's system resources.
   */
  ~Thread();

protected:
  DWORD m_tid = 0;
  HANDLE m_handle = nullptr;
  DWORD m_result = 0;

  static DWORD WINAPI Run(void* data) { return 0; }
};

template <typename T>
void Thread<T>::Create() {
  m_handle = CreateThread(
    NULL,
    0,
    T::Run,
    this,
    0,
    &m_tid
  );

  if (!m_handle) {
    throw std::runtime_error("Thread could not be created");
  }
}

template <typename T>
void Thread<T>::Terminate() {
  if (m_handle) {
    TerminateThread(m_handle, 0);
  }
}

template <typename T>
Thread<T>::~Thread() {
  if (m_handle) {
    CloseHandle(m_handle);
  }
}
