#include <iostream>

#include <Windows.h>

#include "MainWindow.h"

/**
 * DirectX tutorial program from Microsoft documentation
 */

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
  MainWindow win;

  if (!win.Create(L"Hacksim", WS_MINIMIZEBOX | WS_SYSMENU)) {
    return 0;
  }

  ShowWindow(win.Window(), nCmdShow);

  // message loop

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}
