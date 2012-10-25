#include <ZumoMotors.h>

/*
 * This example uses the ZumoMotors library to drive each motor on the Zumo
 * forward, then backward. The yellow user LED is on when a motor should be
 * running forward and off when a motor should be running backward. If a
 * motor on your Zumo has been flipped, you can correct its direction by
 * uncommenting the call to flipLeftMotor() or flipRightMotor() in the setup()
 * function.
 */

#define LED_PIN 13

ZumoMotors motors;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  
  // uncomment one or both of the following lines if your motors' directions need to be flipped
  //motors.flipLeftMotor(true);
  //motors.flipRightMotor(true);
}

void loop()
{
  // run left motor forward
  
  digitalWrite(LED_PIN, HIGH);
  
  for (int speed = 0; speed <= 400; speed++)
  {
    motors.setLeftSpeed(speed);
    delay(2);
  }

  for (int speed = 400; speed >= 0; speed--)
  {
    motors.setLeftSpeed(speed);
    delay(2);
  }
  
  // run left motor backward
  
  digitalWrite(LED_PIN, LOW);
  
  for (int speed = 0; speed >= -400; speed--)
  {
    motors.setLeftSpeed(speed);
    delay(2);
  }
  
  for (int speed = -400; speed <= 0; speed++)
  {
    motors.setLeftSpeed(speed);
    delay(2);
  }

  // run right motor forward
  
  digitalWrite(LED_PIN, HIGH);
  
  for (int speed = 0; speed <= 400; speed++)
  {
    motors.setRightSpeed(speed);
    delay(2);
  }

  for (int speed = 400; speed >= 0; speed--)
  {
    motors.setRightSpeed(speed);
    delay(2);
  }
  
  // run right motor backward
  
  digitalWrite(LED_PIN, LOW);
  
  for (int speed = 0; speed >= -400; speed--)
  {
    motors.setRightSpeed(speed);
    delay(2);
  }
  
  for (int speed = -400; speed <= 0; speed++)
  {
    motors.setRightSpeed(speed);
    delay(2);
  }
  
  delay(500);
}