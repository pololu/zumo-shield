/*! \file ZumoReflectanceSensorArray.h
 *
 * See the ZumoReflectanceSensorArray class reference for more information about
 * this library.
 *
 * \class ZumoReflectanceSensorArray ZumoReflectanceSensorArray.h
 * \brief Read from reflectance sensor array
 *
 * The ZumoReflectanceSensorArray library provides an interface for using a
 * [Zumo Reflectance Sensor Array](http://www.pololu.com/product/1419) connected
 * to a Zumo robot. The library provides access to the raw sensor values as well
 * as to high level functions including calibration and line-tracking.
 *
 * For calibration, memory is allocated using the `malloc()` function. This
 * conserves RAM: if all six sensors are calibrated with the emitters both on
 * and off, a total of 48 bytes is dedicated to storing calibration values.
 * However, for an application where only two sensors are used, and the
 * emitters are always on during reads, only 4 bytes are required.
 *
 * Internally, this library uses all standard Arduino functions such as
 * `micros()` for timing and `digitalRead()` for getting the sensor values, so
 * it should work on all Arduinos without conflicting with other libraries.
 *
 * The ZumoReflectanceSensorArray class is derived from the QTRSensorsRC class,
 * which is in turn derived from the QTRSensors base class. The QTRSensorsRC and
 * QTRSensors classes are part of the \ref QTRSensors.h "QTRSensors" library,
 * which provides more general functionality for working with reflectance
 * sensors and is included in the Zumo Shield libraries as a dependency for this
 * library.
 *
 * We recommend using the ZumoReflectanceSensorArray library instead of
 * the \ref QTRSensors.h "QTRSensors" library when programming an Arduino on a
 * Zumo. For documentation specific to the %QTRSensors library, please see its
 * [user's guide](http://www.pololu.com/docs/0J19) on Pololu's website.
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

  /*! \brief Minimal constructor.
   *
   * This version of the constructor performs no initialization. If it is used,
   * the user must call init() before using the methods in this class.
   */
  ZumoReflectanceSensorArray() {}

  /*! \brief Constructor; initializes with given emitter pin and defaults for
   *         other settings.
   *
   * \param emitterPin Pin that turns IR emitters on or off.
   *
   * This constructor calls `init(unsigned char emitterPin)` with the specified
   * emitter pin and default values for other settings.
   */
  ZumoReflectanceSensorArray(unsigned char emitterPin)
  {
    init(emitterPin);
  }

  /*! \brief Constructor; initializes with all settings as given.
   *
   * \param pins       Array of pin numbers for sensors.
   * \param numSensors Number of sensors.
   * \param timeout    Maximum duration of reflectance reading in microseconds.
   * \param emitterPin Pin that turns IR emitters on or off.
   *
   * This constructor calls `init(unsigned char * pins, unsigned char
   * numSensors, unsigned int timeout, unsigned char emitterPin)` with all
   * settings as given.
   */
  ZumoReflectanceSensorArray(unsigned char * pins, unsigned char numSensors, unsigned int timeout = 2000,
    unsigned char emitterPin = ZUMO_SENSOR_ARRAY_DEFAULT_EMITTER_PIN)
  {
    QTRSensorsRC::init(pins, numSensors, timeout, emitterPin);
  }

  /*! \brief Initializes with given emitter pin and and defaults for other
   *         settings.
   *
   * \param emitterPin Pin that turns IR emitters on or off.
   *
   * This function initializes the ZumoReflectanceSensorArray object with the
   * specified emitter pin. The other settings are set to default values: all
   * six sensors on the array are active, and a timeout of 2000 microseconds is
   * used.
   *
   * _emitterPin_ is the Arduino digital pin that controls whether the IR LEDs
   * are on or off. This pin is optional; if a valid pin is specified, the
   * emitters will only be turned on during a reading. If _emitterPin_ is not
   * specified, the emitters will be controlled with pin 2 on the Uno (and other
   * ATmega328/168 boards) or pin A4 on the Leonardo (and other ATmega32U4
   * boards). (The "LED ON" jumper on the Zumo Reflectance Sensor Array must be
   * configured correctly for this to work.) If the value `QTR_NO_EMITTER_PIN`
   * (255) is used, you can leave the emitter pin disconnected and the IR
   * emitters will always be on.
   */
  void init(unsigned char emitterPin = ZUMO_SENSOR_ARRAY_DEFAULT_EMITTER_PIN)
  {
    unsigned char sensorPins[] = { 4, A3, 11, A0, A2, 5 };
    QTRSensorsRC::init(sensorPins, sizeof(sensorPins), 2000, emitterPin);
  }

  /*! \brief Initializes with all settings as given.
   *
   * \param pins       Array of pin numbers for sensors.
   * \param numSensors Number of sensors.
   * \param timeout    Maximum duration of reflectance reading in microseconds.
   * \param emitterPin Pin that turns IR emitters on or off.
   *
   * This function initializes the ZumoReflectanceSensorArray object with all
   * settings as given.
   *
   * The array _pins_  should contain the Arduino digital pin numbers for each
   * sensor.
   *
   * _numSensors_ specifies the length of the _pins_ array (the number of
   * reflectance sensors you are using).
   *
   * _timeout_ specifies the length of time in microseconds beyond which you
   * consider the sensor reading completely black. That is to say, if the pulse
   * length for a pin exceeds _timeout_, pulse timing will stop and the reading
   * for that pin will be considered full black. It is recommended that you set
   * _timeout_ to be between 1000 and 3000 us, depending on factors like the
   * height of your sensors and ambient lighting. This allows you to shorten the
   * duration of a sensor-reading cycle while maintaining useful measurements of
   * reflectance. If _timeout_ is not specified, it defaults to 2000 us. (See
   * the [product page](http://www.pololu.com/product/1419) for the Zumo
   * Reflectance Sensor Array on Pololu's website for an overview of the
   * sensors' principle of operation.)
   *
   * _emitterPin_ is the Arduino digital pin that controls whether the IR LEDs
   * are on or off. This pin is optional; if a valid pin is specified, the
   * emitters will only be turned on during a reading. If _emitterPin_ is not
   * specified, the emitters will be controlled with pin 2 on the Uno (and other
   * ATmega328/168 boards) or pin A4 on the Leonardo (and other ATmega32U4
   * boards). (The "LED ON" jumper on the Zumo Reflectance Sensor Array must be
   * configured correctly for this to work.) If the value `QTR_NO_EMITTER_PIN`
   * (255) is used, you can leave the emitter pin disconnected and the IR
   * emitters will always be on.
   *
   * This version of `%init()` can be useful if you only want to use a subset
   * of the six reflectance sensors on the array. For example, using the
   * outermost two sensors (on pins 4 and 5 by default) is usually enough for
   * detecting the ring border in sumo competitions:
   *
   * ~~~{.ino}
   * ZumoReflectanceSensorArray reflectanceSensors;
   *
   * ...
   *
   * reflectanceSensors.init((unsigned char[]) {4, 5}, 2);
   * ~~~
   *
   * Alternatively, you can use \ref ZumoReflectanceSensorArray(unsigned char * pins, unsigned char numSensors, unsigned int timeout, unsigned char emitterPin)
   * "a different constructor" to declare and initialize the object at the same
   * time:
   *
   * ~~~{.ino}
   *
   * ZumoReflectanceSensorArray reflectanceSensors((unsigned char[]) {4, 5}, 2);
   * ~~~
   *
   */
  void init(unsigned char * pins, unsigned char numSensors, unsigned int timeout = 2000,
    unsigned char emitterPin = ZUMO_SENSOR_ARRAY_DEFAULT_EMITTER_PIN)
  {
    QTRSensorsRC::init(pins, numSensors, timeout, emitterPin);
  }
};

