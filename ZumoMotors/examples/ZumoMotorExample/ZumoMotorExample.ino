/* 
 * range of valid speeds is -400 to 400
 */

#include <ZumoMotors.h>

ZumoMotors motors;

void setup() {
  pinMode(13, OUTPUT);
  motors.flipLeftMotor(false);
  motors.flipRightMotor(false);
}

void loop() {
  // flash led
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
  
  // run right motor
  for (int speed = 0; speed <= 400; speed++)
  {
    motors.setRightSpeed(speed);
    delay(1);
  }
  
  for (int speed = 400; speed >= -400; speed--)
  {
    motors.setRightSpeed(speed);
    delay(1);
  }
  
  for (int speed = -400; speed <= 0; speed++)
  {
    motors.setRightSpeed(speed);
    delay(1);
  }
  
  // run left motor
  for (int speed = 0; speed <= 400; speed++)
  {
    motors.setLeftSpeed(speed);
    delay(1);
  }
  
  for (int speed = 400; speed >= -400; speed--)
  {
    motors.setLeftSpeed(speed);
    delay(1);
  }
  
  for (int speed = -400; speed <= 0; speed++)
  {
    motors.setLeftSpeed(speed);
    delay(1);
  }
}