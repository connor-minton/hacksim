#pragma once

#include <exception>

class Error : public std::exception {
public:
  Error() noexcept : std::exception() { }

  Error(const Error& other) noexcept : std::exception(other)
  {
    for (int i = 0; i < 255; i++) {
      m_err[i] = other.m_err[i];
      if (!other.m_err[i]) break;
    }
    m_err[255] = '\0'; // just to be safe
  }

  Error(const char* err) noexcept : std::exception() {
    for (int i = 0; i < 255; i++) {
      m_err[i] = err[i];
      if (!err[i]) break;
    }
    m_err[255] = '\0'; // just to be safe
  }

  virtual const char* what() const noexcept {
    return m_err;
  }

private:
  char m_err[256] = {0};
};