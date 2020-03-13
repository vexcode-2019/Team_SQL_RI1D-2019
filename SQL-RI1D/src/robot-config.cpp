#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;

motor FrontLeft(PORT2, gearSetting::ratio36_1);
motor RearLeft(PORT3, gearSetting::ratio36_1);
motor_group Left(FrontLeft, RearLeft);

motor FrontRight(PORT6, gearSetting::ratio36_1, true);
motor RearRight(PORT7, gearSetting::ratio36_1, true);
motor_group Right(FrontRight, RearRight);

drivetrain Drivetrain = drivetrain(Left, Right, 12.56, 15, 13, distanceUnits::in, 1);

motor LeftH(PORT11, gearSetting::ratio36_1);
motor RightH(PORT12, gearSetting::ratio36_1);
motor_group HMotors(LeftH, RightH);

motor IntakeLeft(PORT20);
motor IntakeRight(PORT19, true);
motor_group Intake(IntakeLeft, IntakeRight);

motor Hinge(PORT16, ratio36_1);

controller Controller;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // Nothing to initialize
}