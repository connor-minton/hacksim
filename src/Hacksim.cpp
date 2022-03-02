#include <iostream>
#include <vector>
#include <exception>

#include <Windows.h>

#include "MainWindow.h"
#include "SimulatorThread.h"
#include "BitmapManager.h"
#include "FileUtils.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
  BitmapManager bm;
  MainWindow win(bm);
  std::vector<uint16_t> rom;
  try {
    rom = FileUtils::readHackFile("../test_programs/jack/ScreenTest/ScreenTest.hack");
  }
  catch (std::exception& e) {
    std::cerr << "error: " << e.what() << '\n';
    ExitProcess(1);
  }

  if (!win.Create(L"Hacksim", WS_MINIMIZEBOX | WS_SYSMENU)) {
    return 0;
  }

  ShowWindow(win.Window(), nCmdShow);

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
