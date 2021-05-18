#ifndef ROBOTSAFETYPROPERTIES_HPP_
#define ROBOTSAFETYPROPERTIES_HPP_

#include "ControlSystem.hpp"
#include <eeros/hal/HAL.hpp>
#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/sequencer/Sequencer.hpp>

class RobotSafetyProperties : public eeros::safety::SafetyProperties
{
  public:
    RobotSafetyProperties(ControlSystem &cs, double dt);

    // Define all possible events
    eeros::safety::SafetyEvent abort;
    eeros::safety::SafetyEvent shutdown;
    eeros::safety::SafetyEvent doSystemOn;
    eeros::safety::SafetyEvent systemStarted;
    eeros::safety::SafetyEvent emergency;
    eeros::safety::SafetyEvent resetEmergency;
    eeros::safety::SafetyEvent powerOn;
    eeros::safety::SafetyEvent powerOff;
    eeros::safety::SafetyEvent startMoving;
    eeros::safety::SafetyEvent stopMoving;
    eeros::safety::SafetyEvent motorsHalted;

    // Defina all possible levels
    eeros::safety::SafetyLevel slSystemOff;
    eeros::safety::SafetyLevel slShuttingDown;
    eeros::safety::SafetyLevel slHalting;
    eeros::safety::SafetyLevel slStartingUp;
    eeros::safety::SafetyLevel slEmergency;
    eeros::safety::SafetyLevel slEmergencyBraking;
    eeros::safety::SafetyLevel slSystemOn;
    eeros::safety::SafetyLevel slMotorPowerOn;
    eeros::safety::SafetyLevel slSystemMoving;

  private:
    // Define all critical outputs
    eeros::hal::Output<bool> *redLED;
    eeros::hal::Output<bool> *greenLED;

    // Define all critical inputs
    eeros::hal::Input<bool> *readybutton;

    ControlSystem &cs;
};

#endif // ROBOTSAFETYPROPERTIES_HPP_
