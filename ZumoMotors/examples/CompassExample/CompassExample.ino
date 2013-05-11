#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <Wire.h>
#include <LSM303.h>

/* This example uses the magnetometer in the Zumo Shield's onboard
 * LSM303DLHC to help the Zumo make precise 90-degree turns and drive
 * in squares. It uses the ZumoMotors, Pushbutton, and LSM303
 * (compass) libraries. The LSM303 library is not included in the Zumo
 * Shield libraries; it can be downloaded from GitHub at:
 *
 *   https://github.com/pololu/LSM303
 *
 * This program first first calibrates the compass to account for
 * offsets in its output. Calibration is accomplished in setup().
 *
 * In loop(), The driving angle then changes its offset by 90 degrees
 * from the heading every second. Essentially, this navigates the
 * Zumo to drive in square patterns.
 *
 * It is important to note that stray magnetic fields from electric
 * current (including from the Zumo's own motors) and the environment
 * (for example, steel rebar in a concrete floor) might adversely
 * affect readings from the LSM303 compass and make them less
 * reliable.
 */

#define SPEED 200 // maximum motor speed when turning or going straight

#define CALIB_SAMPLES 70
#define CRA_REG_M_220HZ 0x1C // CRA_REG_M value for magnetometer 220 Hz update rate

// allowed deviation (in degrees) relative to target angle that must be achieved before driving straight
#define DEVIATE_THRESHOLD 5

#define DEVIATE_RIGHT_THRESHOLD   DEVIATE_THRESHOLD
#define DEVIATE_LEFT_THRESHOLD    (360 - DEVIATE_THRESHOLD)

/* Public variables that may be used for compass are:
 * m_min - minimum magnetic vector
 * m_max - maximum magnetic vector
 * m     - magnetic vector
 * a     - acceleration vector
 */

ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
LSM303 compass;

// Setup will calibrate our compass
// We calibrate by finding maximum/minimum magnetic readings
void setup()
{
  // The highest possible magnetic value to read in any direction is 2047
  // The lowest possible magnetic value to read in any direction is -2047
  LSM303::vector running_min = {2047, 2047, 2047}, running_max = {-2048, -2048, -2048};
  unsigned char index;

  Serial.begin(9600);

  // Initiate the Wire library and join the I2C bus as a master
  Wire.begin();

  // Initiate LSM303
  compass.init();

  // Enables Accelerometer and Magnetometer
  compass.enableDefault();

  compass.setMagGain(compass.magGain_25);                  // +/- 2.5 gauss sensitivity to hopefully avoid overflow problems
  compass.writeMagReg(LSM303_CRA_REG_M, CRA_REG_M_220HZ);  // 220 Hz compass update rate

  float min_x_avg[CALIB_SAMPLES];
  float min_y_avg[CALIB_SAMPLES];
  float max_x_avg[CALIB_SAMPLES];
  float max_y_avg[CALIB_SAMPLES];

  button.waitForButton();

  Serial.println("starting calibration");

  // To calibrate the magnetometer we must spin the Zumo to find the Max/Min
  // magnetic vectors. This information is used to find North and East.
  motors.setLeftSpeed(SPEED);
  motors.setRightSpeed(-SPEED);

  for(index = 0; index < CALIB_SAMPLES; index ++)
  {
    // Read in magnetic vector m
    compass.read();

    running_min.x = min(running_min.x, compass.m.x);
    running_min.y = min(running_min.y, compass.m.y);

    running_max.x = max(running_max.x, compass.m.x);
    running_max.y = max(running_max.y, compass.m.y);

    Serial.println(index);

    delay(50);
  }

  motors.setLeftSpeed(0);
  motors.setRightSpeed(0);

  Serial.print("max.x   ");
  Serial.print(running_max.x);
  Serial.println();
  Serial.print("max.y   ");
  Serial.print(running_max.y);
  Serial.println();
  Serial.print("min.x   ");
  Serial.print(running_min.x);
  Serial.println();
  Serial.print("min.y   ");
  Serial.print(running_min.y);
  Serial.println();

  // Set calibrated values to compass.m_max and compass.m_min
  compass.m_max.x = running_max.x;
  compass.m_max.y = running_max.y;
  compass.m_min.x = running_min.x;
  compass.m_min.y = running_min.y;

  button.waitForButton();
}

