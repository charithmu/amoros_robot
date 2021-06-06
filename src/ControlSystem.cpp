#include "ControlSystem.hpp"

ControlSystem::ControlSystem(double dt)
    : E1("enc1"), E2("enc2"),
      scale1(0.03 / M_PI),
      QMax1(0.1),
      i1_inv(104.0 / 3441.0),
      kM1_inv(1 / 8.44e-3),
      R1(8.0),
      M1("motor1"),
      timedomain("Main time domain", dt, true)
{
    // Name all blocks
    E1.setName("E1");
    E2.setName("E2");
    scale1.setName("scale1");
    QMax1.setName("QMax1");
    i1_inv.setName("i1_inv");
    kM1_inv.setName("kM1_inv");
    R1.setName("R1");
    M1.setName("M1");

    // Name all signals
    E1.getOut().getSignal().setName("Position encoder 1 [rad]");
    E2.getOut().getSignal().setName("Position encoder 2 [rad]");
    scale1.getOut().getSignal().setName("Output shaft torque setpoint 1 [Nm]");
    QMax1.getOut().getSignal().setName("Saturated output shaft torque setpoint 1 [Nm]");
    i1_inv.getOut().getSignal().setName("Motor 1 torque setpoint [Nm]");
    kM1_inv.getOut().getSignal().setName("Motor 1 setpoint current [A]");
    R1.getOut().getSignal().setName("Motor 1 setpoint voltage [V]");

    // Connect signals
    scale1.getIn().connect(E2.getOut());
    QMax1.getIn().connect(scale1.getOut());
    i1_inv.getIn().connect(QMax1.getOut());
    kM1_inv.getIn().connect(i1_inv.getOut());
    R1.getIn().connect(kM1_inv.getOut());
    M1.getIn().connect(R1.getOut());

    // Add blocks to timedomain
    timedomain.addBlock(E1);
    timedomain.addBlock(E2);
    timedomain.addBlock(scale1);
    timedomain.addBlock(QMax1);
    timedomain.addBlock(i1_inv);
    timedomain.addBlock(kM1_inv);
    timedomain.addBlock(R1);
    timedomain.addBlock(M1);

    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);
}