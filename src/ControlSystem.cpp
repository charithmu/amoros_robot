#include "ControlSystem.hpp"

ControlSystem::ControlSystem(double dt)
    : E1("enc1"),
      E2("enc2"),
      scale1(21.2 / M_PI),
      qdMax1(21.2),
      i1(3441.0 / 104.0),
      kM1(8.44e-3),
      M1("motor1"),
      timedomain("Main time domain", dt, true)
{
    // Name all blocks
    E1.setName("E1");
    E2.setName("E2");
    scale1.setName("scale1");
    qdMax1.setName("qdMax1");
    i1.setName("i1");
    kM1.setName("kM1");
    M1.setName("M1");

    // Name all signals
    E1.getOut().getSignal().setName("Position encoder 1 [rad]");
    E2.getOut().getSignal().setName("Position encoder 2 [rad]");
    scale1.getOut().getSignal().setName("Output shaft velocity setpoint 1 [rad/s]");
    qdMax1.getOut().getSignal().setName("Saturated output shaft velocity setpoint 1 [rad/s]");
    i1.getOut().getSignal().setName("Motor 1 velocity setpoint [rad/s]");
    kM1.getOut().getSignal().setName("Motor 1 setpoint voltage [V]");

    // Connect signals
    scale1.getIn().connect(E2.getOut());
    qdMax1.getIn().connect(scale1.getOut());
    i1.getIn().connect(qdMax1.getOut());
    kM1.getIn().connect(i1.getOut());
    M1.getIn().connect(kM1.getOut());

    // Add blocks to timedomain
    timedomain.addBlock(E1);
    timedomain.addBlock(E2);
    timedomain.addBlock(scale1);
    timedomain.addBlock(qdMax1);
    timedomain.addBlock(i1);
    timedomain.addBlock(kM1);
    timedomain.addBlock(M1);

    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);
}