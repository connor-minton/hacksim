#pragma once

#include <Windows.h>

/**
 * BaseWindow class adapted from the Microsoft documentation
 *
 * https://docs.microsoft.com/en-us/windows/win32/learnwin32/managing-application-state-
 */

template <class TDerived>
class BaseWindow {
public:
  static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    TDerived* pThis = nullptr;

    if (uMsg == WM_NCCREATE) {
      CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
      pThis = (TDerived*)pCreate->lpCreateParams;
      SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

      UINT dpi = GetDpiForWindow(hwnd);
      pThis->m_dpiScale = dpi / 96.0f;

      pThis->m_hwnd = hwnd;
    }
    else {
      pThis = (TDerived*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (pThis) {
      return pThis->HandleMessage(uMsg, wParam, lParam);
    }
    else {
      return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
  }

  BaseWindow() : m_hwnd(NULL) { }
  virtual ~BaseWindow() { }

  BOOL Create(
    PCWSTR lpWindowName,
    DWORD dwStyle,
    DWORD dwExStyle = 0,
    int x = CW_USEDEFAULT,
    int y = CW_USEDEFAULT,
    int nWidth = CW_USEDEFAULT,
    int nHeight = CW_USEDEFAULT,
    HWND hWndParent = 0,
    HMENU hMenu = 0
  )
  {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = TDerived::WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = ClassName();

    RegisterClass(&wc);

    m_hwnd = CreateWindowEx(
      dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
      nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this);

    return (m_hwnd ? TRUE : FALSE);
  }

  HWND Window() const { return m_hwnd; }

  template <typename T>
  float PixelsToDips(T pixels) {
    return static_cast<float>(pixels) / m_dpiScale;
  }

  void ShowError(std::string err) {
    MessageBoxA(m_hwnd, err.c_str(), "Error", MB_OK | MB_ICONERROR);
  }

protected:
  virtual PCWSTR ClassName() const = 0;
  virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

  HWND m_hwnd = NULL;
  float m_dpiScale = 1.0f;
};