// documentation for inherited functions
/*!
 * \fn void QTRSensors::read(unsigned int *sensor_values, unsigned char readMode = QTR_EMITTERS_ON)
 * \brief Reads the raw sensor values into an array.
 * 
 * \fn void QTRSensors::emittersOff()
 * \brief Turns the IR LEDs off.
 * 
 * \fn void QTRSensors::emittersOn()
 * \brief Turns the IR LEDs on.
 * 
 * \fn void QTRSensors::calibrate(unsigned char readMode = QTR_EMITTERS_ON)
 * \brief Reads the sensors for calibration.
 * 
 * \fn void QTRSensors::resetCalibration()
 * \brief Resets all calibration that has been done.
 * 
 * \fn void QTRSensors::readCalibrated(unsigned int *sensor_values, unsigned char readMode = QTR_EMITTERS_ON)
 * \brief Returns sensor readings normalized to values between 0 and 1000.
 * 
 * \fn int QTRSensors::readLine(unsigned int *sensor_values, unsigned char readMode = QTR_EMITTERS_ON, unsigned char white_line = 0)
 * \brief Returns an estimated position of a line under the sensor array.
 */
 
// documentation for inherited members
/*!
 * \property unsigned int * QTRSensors::calibratedMinimumOn
 * \brief The calibrated minimum values measured for each sensor, with emitters
 *        on.
 *
 * This pointer is unallocated and set to 0 until `calibrate()` is called, and
 * then allocated to exactly the size required. Depending on the _readMode_
 * argument to `calibrate()`, only the On or Off values might be allocated, as
 * required. This variable is made public so that you can use the calibration
 * values for your own calculations and do things like saving them to EEPROM,
 * performing sanity checking, etc.
 * 
 * \property unsigned int * QTRSensors::calibratedMaximumOn
 * \brief The calibrated maximum values measured for each sensor, with emitters
 *        on.
 * \copydetails QTRSensors::calibratedMinimumOn
 * 
 * \property unsigned int * QTRSensors::calibratedMinimumOff
 * \brief The calibrated minimum values measured for each sensor, with emitters
 *        off.
 * \copydetails QTRSensors::calibratedMinimumOn
 *
 * \property unsigned int * QTRSensors::calibratedMaximumOff
 * \brief The calibrated maximum values measured for each sensor, with emitters
 *        off.
 * \copydetails QTRSensors::calibratedMinimumOn
 */

#endif