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
#define SCALE_FACTOR_MEDIUM 0.8
#define SCALE_FACTOR_SLOW 0.3

#define DEADZONE 5
#define SPEEDUNIT velocityUnits::rpm


using namespace vex;

competition Competition;

float scaleFactor = SCALE_FACTOR_MEDIUM;

int inline joyaxis(int i) {
  return (abs(i) > DEADZONE ? int(pow(double(i) / 100, 3) * 100) : 0);
}

enum strafeDirection { left, right };

void strafe(strafeDirection dir, double dist, distanceUnits dUnits,
            double velocity, velocityUnits vUnits) {
  double wheelCircumference;
  switch (dUnits) {
  case distanceUnits::mm:
    wheelCircumference = 319.2;
    break;

  case distanceUnits::cm:
    wheelCircumference = 31.92;
    break;

  case distanceUnits::in:
    wheelCircumference = 12.57;
    break;
  }

  // directionType motorDir = (dir == strafeDirection::left) ?
  // directionType::rev : directionType::fwd;

  double rotationDistance = (dist * 360) / wheelCircumference;
  HMotors.rotateFor((dir == strafeDirection::left) ? directionType::rev
                                                   : directionType::fwd,
                    rotationDistance, rotationUnits::deg, velocity, vUnits);
}

void teleop() {
  bool clamping = false;

  Hinge.setStopping(hold);

  Controller.ButtonA.released([] { scaleFactor = SCALE_FACTOR_FAST; });
  Controller.ButtonB.released([] { scaleFactor = SCALE_FACTOR_MEDIUM; });
  Controller.ButtonX.released([] { scaleFactor = SCALE_FACTOR_SLOW; });

  while (true) {
    int ax1 = joyaxis(Controller.Axis1.position());
    int ax3 = joyaxis(Controller.Axis3.position());
    int ax4 = joyaxis(Controller.Axis4.position());

    auto factor =
        Controller.ButtonR2.pressing() ? SCALE_FACTOR_SLOW : scaleFactor;

    // Left.spin(forward, factor * (ax1 + ax3), percent);
    // Right.spin(forward,  factor * (ax1 + ax3), percent);
    Left.spin(forward, factor * (ax3 + ax1), percent);
    Right.spin(forward, factor * (ax3 - ax1), percent);
    HMotors.spin(forward, factor * (ax4), percent);

    if (Controller.ButtonL1.pressing()) {
      Hinge.spin(reverse, 40, percent);
      clamping = false;
    } else if (Controller.ButtonL2.pressing()) {
      Hinge.spin(forward, 40, percent);
      clamping = true;
    } else if (clamping) {
      Hinge.spin(forward, 100, percent);
      clamping = false;
      Hinge.stop();
    } else {
      Hinge.stop();
    }

    if (Controller.ButtonR1.pressing()) {
      Intake.spin(forward, 50, percent);
    } else if (Controller.ButtonR2.pressing()) {
      // Nothing
    } else {
      Intake.stop();
    }
  }
}

void printMotorTemp() {
  // printf("%3.0f\n", HMotors.temperature(temperatureUnits::fahrenheit));

  Controller.Screen.clearScreen();
  Controller.Screen.setCursor(1, 1);
  Controller.Screen.print("%5.5f\n", Drivetrain.torque());
}