void loop()
{
  int heading, relHeading;
  static int targetHeading = averageHeading();

  // LSM303::vector{1,0,0} starts our heading to point North
  // Heading is given in degrees away from North, increasing clockwise
  heading = averageHeading();

  // This gives us our relative heading in respect to the angle we want to drive in
  relHeading = relativeHeading(targetHeading, heading);

  Serial.print("Target heading: ");
  Serial.print(targetHeading);
  Serial.print("    Actual heading: ");
  Serial.print(heading);
  Serial.print("    Difference: ");
  Serial.print(relHeading);

  // If we have turned to the direction we want to be pointing, go straight and then do another turn
  if((relHeading < DEVIATE_RIGHT_THRESHOLD) || (relHeading > DEVIATE_LEFT_THRESHOLD))
  {
    motors.setSpeeds(SPEED, SPEED);

    Serial.print("   Straight");

    delay(1000);

    // turn off motors and wait a short time to reduce interference from motors
    motors.setSpeeds(0, 0);
    delay(100);

    // We now want to turn 90 degrees relative to the direction we are pointing at.
    // This will help account for variable magnetic field.
    targetHeading = (averageHeading() + 90) % 360;
  }
  else
  {
    if((relHeading - DEVIATE_RIGHT_THRESHOLD) < (DEVIATE_LEFT_THRESHOLD - relHeading))
    {
      turnLeft(relHeading - DEVIATE_RIGHT_THRESHOLD);
      Serial.print("     Turn Left");
    }
    else
    {
      turnRight(DEVIATE_LEFT_THRESHOLD - relHeading);
      Serial.print("     Turn Right");
    }
  }
  Serial.println();
}

// The closer we get to our driving angle, the slower we will turn.
// We do not want to overshoot our direction.
// turnRight() takes the ideal speed and refactors it accordingly to how close
// the zumo is pointing towards the direction we want to drive.
void turnRight(int refactor)
{
  float adjust = ((float)refactor)/180.0;
  motors.setRightSpeed(-SPEED*adjust - 100);
  motors.setLeftSpeed(SPEED*adjust + 100);
}

// The closer we get to our driving angle, the slower we will turn.
// We do not want to overshoot our direction.
// turnLeft() takes the ideal speed and refactors it accordingly to how close
// the zumo is pointing towards the direction we want to drive.
void turnLeft(int refactor)
{
  float adjust = ((float)refactor)/180.0;
  motors.setRightSpeed(SPEED*adjust + 100);
  motors.setLeftSpeed(-SPEED*adjust - 100);
}

// Converts x and y components of a vector to a heading in degrees
int heading(LSM303::vector v)
{
  float xScaled =  2.0*(float)(v.x - compass.m_min.x) / ( compass.m_max.x - compass.m_min.x) - 1.0;
  float yScaled =  2.0*(float)(v.y -  compass.m_min.y) / (compass.m_max.y - compass.m_min.y) - 1.0;

  int angle = round(atan2(yScaled, xScaled)*180 / M_PI);
  if (angle < 0)
    angle += 360;
  return angle;
}

// Yields the angle difference in degrees between two headings
int relativeHeading(int headingFrom, int headingTo)
{
  if(headingFrom < headingTo)
  {
    return headingTo - headingFrom;
  }
  else
  {
    return headingTo + (360 - headingFrom);
  }
}

// We average 10 vectors to get a better measurement. The motors cause too much (magnetic) noise.
int averageHeading()
{
  LSM303::vector avg = {0, 0, 0};

  for(int i = 0; i < 10; i ++)
  {
    compass.read();
    avg.x += compass.m.x;
    avg.y += compass.m.y;
  }
  avg.x /= 10.0;
  avg.y /= 10.0;

  // avg is the average measure of the north vector.
  return heading(avg);
}