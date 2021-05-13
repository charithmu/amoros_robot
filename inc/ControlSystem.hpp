#ifndef CONTROLSYSTEM_HPP_
#define CONTROLSYSTEM_HPP_

#include <eeros/control/Constant.hpp>
#include <eeros/control/Gain.hpp>
#include <eeros/control/TimeDomain.hpp>
#include <eeros/core/Executor.hpp>

using namespace eeros::control;

class ControlSystem
{
  public:
    ControlSystem(double dt);

    // Define Blocks
    Constant<> myConstant;
    Gain<> myGain;

    TimeDomain timedomain;
};

#endif // CONTROLSYSTEM_HPP