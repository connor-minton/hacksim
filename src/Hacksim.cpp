/**
 * Hacksim.cpp
 *
 * HackSim application entry point.
 */

#include <iostream>
#include <filesystem>
#include <vector>
#include <exception>

#include <Windows.h>

#include "AppConfig.h"
#include "MainWindow.h"
#include "SimulatorThread.h"
#include "BitmapManager.h"
#include "FileUtils.h"
#include "KeyboardManager.h"

void initializeAppConfig() {
  namespace fs = std::filesystem;
  if (fs::is_regular_file(L"./hacksim.conf")) {
    AppConfig::initializeFromFile(L"hacksim.conf");
  }
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
  BitmapManager bm;
  KeyboardManager km;
  MainWindow win(bm, km);

  std::vector<uint16_t> rom;
  try {
    initializeAppConfig();
    std::wstring inputROMFile = win.OpenROMDialog();
    rom = FileUtils::readHackFile(inputROMFile);
  }
  catch (std::exception& e) {
    win.ShowError(e.what());
    ExitProcess(1);
  }

  // this combination of options makes the window non-resizable
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
