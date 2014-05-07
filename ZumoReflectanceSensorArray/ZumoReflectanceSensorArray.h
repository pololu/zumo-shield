/*! \file ZumoReflectanceSensorArray.h
 *
 * See the ZumoReflectanceSensorArray class reference for more information about
 * this library.
 *
 * \class ZumoReflectanceSensorArray ZumoReflectanceSensorArray.h
 * \brief Read from reflectance sensor array
 */
 
#ifndef ZumoReflectanceSensorArray_h
#define ZumoReflectanceSensorArray_h

#include <../QTRSensors/QTRSensors.h>
#include <Arduino.h>

#if defined(__AVR_ATmega32U4__)
  // Arduino Leonardo
  #define ZUMO_SENSOR_ARRAY_DEFAULT_EMITTER_PIN  A4
#else
  // Arduino UNO and other ATmega328P/168 Arduinos
  #define ZUMO_SENSOR_ARRAY_DEFAULT_EMITTER_PIN  2  
#endif
 
class ZumoReflectanceSensorArray : public QTRSensorsRC
{
  public:
  
  // if this constructor is used, the user must call init() before using
  // the methods in this class
  ZumoReflectanceSensorArray() {}
  
  // this constructor calls init with the given emitter pin and default values for other settings
  ZumoReflectanceSensorArray(unsigned char emitterPin)
  {
    init(emitterPin);
  }
  
  // this constructor calls init with all settings as given
  ZumoReflectanceSensorArray(unsigned char * pins, unsigned char numSensors, unsigned int timeout = 2000,
    unsigned char emitterPin = ZUMO_SENSOR_ARRAY_DEFAULT_EMITTER_PIN)
  {
    QTRSensorsRC::init(pins, numSensors, timeout, emitterPin);
  }
  
  
  /* To disable the emitter control feature, specify QTR_NO_EMITTER_PIN for the first argument. */
  void init(unsigned char emitterPin = ZUMO_SENSOR_ARRAY_DEFAULT_EMITTER_PIN)
  {
    unsigned char sensorPins[] = { 4, A3, 11, A0, A2, 5 };
    QTRSensorsRC::init(sensorPins, sizeof(sensorPins), 2000, emitterPin);
  }
  
  void init(unsigned char * pins, unsigned char numSensors, unsigned int timeout = 2000,
    unsigned char emitterPin = ZUMO_SENSOR_ARRAY_DEFAULT_EMITTER_PIN)
  {
    QTRSensorsRC::init(pins, numSensors, timeout, emitterPin);
  }
};

// documentation for inherited functions
/*!
\fn void QTRSensors::read(unsigned int *sensor_values, unsigned char readMode = QTR_EMITTERS_ON)
description

\fn void QTRSensors::emittersOff()
description
 
\fn void QTRSensors::emittersOn()
description
 
\fn void QTRSensors::calibrate(unsigned char readMode = QTR_EMITTERS_ON)
description
 
\fn void QTRSensors::resetCalibration()
description
 
\fn void QTRSensors::readCalibrated(unsigned int *sensor_values, unsigned char readMode = QTR_EMITTERS_ON)
description
 
\fn int QTRSensors::readLine(unsigned int *sensor_values, unsigned char readMode = QTR_EMITTERS_ON, unsigned char white_line = 0)
description

\property unsigned int * QTRSensors::calibratedMinimumOn
description

\property unsigned int * QTRSensors::calibratedMaximumOn
description

\property unsigned int * QTRSensors::calibratedMinimumOff
description

\property unsigned int * QTRSensors::calibratedMaximumOff
description
*/

#endif