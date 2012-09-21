/* 
 * motor A is right motor
 * motor B is left motor
 * setSpeeds usage: setSpeeds(rightSpeed, leftSpeed); speed range is -400 to 400
 * change flipDirections if directions are wrong
 */


#include <ZumoMotors.h>

ZumoMotors motors;

void setup() {
  motors.init();
  //drv.flipDirections = 1; // change this if motor directions are wrong
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
    motors.setRightSpeed(foo);
    delay(1);
  }
  
  for (int foo = 400; foo >= -400; foo--)
  {
    motors.setRightSpeed(foo);
    delay(1);
  }
  
  for (int foo = -400; foo <= 0; foo++)
  {
    motors.setRightSpeed(foo);
    delay(1);
  }
  
  // run left motor
  for (int foo = 0; foo <= 400; foo++)
  {
    motors.setLeftSpeed(foo);
    delay(1);
  }
  
  for (int foo = 400; foo >= -400; foo--)
  {
    motors.setLeftSpeed(foo);
    delay(1);
  }
  
  for (int foo = -400; foo <= 0; foo++)
  {
    motors.setLeftSpeed(foo);
    delay(1);
  }
}
