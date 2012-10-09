/* 
 * range of valid speeds is -400 to 400
 */

#include <DRV8835.h>
#include <ZumoMotors.h>

ZumoMotors motors;

void setup() {
  motors.init();
  pinMode(13, OUTPUT);
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