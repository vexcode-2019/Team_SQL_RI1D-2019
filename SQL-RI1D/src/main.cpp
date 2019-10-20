/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       john                                                      */
/*    Created:      Sat Oct 19 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

#define SCALE_FACTOR_FAST 1.0
#define SCALE_FACTOR_MEDIUM 0.4
#define SCALE_FACTOR_SLOW 0.25

#define DEADZONE 5

using namespace vex;

motor FrontLeft(PORT1);
motor RearLeft(PORT2);
motor_group Left(FrontLeft, RearLeft);

motor FrontRight(PORT6);
motor RearRight(PORT7);
motor_group Right(FrontRight, RearRight);

motor LeftH(PORT11);
motor RightH(PORT12);
motor_group HMotors(LeftH, RightH);

motor IntakeLeft(PORT20);
motor IntakeRight(PORT19, true);
motor_group Intake(IntakeLeft, IntakeRight);

motor Hinge(PORT16, ratio36_1);

controller Controller;

float scaleFactor = SCALE_FACTOR_SLOW;

int inline joyaxis(int i) {
  return (abs(i) > DEADZONE ? int(pow(double(i)/100, 3)*100) : 0);
}


int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  bool clamping = false;

  Hinge.setStopping(hold);

  Controller.ButtonA.released([]{ scaleFactor = SCALE_FACTOR_FAST; });
  Controller.ButtonB.released([]{ scaleFactor = SCALE_FACTOR_MEDIUM; });
  Controller.ButtonX.released([]{ scaleFactor = SCALE_FACTOR_SLOW; });

  while (true){
    int ax1 = joyaxis(Controller.Axis1.position());
    int ax3 = joyaxis(Controller.Axis3.position());
    int ax4 = joyaxis(Controller.Axis4.position());

    Left.spin(forward, scaleFactor * (ax1 + ax3), percent);
    Right.spin(forward,  scaleFactor * (ax1 - ax3), percent);
    HMotors.spin(forward, scaleFactor * (ax4), percent);

    if (Controller.ButtonL1.pressing()) {
      Hinge.spin(reverse, 20, percent);
      clamping = false;
    } else if (Controller.ButtonL2.pressing()) {
      Hinge.spin(forward, 20, percent);
      clamping = true;
    } else if (clamping) {
      Hinge.spin(forward, 5, percent);
    } else {
      Hinge.stop();
    }

    if (Controller.ButtonR1.pressing()){
      Intake.spin(forward, 50, percent);
    }
    else if (Controller.ButtonR2.pressing()){
      Intake.spin(reverse, 50, percent);
    }
    else{
      Intake.stop();
    }
  }
}
