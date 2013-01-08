#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>

/*
This example is designed to be run an Arduino that is connected to a Zumo
Reflectance Sensor Array through a Zumo Shield.

The setup phase of this example calibrates the sensor for ten seconds and 
turns on the pin 13 LED while calibration is going on.  During this phase, 
you should expose each reflectance sensor to the lightest and darkest 
readings they will encounter.  For example, if you are making a line 
follower, you should slide the sensors across the line during the 
calibration phase so that each sensor can get a reading of how dark the 
line is and how light the ground is.  Improper calibration will result in 
poor readings. If you want to skip the calibration phase, you can get the 
raw sensor readings (pulse times from 0 to 2500 us) by calling 
reflectance.read(sensorValues) instead of 
reflectance.readLine(sensorValues).

The loop function reads the calibrated sensor values.  It also uses the 
values to estimate the position of a line, in case you are using the Zumo 
for line following. It prints the sensor values to the serial monitor as 
numbers from 0 (maximum reflectance) to 1000 (minimum reflectance) followed 
by the estimated location of the line as a number from 0 to 5000.  A line 
position of 1000 means the line is directly under sensor 1, 2000 means 
directly under sensor 2, etc.  0 means the line is directly under sensor 0 
or was last seen by sensor 0 before being lost.  5000 means the line is 
directly under sensor 5 or was last seen by sensor 5 before being lost.
*/

ZumoReflectanceSensorArray reflectanceSensors;

// Define an array for holding sensor values.
#define NUM_SENSORS 6
unsigned int sensorValues[NUM_SENSORS];

void setup()
{
  reflectanceSensors.init();

  delay(500);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);        // turn on LED to indicate we are in calibration mode
  
  unsigned long startTime = millis();
  while(millis() - startTime < 10000)   // make the calibration take 10 seconds
  {
    reflectanceSensors.calibrate();
  }
  digitalWrite(13, LOW);         // turn off LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
  for (byte i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(reflectanceSensors.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  
  // print the calibration maximum values measured when emitters were on
  for (byte i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(reflectanceSensors.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
}

void loop()
{
  // read calibrated sensor values and obtain a measure of the line position.
  // Note: the values returned will be incorrect if the sensors have not been properly
  // calibrated during the calibration phase.
  unsigned int position = reflectanceSensors.readLine(sensorValues);

  // To get raw sensor values instead, call:  
  //reflectanceSensors.read(sensorValues);

  for (byte i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print(' ');
  }
  Serial.print("    ");
  Serial.println(position);
  
  delay(250);
}
