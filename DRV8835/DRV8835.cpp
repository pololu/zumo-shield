#include "DRV8835.h"

// Constructors ////////////////////////////////////////////////////////////////

DRV8835::DRV8835()
{
  _APHASE = 7;
  _BPHASE = 8;
}

DRV8835::DRV8835(unsigned char APHASE, unsigned char BPHASE)
{
  // AENABLE and BENABLE cannot be remapped because the library assumes PWM is on Timer1
  _APHASE = APHASE;
  _BPHASE = BPHASE;
}

// Public Methods //////////////////////////////////////////////////////////////
void DRV8835::init()
{
  pinMode(_APHASE,  OUTPUT);
  pinMode(_AENABLE, OUTPUT);
  pinMode(_BPHASE,  OUTPUT);
  pinMode(_BENABLE, OUTPUT);

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined (__AVR_ATmega32U4__)
  // Timer 1 configuration
  // prescaler: clockI/O / 1
  // outputs enabled
  // phase-correct PWM
  // top of 400
  //
  // PWM frequency calculation
  // 16MHz / 1 (prescaler) / 2 (phase-correct) / 400 (top) = 20kHz
  TCCR1A = 0b10100000;
  TCCR1B = 0b00010001;
  ICR1 = 400;
#endif
}

// Set speed for motor A; speed is a number between -400 and 400
void DRV8835::setMASpeed(int speed)
{
  unsigned char reverse = 0;
  
  if (speed < 0)
  {
    speed = -speed;  // Make speed a positive quantity
    reverse = 1;  // Preserve the direction
  }
  if (speed > 400)  // Max PWM dutycycle
    speed = 400;
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined (__AVR_ATmega32U4__)
  OCR1A = speed;
#else
  analogWrite(_AENABLE, speed * 51 / 80); // default to using analogWrite, mapping 400 to 255
#endif
  if (reverse)
  {
    digitalWrite(_APHASE, HIGH);
  }
  else
  {
    digitalWrite(_APHASE, LOW);
  }
}

// Set speed for motor B; speed is a number between -400 and 400
void DRV8835::setMBSpeed(int speed)
{
  unsigned char reverse = 0;
  
  if (speed < 0)
  {
    speed = -speed;  // make speed a positive quantity
    reverse = 1;  // preserve the direction
  }
  if (speed > 400)  // Max 
    speed = 400;
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined (__AVR_ATmega32U4__)
  OCR1B = speed;
#else
  analogWrite(_BENABLE, speed * 51 / 80); // default to using analogWrite, mapping 400 to 255
#endif 
  if (reverse)
  {
    digitalWrite(_BPHASE, HIGH);
  }
  else
  {
    digitalWrite(_BPHASE, LOW);
  }
}

// Set speed for motors A and B
void DRV8835::setSpeeds(int mASpeed, int mBSpeed)
{
  setMASpeed(mASpeed);
  setMBSpeed(mBSpeed);
}