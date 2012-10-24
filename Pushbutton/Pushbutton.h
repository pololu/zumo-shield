#ifndef Pushbutton_h
#define Pushbutton_h

#include <Arduino.h>

#define ZUMO_BUTTON 12

#define PULL_UP_DISABLED    0
#define PULL_UP_ENABLED     1

#define DEFAULT_STATE_LOW   0
#define DEFAULT_STATE_HIGH  1

class Pushbutton
{
  public:
    
    Pushbutton(unsigned char pin, unsigned char pullUp = PULL_UP_ENABLED, unsigned char defaultState = DEFAULT_STATE_HIGH);
    
    void waitForPress();
    void waitForRelease();
    void waitForButton();
    
    boolean isPressed();
    
    boolean getSingleDebouncedPress();
    boolean getSingleDebouncedRelease();
    
  private:
  
    unsigned char _pin;
    unsigned char _pullUp;
    unsigned char _defaultState;
    
    inline void init()
    {
      static boolean initialized = false;

      if (!initialized)
      {
        initialized = true;
        init2();
      }
    }
    
    // initializes I/O pins for use as button inputs
    void init2();
    
    // behaves the same as isPressed, but skips init
    boolean _isPressed();
};

#endif