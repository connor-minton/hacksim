#pragma once

#include <exception>

class Error : public std::exception {
public:
  Error() : std::exception() { }

  Error(const Error& other) : std::exception(other), m_err(other.m_err)
  { }

  Error(const char* err) : std::exception(), m_err(err)
  { }

  Error(const std::string err) : std::exception(), m_err(err)
  { }

  virtual const char* what() const {
    return m_err.c_str();
  }

private:
  std::string m_err;
};