#pragma once

#include <string>
#include <unordered_map>

/**
 * AppConfig
 *
 * Static class for loading configuration from the .conf file.
 */
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

  /**
   * Sets the static member variables by interpreting the string key-value pairs `keyValues`.
   */
  static void initialize(const std::unordered_map<std::string, std::string> & keyValues);
};
