#ifndef ZumoMotors_h
#define ZumoMotors_h

#include <Arduino.h>

class ZumoMotors
{
  public:  

    ZumoMotors();
    
    static void flipLeftMotor(boolean flip);
    static void flipRightMotor(boolean flip);
    
    static void setLeftSpeed(int speed); // Set speed for left motor.
    static void setRightSpeed(int speed); // Set speed for right motor.
    static void setSpeeds(int leftSpeed, int rightSpeed); // Set speed for both left and right motors.
    
  private:

    static inline void init()
    {
      static boolean initialized = false;

      if (!initialized)
      {
        initialized = true;
        init2();
      }
    }
    
    // initializes timer1 for proper PWM generation
    static void init2();
};

#endif