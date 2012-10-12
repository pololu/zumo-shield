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
  
  private:
    // PRIVATE METHODS
    inline void init()
    {
      unsigned char initialized = 0;

      if (!initialized)
      {
        initialized = 1;
        DRV8835::init();
      }
    }
    
    // hide A/B functions by making them private
    DRV8835::setASpeed;
    DRV8835::setBSpeed;
};

#endif