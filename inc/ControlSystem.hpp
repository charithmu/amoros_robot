#ifndef CONTROLSYSTEM_HPP_
#define CONTROLSYSTEM_HPP_

#include <eeros/control/Constant.hpp>
#include <eeros/control/Gain.hpp>
#include <eeros/control/PeripheralInput.hpp>
#include <eeros/control/PeripheralOutput.hpp>
#include <eeros/control/Saturation.hpp>
#include <eeros/control/TimeDomain.hpp>
#include <eeros/core/Executor.hpp>

using namespace eeros::control;

class ControlSystem
{
  public:
    ControlSystem(double dt);

    // Define Blocks
    PeripheralInput<> E1, E2;
    PeripheralOutput<> M1;

    Gain<> scale1, i1_inv, kM1_inv, R1;
    Saturation<> QMax1;

    TimeDomain timedomain;
};

#endif // CONTROLSYSTEM_HPP