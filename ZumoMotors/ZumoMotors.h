#ifndef ZumoMotors_h
#define ZumoMotors_h

#include <Arduino.h>

class ZumoMotors
{
  public:  

    ZumoMotors();
    
    void setLeftSpeed(int speed); // Set speed for left motor.
    void setRightSpeed(int speed); // Set speed for right motor.
    void setSpeeds(int leftSpeed, int rightSpeed); // Set speed for both left and right motors.
    
    boolean flipRightMotor;
    boolean flipLeftMotor;
  
  private:

    static inline void init()
    {
      static unsigned char initialized = 0;

      if (!initialized)
      {
        initialized = 1;
        init2();
      }
    }
    
    // initializes timer1 for proper PWM generation
    static void init2();
};

#endif