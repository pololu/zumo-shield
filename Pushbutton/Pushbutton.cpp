#include "Pushbutton.h"

Pushbutton::Pushbutton(unsigned char pin, unsigned char pullUp, unsigned char defaultState)
{
  _pin = pin;
  _pullUp = pullUp;
  _defaultState = defaultState;
}

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

void Pushbutton::waitForButton()
{
  waitForPress();
  waitForRelease();
}

// returns boolean indicating whether button is pressed
// button is pressed if it's pulled up but reads low OR if it's pulled down but reads high
boolean Pushbutton::isPressed()
{
  init(); // initialize if necessary

  return _isPressed();
}

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

void Pushbutton::init2()
{
  if (_pullUp == PULL_UP_ENABLED)
    pinMode(_pin, INPUT_PULLUP);
  else
    pinMode(_pin, INPUT); // high impedance
    
  delayMicroseconds(5); // give pull-up time to stabilize
}

inline boolean Pushbutton::_isPressed()
{
  return (digitalRead(_pin) == LOW) ^ (_defaultState == LOW); 
}