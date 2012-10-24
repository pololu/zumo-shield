#include "Pushbutton.h"

// constructor; takes arguments specifying whether to enable internal pull-up
// and the default state of the pin that the button is connected to
Pushbutton::Pushbutton(unsigned char pin, unsigned char pullUp, unsigned char defaultState)
{
  _pin = pin;
  _pullUp = pullUp;
  _defaultState = defaultState;
}

// wait for button to be pressed
void Pushbutton::waitForPress()
{
  init(); // initialize if necessary
  
  do
  {
    while (!_isPressed()); // wait for button to be pressed
    delay(10);            // debounce the button press
  }
  while (!_isPressed());   // if button isn't still pressed, loop
}

// wait for button to be released
void Pushbutton::waitForRelease()
{
  init(); // initialize if necessary
  
  do
  {
    while (_isPressed()); // wait for button to be released
    delay(10);           // debounce the button release
  }
  while (_isPressed());   // if button isn't still released, loop
}

// wait for button to be pressed, then released
void Pushbutton::waitForButton()
{
  waitForPress();
  waitForRelease();
}

// indicates whether button is pressed
boolean Pushbutton::isPressed()
{
  init(); // initialize if necessary

  return _isPressed();
}

// Uses a finite state machine to detect a single button press and returns
// true to indicate the press (false otherwise).  It requires the button to be
// released for at least 15 ms and then pressed for at least 15 ms before
// reporting the press.  This function handles all necessary debouncing and
// should be called repeatedly in a loop.
boolean Pushbutton::getSingleDebouncedPress()
{
  static unsigned char state = 0;
  static unsigned long prevTimeMillis = 0;
  
  unsigned long timeMillis = millis();
  
  init(); // initialize if necessary
  
  switch (state)
  {
    case 0:
      if (!_isPressed())                       // if button is released
      {
        prevTimeMillis = timeMillis;
        state = 1;                            // proceed to next state
      }
      break;
      
    case 1:
      if (timeMillis - prevTimeMillis >= 15)  // if 15 ms or longer has elapsed
      {
        if (!_isPressed())                     // and button is still released
          state = 2;                          // proceed to next state
        else
          state = 0;                          // go back to previous (initial) state
      }
      break;
      
    case 2:
      if (_isPressed())                        // if button is now pressed
      {
        prevTimeMillis = timeMillis;
        state = 3;                            // proceed to next state
      }
      break;
      
    case 3:
      if (timeMillis - prevTimeMillis >= 15)  // if 15 ms or longer has elapsed
      {
        if (_isPressed())                      // and button is still pressed
        {
          state = 0;                          // next state becomes initial state
          return true;                        // report button press
        }
        else
          state = 2;                          // go back to previous state
      }
      break;
  }
  
  return false;
}

// Uses a finite state machine to detect a single button release and returns
// true to indicate the release (false otherwise).  It requires the button to be
// pressed for at least 15 ms and then released for at least 15 ms before
// reporting the release.  This function handles all necessary debouncing and
// should be called repeatedly in a loop.
boolean Pushbutton::getSingleDebouncedRelease()
{
  static unsigned char state = 0;
  static unsigned long prevTimeMillis = 0;
  
  unsigned long timeMillis = millis();

  init(); // initialize if necessary
  
  switch (state)
  {
    case 0:
      if (_isPressed())                        // if button is pressed
      {
        prevTimeMillis = timeMillis;
        state = 1;                            // proceed to next state
      }
      break;
      
    case 1:
      if (timeMillis - prevTimeMillis >= 15)  // if 15 ms or longer has elapsed
      {
        if (_isPressed())                      // and button is still pressed
          state = 2;                          // proceed to next state
        else
          state = 0;                          // go back to previous (initial) state
      }
      break;
      
    case 2:
      if (!_isPressed())                       // if button is now released
      {
        prevTimeMillis = timeMillis;
        state = 3;                            // proceed to next state
      }
      break;
      
    case 3:
      if (timeMillis - prevTimeMillis >= 15)  // if 15 ms or longer has elapsed
      {
        if (!_isPressed())                     // and button is still released
        {
          state = 0;                          // next state becomes initial state
          return true;                        // report button release
        }
        else
          state = 2;                          // go back to previous state
      }
      break;
  }
  
  return false;
}

// initializes I/O pin for use as button inputs
void Pushbutton::init2()
{
  if (_pullUp == PULL_UP_ENABLED)
    pinMode(_pin, INPUT_PULLUP);
  else
    pinMode(_pin, INPUT); // high impedance
    
  delayMicroseconds(5); // give pull-up time to stabilize
}

// button is pressed if pin state differs from default state
inline boolean Pushbutton::_isPressed()
{
  return (digitalRead(_pin) == LOW) ^ (_defaultState == DEFAULT_STATE_LOW); 
}