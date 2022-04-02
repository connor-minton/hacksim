#pragma once

/**
 * ICombinationalCircuit
 *
 * A combinational circuit computes its output "instantaneously" when the
 * computeOutput() function is called. That is, the ICombinationalCircuit
 * object sets the values of its output pins at time t based on the values of
 * its input pins at time t.
 */
class ICombinationalCircuit {
public:
  virtual void computeOutput() = 0;

  virtual ~ICombinationalCircuit() { }
};
