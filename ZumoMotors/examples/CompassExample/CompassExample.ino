#include <ZumoMotors.h>
#include <Wire.h>
#include <LSM303.h>

/* This example uses ZumoMotors, Wire, and LSM303 (compass) library to 
 * drive the Zumo in squares. To do this, the Zumo first calibrates to
 * find North and East. Calibration is accomplished in setup().
 * In loop(), The driving angle then changes its offset by 90 degrees 
 * from the heading every second. Essentially, this navigates the
 * Zumo to drive in square patterns.
 *
 * It is important to note that electronics, buildings, and environment
 * may adversly affect readings from the LSM303 compass. 
 */
 
#define SPEED 200
#define DEVIATION 5
#define CALISIZE 70
#define CRA_REG_M          0x00    // magnetometer control register location
#define CRA_REG_M_220HZ    0x1C    // reg value 220 Hz update rate for magnetometer

/*	Public variables that may be used for compass are:
 *	m_min - minimum magnetic vector
 *	m_max - maximum magnetic vector
 *	m     - magnetic vector
 *	a     - acceleration vector
 */

ZumoMotors motors;
LSM303 compass;
int heading,relativeHeading;
LSM303::vector averageHeading;

// The highest possible magnetic value to read in any direction is 2047
// The lowest possible magnetic value to read in any direction is -2047
LSM303::vector running_min = {2047, 2047, 2047}, running_max = {-2048, -2048, -2048};
int drivingAngle = 0;

// deviateRightAngle, stands for deviated angle on right side of the driving angle. 
// If we are within DEVIATION degrees relative to the driving angle, then we drive straight.
int deviateRightAngle = DEVIATION;
int deviateLeftAngle = 360 - DEVIATION;

// Setup will calibrate our compass
// We calibrate by finding maximum/minimum magnetic readings
void setup() 
{  
  unsigned long index;
  Serial.begin(9600);
  Serial.println("starting calibration");
  // Initiate the Wire library and join the I2C bus as a master or slave
  Wire.begin(); 
  
  // Initiate LSM303
  compass.init();
  
  // Enables Accelerometer and Magnetometer
  compass.enableDefault(); 
  
  compass.setMagGain(compass.magGain_13);    // highest sensitivity: +/- 1.3 gauss range (this is the default)
  compass.writeMagReg(CRA_REG_M, CRA_REG_M_220HZ);  // 220 Hz compass update rate (default is 15 Hz)
  
  // To calibrate the magnetometer we must spin the Zumo to find the Max/Min
  // magnetic vectors. This information is used to find North and East.
  motors.setLeftSpeed(SPEED);
  motors.setRightSpeed(-SPEED);
  
  float min_x_avg[CALISIZE];
  float min_y_avg[CALISIZE];
  float max_x_avg[CALISIZE]; 
  float max_y_avg[CALISIZE];

  for(index = 0; index < CALISIZE; index ++)
  {
    // Read in magnetic vector m
    compass.read();
    
    running_min.x = min(running_min.x, compass.m.x);
    running_min.y = min(running_min.y, compass.m.y);
  
    running_max.x = max(running_max.x, compass.m.x);
    running_max.y = max(running_max.y, compass.m.y);
    
    min_x_avg[index] = running_min.x;
    min_y_avg[index] = running_min.y;
  
    max_x_avg[index] = running_max.x;
    max_y_avg[index] = running_max.y;
    delay(100);
    Serial.println(index);
  }
  
  Serial.println("Left loop");
  motors.setLeftSpeed(0);
  motors.setRightSpeed(0);
  //delay(2000);
  // Throw out outliers and take a better average
  running_max.x = bestAverage(max_x_avg,CALISIZE);
  running_max.y = bestAverage(max_y_avg,CALISIZE);
  running_min.x = bestAverage(min_x_avg,CALISIZE);
  running_min.y = bestAverage(min_y_avg,CALISIZE);
  
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
}

