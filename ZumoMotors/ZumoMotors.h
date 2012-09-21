#ifndef ZumoMotors_h
#define ZumoMotors_h

#include <Arduino.h>
#include "../DRV8835/DRV8835.h"

class ZumoMotors : public DRV8835
{
  public:  
    // CONSTRUCTORS
    ZumoMotors();
    
    // PUBLIC METHODS
    void setLeftSpeed(int speed); // Set speed for left motor.
    void setRightSpeed(int speed); // Set speed for right motor.
    void setSpeeds(int leftSpeed, int rightSpeed); // Set speed for both left and right motors.
    
    boolean flipRightMotor;
    boolean flipLeftMotor;
};

#endif