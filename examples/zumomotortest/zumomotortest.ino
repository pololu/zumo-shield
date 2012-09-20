/* 
 * motor A is right motor
 * motor B is left motor
 * setSpeeds usage: setSpeeds(rightSpeed, leftSpeed); speed range is -400 to 400
 * change flipDirections if directions are wrong
 */


#include <DRV8835.h>

DRV8835 drv;

void setup() {
  drv.init();
  drv.flipDirections = 1; // change this if motor directions are wrong
}

void loop() {
  // flash led
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
  
  // run right motor
  for (int foo = 0; foo <= 400; foo++)
  {
    drv.setSpeeds(foo, 0);
    delay(1);
  }
  
  for (int foo = 400; foo >= -400; foo--)
  {
    drv.setSpeeds(foo, 0);
    delay(1);
  }
  
  for (int foo = -400; foo <= 0; foo++)
  {
    drv.setSpeeds(foo, 0);
    delay(1);
  }
  
  // run left motor
  for (int foo = 0; foo <= 400; foo++)
  {
    drv.setSpeeds(0, foo);
    delay(1);
  }
  
  for (int foo = 400; foo >= -400; foo--)
  {
    drv.setSpeeds(0, foo);
    delay(1);
  }
  
  for (int foo = -400; foo <= 0; foo++)
  {
    drv.setSpeeds(0, foo);
    delay(1);
  }
}
