#pragma once

#include <stdexcept>
#include <string>

#include <Windows.h>

template <typename T>
class Thread {
public:
  void Create();

  DWORD GetTID() const { return m_tid; }
  HANDLE GetHandle() const { return m_handle; }
  DWORD GetResult() const { return m_result; }

  operator HANDLE() const { return m_handle; }

  ~Thread();

protected:
  DWORD m_tid;
  HANDLE m_handle;
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
Thread<T>::~Thread() {
  if (m_handle) {
    CloseHandle(m_handle);
  }
}
