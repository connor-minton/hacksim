#pragma once

#include <string>
#include <unordered_map>

class AppConfig {
public:
  /**
   * Initialize config from a file.
   */
  static void initializeFromFile(const std::wstring& filename);

  // default: false
  static bool shallowMemory()
  { return m_shallowMemory; }

  // default: false
  static bool shallowCPU()
  { return m_shallowCPU; }

private:
  static bool m_shallowMemory;
  static bool m_shallowCPU;

  static void initialize(const std::unordered_map<std::string, std::string> & keyValues);
};