void auton() {
  const bool BLUE_AUTON = false;

  Hinge.setBrake(hold);
  Drivetrain.setStopping(brakeType::brake);
  // Drivetrain.setTimeout(8, seconds);

  if (BLUE_AUTON) {
    // Blue code
    Drivetrain.driveFor(directionType::fwd, 36, distanceUnits::in);
    strafe(strafeDirection::left, 10, inches, 20, SPEEDUNIT);
    Hinge.spinFor(directionType::fwd, 1, timeUnits::sec, 30,
                  SPEEDUNIT);

    // Drivetrain.turnFor(-120, rotationUnits::deg);
    // Drivetrain.driveFor(directionType::fwd, 10, inches);

    strafe(strafeDirection::right, 16, inches, 20, SPEEDUNIT);
    Drivetrain.driveFor(directionType::rev, 22, distanceUnits::in);
    Drivetrain.turnFor(-120, rotationUnits::deg, 40, SPEEDUNIT);
    // HMotors.spin(directionType::rev, 10, percentUnits::pct);
    strafe(strafeDirection::right, 1, inches, 20, SPEEDUNIT);
    strafe(strafeDirection::left, 7, inches, 20, SPEEDUNIT);
    Drivetrain.driveFor(directionType::fwd, 10, distanceUnits::in);
    Drivetrain.driveFor(directionType::rev, 3, distanceUnits::in);
    strafe(strafeDirection::left, 5, inches, 20, SPEEDUNIT);
    strafe(strafeDirection::right, 1, inches, 20, SPEEDUNIT);
    
    // Release cube stack
    Hinge.spinFor(directionType::rev, 1, timeUnits::sec, 30,
                  SPEEDUNIT);
    strafe(strafeDirection::right, 16, inches, 20, SPEEDUNIT);

    // Square to wall
    Drivetrain.drive(directionType::fwd);
    task::sleep(1500);
    Drivetrain.stop();

    // Place cubes in tower
    Drivetrain.driveFor(directionType::rev, 45, inches);
    Intake.spinFor(directionType::fwd, 3, timeUnits::sec, 75,
                   velocityUnits::pct);

    // Prepare for driver control
    // Drivetrain.driveFor(directionType::fwd, 10, inches);
    // Drivetrain.turnFor(-50, rotationUnits::deg);
    // Drivetrain.driveFor(directionType::rev, 18, inches);
    // Drivetrain.turnFor(-15, rotationUnits::deg);
  } else {
    int speed = 40;
    // Red code
    Drivetrain.setDriveVelocity(speed, percent);
    Drivetrain.setTurnVelocity(speed, percent);

    strafe(strafeDirection::left, 8, inches, speed, SPEEDUNIT);
    Drivetrain.driveFor(forward, 6, inches);
    Drivetrain.driveFor(reverse, 3, inches);

    // Approach the 4 stack
    strafe(strafeDirection::left, 18, inches, speed, SPEEDUNIT);
    // Drivetrain.driveFor(forward, 4, inches);
    strafe(strafeDirection::left, 6, inches, speed, SPEEDUNIT);
    Drivetrain.turnFor(-10, degrees);

    // Grab the 4 stack
    Hinge.spinFor(forward, 1, seconds, 40, velocityUnits::pct);


    // Approach scoring zone
    Drivetrain.turnFor(-65, degrees);
    strafe(strafeDirection::left, 18, inches, speed, SPEEDUNIT);
    Drivetrain.turnFor(-55, degrees);
    strafe(strafeDirection::left, 20, inches, speed, SPEEDUNIT);
    Drivetrain.driveFor(forward, 6, inches);
    // Drivetrain.driveFor(reverse, 4, inches);

    // Release cubes into scoring zone
    // Drivetrain.turnFor(-10, degrees);
    Drivetrain.driveFor(reverse, 2, inches);
    Hinge.spinFor(reverse, 1, seconds, 40,  velocityUnits::pct);
    // Drivetrain.driveFor(forward, 2, inches);

    // Back from cubes
    strafe(strafeDirection::right, 12, inches, speed, SPEEDUNIT);

    // // Square to wall
    // Drivetrain.drive(forward);
    // task::sleep(2000);
    // Drivetrain.stop();

    // // Approach tower
    // Drivetrain.driveFor(reverse, 20, inches);
    // Drivetrain.turnFor(-55, degrees);
    // // Drivetrain.driveFor(reverse, 24, inches);

    // // // Outtake tower cube
    // Hinge.spinFor(forward, 0.5, seconds, 40,  velocityUnits::pct); // Move cage away from outtake tread
    // Intake.spinFor(directionType::fwd, 1, timeUnits::sec, 25,
    //                velocityUnits::pct);
    // task::sleep(2000);
  }
}

int main() {
  const bool PRACTICE = false;
  
  if (PRACTICE) {
    // thread print_temp = thread(printMotorTemp);
    timer::event(printMotorTemp, 200);
    auton();

    // teleop();

  }
  else {
    Competition.autonomous(auton);
    Competition.drivercontrol(teleop);

    // Initializing Robot Configuration. DO NOT REMOVE!g
    vexcodeInit();

    // auton();

    // thread print_temp = thread(printMotorTemp);

  }

}
