/**
 * Test.h
 *
 * A simple unit testing framework.
 */

#pragma once

#include <functional>
#include <chrono>
#include <vector>
#include <iostream>
#include <iomanip>

/**
 * TestContext
 *
 * Create a TestContext to measure the number of tests that succeed and fail.
 *
 * Example:
 *     TestContext cx;
 *     cx.test("This test should pass", [](_cx) { _cx.expectEqual(1, 1); });
 *     cx.test("This test should fail", [](_cx) { _cx.expectEqual(0, 1); });
 *     cx.printResults();
 */
class TestContext {
public:
  /**
   * If actual == theoretical, does nothing. Otherwise, increments the tests failed count.
   */
  template <typename T, typename U>
  bool expectEqual(const T& actual, const U& theoretical);

  /**
   * Runs the func and prints the results, including the time elapsed.
   */
  void test(std::string name, std::function<void(TestContext&)> func);

  /**
   * Prints the number of tests run and number of tests that failed.
   */
  void printResults() const;

private:
  // incremented for every expectXXX() call
  int testNum = 1;

  int successCt = 0;
  int failedCt = 0;

  // Keeps track of the number of failed expectXXX() calls in the current test() call.
  int currentTestFailed = 0;
};

/**
 * Output a vector.
 */
template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T> v);

// --------------------------

template <typename T, typename U>
bool TestContext::expectEqual(const T& actual, const U& theoretical) {
  bool result = true;
  if (theoretical != actual) {
    if (currentTestFailed < 6) {
      std::cout << "FAILED [" << testNum << "]: expected "
                << theoretical << ", got " << actual << '\n';
    }
    result = false;
    failedCt++;
    currentTestFailed++;
  }
  else {
    successCt++;
  }
  testNum++;
  return result;
}

void TestContext::test(std::string name, std::function<void(TestContext&)> func) {
  currentTestFailed = 0;
  std::chrono::high_resolution_clock clk;
  std::cout << name << "...";
  auto t1 = clk.now();
  func(*this);
  auto t2 = clk.now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
  if (currentTestFailed > 5) {
    std::cout << "[" << (currentTestFailed-5) << " other failures omitted]\n";
  }

  if (currentTestFailed == 0) {
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
