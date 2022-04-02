/**
 * WinUtils.h
 *
 * Some utility functions for working with the Windows API.
 */

#pragma once

#include <Windows.h>

/**
 * ppT
 *   The address of a pointer to a COM object.
 *
 * If *ppT is not null, then *ppT is released and *ppT is set to nullptr.
 * Otherwise, nothing happens.
 */
template <class T> static void SafeRelease(T** ppT) {
  if (*ppT) {
    (*ppT)->Release();
    *ppT = nullptr;
  }
}

/**
 * ppT
 *   The address of a pointer to memory allocated by CoTaskMemAlloc() or CoTaskMemRealloc().
 *
 * If *ppT is not null, then *ppT is released and *ppT is set to nullptr.
 * Otherwise, nothing happens.
 */
template <class T> static void SafeCoTaskMemFree(T** ppT) {
  if (*ppT) {
    CoTaskMemFree(*ppT);
    *ppT = nullptr;
  }
}

