#ifndef Pushbutton_h
#define Pushbutton_h

#include <Arduino.h>

#define ZUMO_BUTTON 12

#define PULL_UP_INTERNAL   0
#define PULL_UP_EXTERNAL   1
#define PULL_DOWN_EXTERNAL 2

class Pushbutton
{
  public:
    
    Pushbutton(unsigned char pin, unsigned char type = PULL_UP_INTERNAL);
    
    void waitForPress();
    void waitForRelease();
    void waitForButton();
    
    boolean isPressed();
    
    boolean getSingleDebouncedPress();
    boolean getSingleDebouncedRelease();
    
  private:
  
    unsigned char _pin;
    unsigned char _type;
    
    inline void init()
    {
      static boolean initialized = false;

      if (!initialized)
      {
        initialized = true;
        init2();
      }
    }
    
    void init2();
};

#endif