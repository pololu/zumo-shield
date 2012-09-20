#ifndef DRV8835_h
#define DRV8835_h

#include <Arduino.h>

class DRV8835
{
  public:  
    // CONSTRUCTORS
    DRV8835();
    
    // PUBLIC METHODS
    void init(); // Initialize TIMER 1, set the PWM to 20kHZ.
    void setMASpeed(int speed); // Set speed for M1.
    void setMBSpeed(int speed); // Set speed for M2.
    void setSpeeds(int mASpeed, int mBSpeed); // Set speed for both M1 and M2.
    
    unsigned char flipRightMotor;
    unsigned char flipLeftMotor;
    
  private:
    static const unsigned char _APHASE = 7;
    static const unsigned char _AENABLE = 9;
    static const unsigned char _BPHASE = 8;
    static const unsigned char _BENABLE = 10;
};

#endif