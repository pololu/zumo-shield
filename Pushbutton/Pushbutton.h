/*! \file Pushbutton.h
 *
 * See the Pushbutton class reference for more information about this library.
 *
 * \class Pushbutton Pushbutton.h
 * \brief Read button presses and releases with debouncing
 * 
 */

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

    // constructor; takes arguments specifying whether to enable internal pull-up
    // and the default state of the pin that the button is connected to
    Pushbutton(unsigned char pin, unsigned char pullUp = PULL_UP_ENABLED, unsigned char defaultState = DEFAULT_STATE_HIGH);

    // wait for button to be pressed, released, or pressed and released
    void waitForPress();
    void waitForRelease();
    void waitForButton();

    // indicates whether button is currently pressed
    boolean isPressed();

    // more complex functions that return true once for each button transition
    // from released to pressed or pressed to released
    boolean getSingleDebouncedPress();
    boolean getSingleDebouncedRelease();

  private:

    unsigned char _pin;
    unsigned char _pullUp;
    unsigned char _defaultState;
    unsigned char gsdpState;
    unsigned char gsdrState;
    unsigned int gsdpPrevTimeMillis;
    unsigned int gsdrPrevTimeMillis;
    boolean initialized;

    inline void init()
    {
      if (!initialized)
      {
        initialized = true;
        init2();
      }
    }

    // initializes I/O pin for use as button input
    void init2();

    boolean _isPressed();
};

#endif