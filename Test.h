#pragma once

#include <functional>
#include <chrono>
#include <vector>
#include <iostream>
#include <iomanip>

class TestContext {
public:
  template <typename T, typename U>
  bool expectEqual(const T& actual, const U& theoretical);

  void test(std::string name, std::function<void(TestContext&)> func);

  void printResults() const;

private:
  int testNum = 1;
  int successCt = 0;
  int failedCt = 0;
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T> v);

// --------------------------

template <typename T, typename U>
bool TestContext::expectEqual(const T& actual, const U& theoretical) {
  bool result = true;
  if (theoretical != actual) {
    std::cout << "FAILED [" << testNum << "]: expected "
              << theoretical << ", got " << actual << '\n';
    result = false;
    failedCt++;
  }
  else {
    successCt++;
  }
  testNum++;
  return result;
}

void TestContext::test(std::string name, std::function<void(TestContext&)> func) {
  std::chrono::high_resolution_clock clk;
  std::cout << name << "...";
  int old = failedCt;
  auto t1 = clk.now();
  func(*this);
  auto t2 = clk.now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
  if (old == failedCt) {
    std::cout << " PASS (" << ms.count() << " ms)\n";
  }
  else {
    std::cout << name << "... FAIL (" << ms.count() << " ms)\n";
  }
}

void TestContext::printResults() const {
  std::cout << "===================================\n"
            << "TESTS FAILED:    " << std::setw(7) << failedCt << '\n'
            << "TESTS PERFORMED: " << std::setw(7) << (failedCt + successCt) << '\n'
            << "===================================\n";
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T> v) {
  size_t len = v.size();
  out << '{';
  for (size_t i = 0; i < len; i++) {
    out << v[i];
    if (i < len - 1) {
      out << ',';
    }
  }
  out << '}';

  return out;
}
