#include "RobotSafetyProperties.hpp"

RobotSafetyProperties::RobotSafetyProperties(ControlSystem &cs, double dt)
    : cs(cs),

      slSystemOff("System offline"),
      slShuttingDown("System shutting down"),
      slHalting("System halting"),
      slStartingUp("System starting up"),
      slEmergency("Emergency"),
      slEmergencyBraking("Emergency breaking"),
      slSystemOn("System online"),
      slMotorPowerOn("Motors powered-on"),
      slSystemMoving("System moving"),

      abort("Abort"),
      shutdown("Shutdown"),
      doSystemOn("Do system on"),
      systemStarted("System started"),
      emergency("Emergency"),
      resetEmergency("Reset emergency"),
      powerOn("Power on"),
      powerOff("Power off"),
      startMoving("Start moving"),
      stopMoving("Stop moving"),
      motorsHalted("Motors halted")
{
    eeros::hal::HAL &hal = eeros::hal::HAL::instance();

    // Declare and add critical outputs
    redLED = hal.getLogicOutput("onBoardLEDred");
    greenLED = hal.getLogicOutput("onBoardLEDgreen");

    criticalOutputs = {redLED, greenLED};

    // Declare and add critical inputs
    readybutton = eeros::hal::HAL::instance().getLogicInput("onBoardButtonPause", true);

    criticalInputs = {readybutton};

    // Add all safety levels to the safety system
    addLevel(slSystemOff);
    addLevel(slShuttingDown);
    addLevel(slHalting);
    addLevel(slStartingUp);
    addLevel(slEmergency);
    addLevel(slEmergencyBraking);
    addLevel(slSystemOn);
    addLevel(slMotorPowerOn);
    addLevel(slSystemMoving);

    // Add events to individual safety levels
    slSystemOff.addEvent(doSystemOn, slStartingUp, kPublicEvent);
    slShuttingDown.addEvent(shutdown, slSystemOff, kPrivateEvent);
    slHalting.addEvent(motorsHalted, slShuttingDown, kPrivateEvent);
    slStartingUp.addEvent(systemStarted, slSystemOn, kPrivateEvent);
    slEmergency.addEvent(resetEmergency, slSystemOn, kPrivateEvent);
    slEmergencyBraking.addEvent(motorsHalted, slEmergency, kPrivateEvent);
    slSystemOn.addEvent(powerOn, slMotorPowerOn, kPublicEvent);
    slMotorPowerOn.addEvent(startMoving, slSystemMoving, kPublicEvent);
    slMotorPowerOn.addEvent(powerOff, slSystemOn, kPublicEvent);
    slSystemMoving.addEvent(stopMoving, slMotorPowerOn, kPublicEvent);
    slSystemMoving.addEvent(emergency, slEmergencyBraking, kPublicEvent);
    slSystemMoving.addEvent(abort, slHalting, kPublicEvent);

    // Add events to multiple safety levels
    addEventToAllLevelsBetween(slEmergency, slMotorPowerOn, abort, slShuttingDown, kPublicEvent);
    addEventToAllLevelsBetween(slSystemOn, slMotorPowerOn, emergency, slEmergency, kPublicEvent);

    // Define input actions for all levels
    slSystemOff.setInputActions({ignore(readybutton)});
    slShuttingDown.setInputActions({ignore(readybutton)});
    slHalting.setInputActions({ignore(readybutton)});
    slStartingUp.setInputActions({ignore(readybutton)});
    slEmergency.setInputActions({ignore(readybutton)});
    slEmergencyBraking.setInputActions({ignore(readybutton)});
    slSystemOn.setInputActions({check(readybutton, false, powerOn)});
    slMotorPowerOn.setInputActions({ignore(readybutton)});
    slSystemMoving.setInputActions({ignore(readybutton)});

    // Define output actions for all levels
    slSystemOff.setInputActions({set(redLED, true), set(greenLED, false)});
    slShuttingDown.setInputActions({set(redLED, true), set(greenLED, false)});
    slHalting.setInputActions({set(redLED, true), set(greenLED, false)});
    slStartingUp.setInputActions({set(redLED, true), set(greenLED, false)});
    slEmergency.setInputActions({set(redLED, true), set(greenLED, false)});
    slEmergencyBraking.setInputActions({set(redLED, true), set(greenLED, false)});
    slSystemOn.setInputActions({set(redLED, true), set(greenLED, true)});
    slMotorPowerOn.setInputActions({set(redLED, false), set(greenLED, true)});
    slSystemMoving.setInputActions({set(redLED, false), set(greenLED, true)});

    // Define and add level actions
    slSystemOff.setLevelAction([&](SafetyContext *privateContext) {
        eeros::Executor::stop();
        eeros::sequencer::Sequencer::instance().abort();
    });

    slShuttingDown.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.stop();
        privateContext->triggerEvent(shutdown);
    });

    slHalting.setLevelAction([&](SafetyContext *privateContext) {
        // Check if motors are standing sill
        privateContext->triggerEvent(motorsHalted);
    });

    slStartingUp.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.start();
        privateContext->triggerEvent(systemStarted);
    });

    slEmergency.setLevelAction([&, dt](SafetyContext *privateContext) {
        if (slEmergency.getNofActivations() * dt == 1) // wait 1 sec
        {
            static int counter = 0;
            if (counter++ == 3) privateContext->triggerEvent(abort); // abort after entering emergency sequence 4 times
            privateContext->triggerEvent(resetEmergency);
        }
    });

    slEmergencyBraking.setLevelAction([&](SafetyContext *privateContext) {
        // Check if motors are standing still
        privateContext->triggerEvent(motorsHalted);
    });

    slSystemOn.setLevelAction([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(powerOn);
    });

    slMotorPowerOn.setLevelAction([&](SafetyContext *privateContext) {
        // check if motors are moving
        privateContext->triggerEvent(startMoving);
    });

    slSystemMoving.setLevelAction([&](SafetyContext *privateContext) {
        // check if motors are standing still -> slMortorsPowerOn
        // privateContext->triggerEvent(emergency);
    });

    // Define entry level
    setEntryLevel(slSystemOff);

    // Define exit function
    exitFunction = ([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(abort);
    });
}
