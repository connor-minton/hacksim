#pragma once

#include <string>
#include <mutex>

#include <Windows.h>
#include <windowsx.h>
#include <d2d1.h>
#include <ShObjIdl.h>
#pragma comment(lib, "d2d1")

#include "BaseWindow.h"
#include "WinUtils.h"
#include "BitmapManager.h"
#include "Exceptions.h"
#include "KeyboardManager.h"

/**
 * MainWindow
 *
 * The main window, which displays the Hack computer screen. The constructor
 * initializes COM and the destructor uninitializes COM.
 */
class MainWindow : public BaseWindow<MainWindow> {
public:
  MainWindow(BitmapManager & bm, KeyboardManager & km);

  ~MainWindow() {
    CoUninitialize();
    delete[] m_screenMem;
  }

  PCWSTR ClassName() const
    { return L"Circle Window Class"; }

  ID2D1Bitmap* GetScreenBitmap() const
    { return m_screenBitmap; }

  BOOL Create(PCWSTR lpWindowName, DWORD dwStyle);

  LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

  /**
   * Invokes the IFileOpenDialog to allow the user to select a *.hack file.
   */
  std::wstring OpenROMDialog();

private:
  ID2D1Factory *m_factory;
  ID2D1HwndRenderTarget *m_renderTarget;
  ID2D1SolidColorBrush *m_brush; // for the background
  ID2D1Bitmap *m_screenBitmap; // representing the Hack computer screen

  BitmapManager & m_bm;
  KeyboardManager & m_km;

  uint32_t *m_screenMem = nullptr; // bitmap buffer managed by BitmapManager

  HRESULT CreateGraphicsResources();
  void DiscardGraphicsResources();

  /**
   * WM_PAINT
   */
  void OnPaint();

  /**
   * WM_CREATE
   */
  void OnCreate();

  /**
   * WM_RESIZE
   */
  void Resize();

  /**
   * Fills bitmap buffer with white pixels.
   */
  void InitializeScreen();
};

MainWindow::MainWindow(BitmapManager & bm, KeyboardManager & km)
  : m_factory(nullptr),
    m_renderTarget(nullptr),
    m_brush(nullptr),
    m_screenBitmap(nullptr),
    m_bm(bm),
    m_km(km)
{
  HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
  if (FAILED(hr)) {
    throw Error("Could not initialize COM.");
  }

  m_screenMem = new uint32_t[512 * 256];
  InitializeScreen();
  m_bm.SetDestMem(m_screenMem);
}

BOOL MainWindow::Create(PCWSTR lpWindowName, DWORD dwStyle) {
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

void MainWindow::InitializeScreen() {
  for (uint32_t i = 0; i < 256 * 512; i++) {
    m_screenMem[i] = 0x00ffffff;
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
    bool updateThisTime = false;

    {
      std::lock_guard<std::mutex> lck(m_bm.mtx());
      if (m_bm.Ready()) {
        updateThisTime = true;
        m_bm.UpdateBitmap();
        m_bm.TriggerRenderHandled();
      }
    }

    if (updateThisTime) {
      auto destRectBitmap = D2D1::RectU(0, 0, 512, 256);
      hr = m_screenBitmap->CopyFromMemory(&destRectBitmap, m_screenMem, 512 * 4);
      if (FAILED(hr)) return;
    }

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

  case WM_KEYDOWN:
    return m_km.OnKeyDown(wParam, lParam);

  case WM_KEYUP:
    return m_km.OnKeyUp(wParam, lParam);
  }

  return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

std::wstring MainWindow::OpenROMDialog() {
  std::wstring out;

  IFileOpenDialog* dialog = nullptr;
  IShellItem* shItem = nullptr;
  wchar_t* filepath = nullptr;

  try {
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
      IID_IFileOpenDialog, (void**) &dialog);

    if (FAILED(hr)) {
      throw Error("There was a COM error.");
    }

    COMDLG_FILTERSPEC fileFilters[] = {
      { L"Hack ROM File", L"*.hack" }
    };

    hr = dialog->SetFileTypes(1, fileFilters);

    if (FAILED(hr)) {
      throw Error("There was a COM error.");
    }

    hr = dialog->Show(m_hwnd);

    if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED)) {
      throw Error("No file selected.");
    }

    if (FAILED(hr)) {
      throw Error("There was a COM error.");
    }

    hr = dialog->GetResult(&shItem);

    if (FAILED(hr)) {
      throw Error("There was a COM error.");
    }

    hr = shItem->GetDisplayName(SIGDN_FILESYSPATH, &filepath);
    out = filepath;
  }
  catch (std::exception&) {
    SafeCoTaskMemFree(&filepath);
    SafeRelease(&shItem);
    SafeRelease(&dialog);
    throw;
  }

  SafeCoTaskMemFree(&filepath);
  SafeRelease(&shItem);
  SafeRelease(&dialog);
  return out;
}
