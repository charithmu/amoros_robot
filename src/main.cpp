#include "ControlSystem.hpp"
#include "MainSequence.hpp"
#include "RobotSafetyProperties.hpp"
#include <eeros/core/Executor.hpp>
#include <eeros/hal/HAL.hpp>
#include <eeros/logger/StreamLogWriter.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <signal.h>

void signalHandler(int signum)
{
    eeros::safety::SafetySystem::exitHandler();
    eeros::sequencer::Sequencer::instance().abort(); // needed?
}

int main(int argc, char **argv)
{
    const double dt = 0.001; // 100Hz
    eeros::logger::Logger::setDefaultStreamLogger(std::cout);
    eeros::logger::Logger log = eeros::logger::Logger::getLogger();

    log.info() << "Starting amaros_robot...";

    log.info() << "Initializing hardware...";
    eeros::hal::HAL &hal = eeros::hal::HAL::instance();
    hal.readConfigFromFile(&argc, argv);

    log.info() << "Initializing control system...";
    ControlSystem cs(dt);

    log.info() << "Initializing safety system...";
    RobotSafetyProperties sp(cs, dt);
    eeros::safety::SafetySystem ss(sp, dt);

    cs.timedomain.registerSafetyEvent(ss, sp.abort); // fired if timedomain fails to run properly
    signal(SIGINT, signalHandler);

    log.info() << "Initializing sequencer...";
    auto &sequencer = eeros::sequencer::Sequencer::instance();
    MainSequence mainSequence("Main Sequence", sequencer, ss, sp, cs);
    mainSequence();

    log.info() << "Initializing executor...";
    auto &executor = eeros::Executor::instance();

    executor.setMainTask(ss);
    ss.triggerEvent(sp.doSystemOn); // start the system
    executor.run();

    mainSequence.wait();

    log.info() << "amaros_robot finished...";

    return 0;
}