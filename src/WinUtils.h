#pragma once

#include <Windows.h>

template <class T> static void SafeRelease(T** ppT) {
  if (*ppT) {
    (*ppT)->Release();
    *ppT = nullptr;
  }
}

template <class T> static void SafeCoTaskMemFree(T** ppT) {
  if (*ppT) {
    CoTaskMemFree(*ppT);
    *ppT = nullptr;
  }
}