void loop() 
{  
  // LSM303::vector{1,0,0} starts our heading to point North
  // Heading is given in degrees away from North, increasing clockwise
  heading = findBestHeading();
  
  // This gives us our relative heading in respect to the angle we want to drive in
  relativeHeading = findRelativeHeading(heading);
  
  Serial.print("Driving Angle: ");
  Serial.print(drivingAngle);
  Serial.print("   Adjusted Angle: ");
  Serial.print(relativeHeading);
  Serial.print("    Heading     ");
  Serial.print(heading);
  
  // We want to take a relative heading and turn 90 degrees to make our right turn
  if((relativeHeading < deviateRightAngle) || (relativeHeading > deviateLeftAngle))
  {
    motors.setLeftSpeed(SPEED);
    motors.setRightSpeed(SPEED);
    Serial.print("   Straight");
    
    // We now want to turn 90 degrees relative to the direction we are pointing at.
    // This will help account for variable magnetic field.
    drivingAngle = (drivingAngle + 90)%360;
    delay(1000);
  } 
  else 
  {
    if((relativeHeading - deviateRightAngle)<(deviateLeftAngle - relativeHeading))
	{
      turnLeft(relativeHeading - deviateRightAngle);
      Serial.print("     Turn Left");
    }
    else
	{
      turnRight(deviateLeftAngle - relativeHeading);
      Serial.print("     Turn Right");
    }
  }
  Serial.println();
}

// bestAverage finds an overall average while taking outlier 
// measurements out of the equation for calibration
float bestAverage(float * average,int array_size)
{
  float temp_value;
  float sum = 0;
  int upper_limit = array_size-array_size/3;
  int lower_limit = array_size/3;
  // First we sort the data so we can find the middle data.
  for(int i = array_size; i > -1; i --)
  {
    for(int j = 0; j < i; j ++)
	{
      if(average[i]<average[j])
	  {
        temp_value = average[i];
        average[i]=average[j];
        average[j] = temp_value;
      }
    }
  }

  // We then exclude the outliers out of the average 
  // by only averaging up the the middle third of the data
  for(int i = lower_limit; i <upper_limit ; i ++)
  {
    sum += average[i];
  }
  return sum/((float)(upper_limit-lower_limit));
}

// The closer we get to our driving angle, the slower we will turn.
// We do not want to overshoot our direction.
// turnRight(), takes in the ideal speed and refactors it accordingly to how close
// the zumo is pointing towards the direction we want to drive.
void turnRight(int refactor)
{
  float adjust = ((float)refactor)/180.0;
  motors.setRightSpeed(-SPEED*adjust - 100);
  motors.setLeftSpeed(SPEED*adjust + 100);
}

// The closer we get to our driving angle, the slower we will turn.
// We do not want to overshoot our direction.
// turnLeft(), takes in the ideal speed and refactors it accordingly to how close
// the zumo is pointing towards the direction we want to drive.
void turnLeft(int refactor)
{
  float adjust = ((float)refactor)/180.0;
  motors.setRightSpeed(SPEED*adjust + 100);
  motors.setLeftSpeed(-SPEED*adjust - 100);
}

// Yields a relative heading in respect to our driving angle/heading
int findRelativeHeading(int heading)
{
  int relativeHeading;
  
  if(drivingAngle < heading){
    relativeHeading = heading - drivingAngle;
  }
  else
  {
    relativeHeading = heading + (360 - drivingAngle);
  }

  return relativeHeading;
}

// We average 10 vectors to get a better measurement. The motors cause too much (magnetic) noise.
int findBestHeading()
{
  averageHeading.x = 0;
  averageHeading.y = 0;
  for(int i = 0; i < 10; i ++)
  {
    compass.read();
    averageHeading.x += compass.m.x;
    averageHeading.y += compass.m.y;      
  }
  averageHeading.x /= 10.0;
  averageHeading.y /= 10.0;
  compass.m = averageHeading;
  return compass.heading((LSM303::vector){1,0,0});    
}
