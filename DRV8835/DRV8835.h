#ifndef DRV8835_h
#define DRV8835_h

#include <Arduino.h>

class DRV8835
{
  public:  
    // CONSTRUCTORS
    DRV8835(); // default pins
    DRV8835(unsigned char APHASE, unsigned char BPHASE);
    
    // PUBLIC METHODS
    void init(); // Initialize TIMER 1, set the PWM to 20kHz.
    void setMASpeed(int speed); // Set speed for motor A.
    void setMBSpeed(int speed); // Set speed for motor B.
    void setSpeeds(int mASpeed, int mBSpeed); // Set speed for both M1 and M2.
    
  private:
    unsigned char _APHASE;
    static const unsigned char _AENABLE = 9;
    unsigned char _BPHASE;
    static const unsigned char _BENABLE = 10;
};

#endif