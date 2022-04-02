#pragma once

#include <exception>
#include <string>

/**
 * Error
 *
 * A convenience exception class that carries a string around with it.
 */
class Error : public std::exception {
public:
  Error() : std::exception(), m_err("An unexpected error has occurred.")
  { }

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