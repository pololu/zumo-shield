#include "Pushbutton.h"

#define BUTTON_IS_DOWN ((digitalRead(_pin) == LOW)  ^ (_type == PULL_DOWN_EXTERNAL))
#define BUTTON_IS_UP   ((digitalRead(_pin) == HIGH) ^ (_type == PULL_DOWN_EXTERNAL))

Pushbutton::Pushbutton(unsigned char pin, unsigned char type)
{
  _pin = pin;
  _type = type;
}

void Pushbutton::waitForPress()
{
  init(); // initialize if necessary
  
  do
  {
    while (BUTTON_IS_UP); // wait for button to be pressed
    delay(10);            // debounce the button press
  }
  while (BUTTON_IS_UP);   // if button isn't still pressed, loop
}

void Pushbutton::waitForRelease()
{
  init(); // initialize if necessary
  
  do
  {
    while (BUTTON_IS_DOWN); // wait for button to be released
    delay(10);              // debounce the button release
  }
  while (BUTTON_IS_DOWN);   // if button isn't still released, loop
}

void Pushbutton::waitForButton()
{
  waitForPress();
  waitForRelease();
}

boolean Pushbutton::isPressed()
{
  return BUTTON_IS_DOWN;
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
      if (BUTTON_IS_UP)                       // if button is up
      {
        prevTimeMillis = timeMillis;
        state = 1;                            // proceed to next state
      }
      break;
      
    case 1:
      if (timeMillis - prevTimeMillis >= 15)  // if 15 ms or longer has elapsed
      {
        if (BUTTON_IS_UP)                     // and button is still up
          state = 2;                          // proceed to next state
        else
          state = 0;                          // go back to previous (initial) state
      }
      break;
      
    case 2:
      if (BUTTON_IS_DOWN)                     // if button is now down
      {
        prevTimeMillis = timeMillis;
        state = 3;                            // proceed to next state
      }
      break;
      
    case 3:
      if (timeMillis - prevTimeMillis >= 15)  // if 15 ms or longer has elapsed
      {
        if (BUTTON_IS_DOWN)                   // and button is still down
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
      if (BUTTON_IS_DOWN)                     // if button is down
      {
        prevTimeMillis = timeMillis;
        state = 1;                            // proceed to next state
      }
      break;
      
    case 1:
      if (timeMillis - prevTimeMillis >= 15)  // if 15 ms or longer has elapsed
      {
        if (BUTTON_IS_DOWN)                   // and button is still down
          state = 2;                          // proceed to next state
        else
          state = 0;                          // go back to previous (initial) state
      }
      break;
      
    case 2:
      if (BUTTON_IS_UP)                       // if button is now up
      {
        prevTimeMillis = timeMillis;
        state = 3;                            // proceed to next state
      }
      break;
      
    case 3:
      if (timeMillis - prevTimeMillis >= 15)  // if 15 ms or longer has elapsed
      {
        if (BUTTON_IS_UP)                     // and button is still up
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
  if (_type == PULL_UP_INTERNAL)
    pinMode(_pin, INPUT_PULLUP);
  else
    pinMode(_pin, INPUT); // high impedance
    
  delayMicroseconds(5); // give pull-up time to stabilize
}