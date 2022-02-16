#include <iostream>

#include "Chips.h"

int main() {
  Nand n1;
  n1.a = true;
  n1.b = false;

  n1.computeOutput();
  std::cout << "output: " << n1.out << '\n';

  n1.a = true;
  n1.b = true;

  n1.computeOutput();
  std::cout << "output: " << n1.out << '\n';
}