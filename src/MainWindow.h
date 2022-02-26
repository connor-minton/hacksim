#pragma once

#include <string>

#include <Windows.h>
#include <windowsx.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")

#include "BaseWindow.h"
#include "WinUtils.h"

class MainWindow : public BaseWindow<MainWindow> {
public:
  MainWindow() 
    : m_factory(nullptr), m_renderTarget(nullptr), m_brush(nullptr),
      m_screenBitmap(nullptr)
  {
    m_screenMem = new uint32_t[512 * 256];
    FillScreen();
  }

  ~MainWindow() { delete[] m_screenMem; }

  BOOL Create(PCWSTR lpWindowName, DWORD dwStyle)
  { 
    RECT windowRect;
    windowRect.top = 100;
    windowRect.left = 100;
    windowRect.right = 612;
    windowRect.bottom = 356;

    if (!AdjustWindowRectEx(&windowRect, dwStyle, FALSE, 0)) {
      OutputDebugString(L"ADJUST RETURNED FALSE\n");
    }

    int width = windowRect.right - windowRect.left;
    int height = windowRect.bottom - windowRect.top;

    return BaseWindow<MainWindow>::Create(
      lpWindowName, dwStyle, 0, CW_USEDEFAULT, CW_USEDEFAULT,
      width, height);
  }

  PCWSTR ClassName() const { return L"Circle Window Class"; }
  LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
  ID2D1Factory *m_factory;
  ID2D1HwndRenderTarget *m_renderTarget;
  ID2D1SolidColorBrush *m_brush;
  ID2D1Bitmap *m_screenBitmap;

  uint32_t *m_screenMem = nullptr;

  void CalculateLayout();
  HRESULT CreateGraphicsResources();
  void DiscardGraphicsResources();
  void OnPaint();
  void OnCreate();
  void Resize();

  void FillScreen();
  void UpdateScreen();
};

void MainWindow::CalculateLayout() {
}

void MainWindow::FillScreen() {
  for (uint32_t i = 0; i < 256; i++) {
    for (uint32_t j = 0; j < 512; j++) {
      m_screenMem[i*512 + j] = (i << 16) | (i << 8) | i;
    }
  }
}

void MainWindow::UpdateScreen() {
  for (uint32_t i = 0; i < 256; i++) {
    for (uint32_t j = 0; j < 512; j++) {
      m_screenMem[i*512 + j] = (i << 16) | (i << 8) | 255;
    }
  }
}

void MainWindow::OnCreate() {
  RECT cr, wr;

  GetClientRect(m_hwnd, &cr);
  GetWindowRect(m_hwnd, &wr);

  int cWidth = cr.right - cr.left;
  int cHeight = cr.bottom - cr.top;
  int wWidth = wr.right - wr.left;
  int wHeight = wr.bottom - wr.top;

  int width = 512 + wWidth - cWidth;
  int height = 256 + wHeight - cHeight;

  SetWindowPos(m_hwnd, 
               NULL,
               wr.left,
               wr.top,
               width,
               height,
               0);
}

HRESULT MainWindow::CreateGraphicsResources() {
  HRESULT hr = S_OK;
  if (!m_renderTarget) {
    RECT rc;
    GetClientRect(m_hwnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

    hr = m_factory->CreateHwndRenderTarget(
      D2D1::RenderTargetProperties(),
      D2D1::HwndRenderTargetProperties(m_hwnd, size),
      &m_renderTarget);

    if (FAILED(hr)) return hr;

    const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
    hr = m_renderTarget->CreateSolidColorBrush(color, &m_brush);

    if (FAILED(hr)) return hr;

    auto bmProps = D2D1::BitmapProperties();
    bmProps.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);

    auto bmSize = D2D1::SizeU(512, 256);

    hr = m_renderTarget->CreateBitmap(bmSize, m_screenMem, bmSize.width*4, bmProps, &m_screenBitmap);

    if (FAILED(hr)) return hr;

    UpdateScreen();
    auto destRect = D2D1::RectU(411, 155, 511, 255);
    hr = m_screenBitmap->CopyFromMemory(&destRect, m_screenMem + (512*155 + 411), 512 * 4);
  }

  return hr;
}

void MainWindow::DiscardGraphicsResources() {
  SafeRelease(&m_renderTarget);
  SafeRelease(&m_brush);
}

void MainWindow::OnPaint() {
  HRESULT hr = CreateGraphicsResources();
  if (SUCCEEDED(hr)) {
    RECT rc;
    GetClientRect(m_hwnd, &rc);
    auto destRect = D2D1::RectF(rc.left, rc.top, rc.right, rc.bottom);

    PAINTSTRUCT ps;
    BeginPaint(m_hwnd, &ps);

    m_renderTarget->BeginDraw();

    m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
    m_renderTarget->DrawBitmap(m_screenBitmap, destRect);

    hr = m_renderTarget->EndDraw();
    if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET) {
      DiscardGraphicsResources();
    }

    EndPaint(m_hwnd, &ps);
  }
}

void MainWindow::Resize() {
  if (m_renderTarget) {
    RECT rc;
    GetClientRect(m_hwnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

    m_renderTarget->Resize(size);
    CalculateLayout();
    InvalidateRect(m_hwnd, NULL, FALSE);
  }
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
  case WM_CREATE:
    if (FAILED(D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_factory)))
    {
      return -1; // fail CreateWindowEx
    }
    OnCreate();
    return 0;

  case WM_DESTROY:
    DiscardGraphicsResources();
    SafeRelease(&m_factory);
    PostQuitMessage(0);
    return 0;

  case WM_PAINT:
    OnPaint();
    return 0;

  case WM_SIZE:
    Resize();
    return 0;
  }

  return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}
