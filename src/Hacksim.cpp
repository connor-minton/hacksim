#include <iostream>
#include <vector>
#include <exception>

#include <Windows.h>

#include "MainWindow.h"
#include "SimulatorThread.h"
#include "BitmapManager.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
  BitmapManager bm;
  MainWindow win(bm);

  if (!win.Create(L"Hacksim", WS_MINIMIZEBOX | WS_SYSMENU)) {
    return 0;
  }

  ShowWindow(win.Window(), nCmdShow);

  std::vector<uint16_t> rom;
  SimulatorThread simThread(rom, bm, win.Window());

  try {
    simThread.Create();
  }
  catch (std::exception& e) {
    std::cerr << "Simulator thread could not be created: " << e.what() << '\n';
    ExitProcess(1);
  }

  // message loop

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  WaitForSingleObject(simThread, INFINITE);

  return 0;
}
