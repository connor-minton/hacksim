#pragma once

#include <stdexcept>
#include <string>

#include <Windows.h>

#include "Thread.h"

class ExampleThread : public Thread<ExampleThread> {
public:
  int val1 = 0;
  int val2 = 0;

  static DWORD WINAPI Run(void* data);
};

DWORD WINAPI ExampleThread::Run(void* data) {
  ExampleThread* td = (ExampleThread*)data;
  std::wstring msg = L"Parameters = " + std::to_wstring(td->val1)
    + L", " + std::to_wstring(td->val2) + L'\n';
  DWORD dwChars;

  HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hStdout == INVALID_HANDLE_VALUE) {
    td->m_result = 1;
    return 1;
  }

  WriteConsole(hStdout, msg.c_str(), msg.size(), &dwChars, NULL);
  td->m_result = 0;
  return 0;
}

