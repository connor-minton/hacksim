#pragma once

class ISequentialCircuit {
public:
  virtual void tick() = 0;
  virtual void tock() = 0;

  virtual ~ISequentialCircuit() { }
};
