#pragma once

/**
 * ISequentialCircuit
 *
 * A sequential circuit has the values of its output pins at time t
 * dependent on the values of its input pins at time t-1.
 *
 * To simulate the sequential circuit for a given clock cycle, the
 * tick() function should be called once, followed by the tock() function.
 * The tick() and tock() functions must be idempotent.
 *
 * The tick() function represents the period of time when the clock voltage
 * is low. In tick() a circuit should tick() all of its internal ISequentialCircuit
 * components and call computeOutput() on all of its internal ICombinationalCircuit
 * components in an appropriate order so as to simulate combinational logic which can
 * be computed instantaneously. Sometimes tick() might change the value of an
 * output pin. This would happen in the case that the pin is not clocked.
 *
 * The tock() function represents the moment when the clock rises from low
 * to high. In tock() a circuit should tock() all of its ISequentialCircuit components
 * and set the values of its output pins appropriately.
 */
class ISequentialCircuit {
public:
  virtual void tick() = 0;
  virtual void tock() = 0;

  virtual ~ISequentialCircuit() { }
};
