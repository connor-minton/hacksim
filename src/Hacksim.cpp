#include <iostream>
#include <vector>
#include <exception>

#include <Windows.h>

#include "MainWindow.h"
#include "SimulatorThread.h"
#include "BitmapManager.h"
#include "FileUtils.h"
#include "KeyboardManager.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
  BitmapManager bm;
  KeyboardManager km;
  MainWindow win(bm, km);
  std::vector<uint16_t> rom;
  try {
    std::wstring inputROMFile = win.OpenROMDialog();
    rom = FileUtils::readHackFile(inputROMFile);
  }
  catch (std::exception& e) {
    win.ShowError(e.what());
    ExitProcess(1);
  }

  if (!win.Create(L"HackSim", WS_MINIMIZEBOX | WS_SYSMENU)) {
    return 0;
  }

  ShowWindow(win.Window(), nCmdShow);

  SimulatorThread simThread(rom, bm, km, win.Window());

  try {
    simThread.Create();
  }
  catch (std::exception& e) {
    win.ShowError("Simulator thread could not be created: " + std::string(e.what()));
    ExitProcess(1);
  }

  // message loop

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  simThread.Terminate();
  return 0;
}
