#ifndef MAINSEQUENCE_HPP_
#define MAINSEQUENCE_HPP_

#include "ControlSystem.hpp"
#include "RobotSafetyProperties.hpp"
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/Wait.hpp>

class MainSequence : public eeros::sequencer::Sequence
{
  public:
    MainSequence(std::string name,
                 eeros::sequencer::Sequencer &seq,
                 eeros::safety::SafetySystem &ss,
                 RobotSafetyProperties &sp,
                 ControlSystem &cs)
        : eeros::sequencer::Sequence(name, seq),
          ss(ss),
          sp(sp),
          cs(cs),

          sleep("Sleep", this)
    {
        log.info() << "Sequence created: " << name;
    }

    int action()
    {
        while (eeros::sequencer::Sequencer::running)
        {
            sleep(1.0);
            log.info() << cs.E2.getOut().getSignal();
            log.info() << cs.scale1.getOut().getSignal();
            log.info() << cs.QMax1.getOut().getSignal();
            log.info() << cs.i1_inv.getOut().getSignal();
            log.info() << cs.kM1_inv.getOut().getSignal();
            log.info() << cs.R1.getOut().getSignal();
        }
        return 0;
    }

  private:
    eeros::safety::SafetySystem &ss;
    ControlSystem &cs;
    RobotSafetyProperties &sp;

    eeros::sequencer::Wait sleep;
};

#endif // MAINSEQUENCE_HPP_