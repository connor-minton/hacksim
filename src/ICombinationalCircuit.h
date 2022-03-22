#pragma once

class ICombinationalCircuit {
public:
  virtual void computeOutput() = 0;

  virtual ~ICombinationalCircuit() { }
};
