#include "ZumoMotors.h"

// Constructors ////////////////////////////////////////////////////////////////

ZumoMotors::ZumoMotors()
{
  flipRightMotor = false;
  flipLeftMotor = false;
}

// Public Methods //////////////////////////////////////////////////////////////

// Set speed for left motor; speed is a number between -400 and 400
void ZumoMotors::setLeftSpeed(int speed)
{
  if (flipLeftMotor)
    speed = -speed;
    
  setBSpeed(speed);
}

// Set speed for right motor; speed is a number between -400 and 400
void ZumoMotors::setRightSpeed(int speed)
{
  if (flipRightMotor)
    speed = -speed;
    
  setASpeed(speed);
}

// Set speed for motor 1 and 2
void ZumoMotors::setSpeeds(int leftSpeed, int rightSpeed)
{
  setLeftSpeed(leftSpeed);
  setRightSpeed(rightSpeed);
}