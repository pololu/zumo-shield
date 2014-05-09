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
 * ### Calibration ###
 *
 * This library allows you to use the `calibrate()` method to easily calibrate
 * your sensors for the particular conditions it will encounter. Calibrating
 * your sensors can lead to substantially more reliable sensor readings, which
 * in turn can help simplify your code. As such, we recommend you build a
 * calibration phase into your Zumo's initialization routine. This can be as
 * simple as a fixed duration over which you repeatedly call the `calibrate()`
 * method.
 *
 * During this calibration phase, you will need to expose each of your
 * reflectance sensors to the lightest and darkest readings they will encounter.
 * For example, if your Zumo is programmed to be a line follower, you will want
 * to slide it across the line during the calibration phase so the each sensor
 * can get a reading of how dark the line is and how light the ground is (or you
 * can program it to automatically turn back and forth to pass all of the
 * sensors over the line). The **SensorCalibration** example included with this
 * library demonstrates a calibration routine.
 *
 * ### Reading the sensors
 *
 *
 * This library gives you a number of different ways to read the sensors.
 *
 * - You can request raw sensor values using the `read()` method.
 *
 * - You can request calibrated sensor values using the `readCalibrated()`
 *   method. Calibrated sensor values will always range from 0 to 1000, with the
 *   extreme values corresponding to the most and least reflective surfaces
 *   encountered during calibration.
 *
 * - For line-detection applications, you can request the line location using
 *   the `readLine()` method. This function provides calibrated values
 *   for each sensor and returns an integer that tells you where it thinks the
 *   line is.
 *
 * ### Class Inheritance ###
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
   * \a emitterPin is the Arduino digital pin that controls whether the IR LEDs
   * are on or off. This pin is optional; if a valid pin is specified, the
   * emitters will only be turned on during a reading. If \a emitterPin is not
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
   * The array \a pins should contain the Arduino digital pin numbers for each
   * sensor.
   *
   * \a numSensors specifies the length of the \a pins array (the number of
   * reflectance sensors you are using).
   *
   * \a timeout specifies the length of time in microseconds beyond which you
   * consider the sensor reading completely black. That is to say, if the pulse
   * length for a pin exceeds \a timeout, pulse timing will stop and the reading
   * for that pin will be considered full black. It is recommended that you set
   * \a timeout to be between 1000 and 3000 us, depending on factors like the
   * height of your sensors and ambient lighting. This allows you to shorten the
   * duration of a sensor-reading cycle while maintaining useful measurements of
   * reflectance. If \a timeout is not specified, it defaults to 2000 us. (See
   * the [product page](http://www.pololu.com/product/1419) for the Zumo
   * Reflectance Sensor Array on Pololu's website for an overview of the
   * sensors' principle of operation.)
   *
   * \a emitterPin is the Arduino digital pin that controls whether the IR LEDs
   * are on or off. This pin is optional; if a valid pin is specified, the
   * emitters will only be turned on during a reading. If \a emitterPin is not
   * specified, the emitters will be controlled with pin 2 on the Uno (and other
   * ATmega328/168 boards) or pin A4 on the Leonardo (and other ATmega32U4
   * boards). (The corresponding connection should be made with the "LED ON"
   * jumper on the Zumo Reflectance Sensor Array.) If the value
   * `QTR_NO_EMITTER_PIN` (255) is used, you can leave the emitter pin
   * disconnected and the IR emitters will always be on.
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

/*! \fn void QTRSensors::read(unsigned int *sensor_values, unsigned char readMode = QTR_EMITTERS_ON)
\memberof ZumoReflectanceSensorArray
 * \brief Reads the raw sensor values into an array.
 *
 * \param sensorValues Array to populate with sensor readings.
 * \param readMode     Read mode (`QTR_EMITTERS_OFF`, `QTR_EMITTERS_ON`, or
 *                     `QTR_EMITTERS_ON_AND_OFF`).
 *
 * There **must** be space in the \a sensorValues array for as many values as
 * there were sensors specified in the constructor. The values returned are
 * measures of the reflectance in units of microseconds. They will be raw
 * readings between 0 and the \a timeout argument (in units of microseconds)
 * provided in the constructor (which defaults to 2000).
 *
 * The \a readMode argument specifies the kind of read that will be performed.
 * Several options are defined:
 *
 * - `QTR_EMITTERS_OFF` specifies that the reading should be made without
 *   turning on the infrared (IR) emitters, in which case the reading represents
 *   ambient light levels near the sensor.
 * - `QTR_EMITTERS_ON` specifies that the emitters should be turned on for the
 *   reading, which results in a measure of reflectance.
 * - `QTR_EMITTERS_ON_AND_OFF` specifies that a reading should be made in both
 *   the on and off states. The values returned when this option is used are
 *   given by the formula **on + max &minus; off**, where **on** is the reading
 *   with the emitters on, **off** is the reading with the emitters off, and
 *   **max** is the maximum sensor reading. This option can reduce the amount of
 *   interference from uneven ambient lighting.
 *
 * Note that emitter control will only work if you specify a valid emitter pin
 * in the constructor and make the corresponding connection (with the "LED ON"
 * jumper or otherwise).
 *
 * The ZumoReflectanceSensorArray class inherits this function from the
 * QTRSensors class.
 */

/*! \fn void QTRSensors::emittersOff()
 * \brief Turns the IR LEDs off.
 *
 * This is mainly for use by the `read()` method, and calling this function
 * before or after reading the sensors will have no effect on the readings, but
 * you might wish to use it for testing purposes. This method will only do
 * something if the emitter pin specified in the constructor is valid (i.e. not
 * `QTR_NO_EMITTER_PIN`) and the corresponding connection is made.
 *
 * The ZumoReflectanceSensorArray class inherits this function from the
 * QTRSensors class.
 */

/*! \fn void QTRSensors::emittersOn()
 * \brief Turns the IR LEDs on.
 * \copydetails emittersOff
 */

/*! \fn void QTRSensors::calibrate(unsigned char readMode = QTR_EMITTERS_ON)
 * \brief Reads the sensors for calibration.
 *
 * \param readMode     Read mode (`QTR_EMITTERS_OFF`, `QTR_EMITTERS_ON`, or
 *                     `QTR_EMITTERS_ON_AND_OFF`).
 *
 * The sensor values read by this function are not returned; instead, the
 * maximum and minimum values found over time are stored internally and used for
 * the `readCalibrated()` method. You can access the calibration (i.e raw max
 * and min sensor readings) through the public member pointers
 * `calibratedMinimumOn`, `calibratedMaximumOn`, `calibratedMinimumOff`, and
 * `calibratedMaximumOff`. Note that these pointers will point to arrays of
 * length \a numSensors, as specified in the constructor, and they will only be
 * allocated **after** `calibrate()` has been called. If you only calibrate with
 * the emitters on, the calibration arrays that hold the off values will not be
 * allocated.
 *
 * The ZumoReflectanceSensorArray class inherits this function from the
 * QTRSensors class.
 */

/*! \fn void QTRSensors::resetCalibration()
 * \brief Resets all calibration that has been done.
 *
 * This function discards the calibration values that have been previously
 * recorded, resetting the min and max values.
 *
 * The ZumoReflectanceSensorArray class inherits this function from the
 * QTRSensors class.
 */

/*! \fn void QTRSensors::readCalibrated(unsigned int *sensor_values, unsigned char readMode = QTR_EMITTERS_ON)
 * \brief Returns sensor readings normalized to values between 0 and 1000.
 *
 * \param sensorValues Array to populate with sensor readings.
 * \param readMode     Read mode (`QTR_EMITTERS_OFF`, `QTR_EMITTERS_ON`, or
 *                     `QTR_EMITTERS_ON_AND_OFF`).
 *
 * 0 corresponds to a reading that is less than or equal to the minimum value
 * read by `calibrate()` and 1000 corresponds to a reading that is greater than
 * or equal to the maximum value. Calibration values are stored separately for
 * each sensor, so that differences in the sensors are accounted for
 * automatically.
 *
 * The ZumoReflectanceSensorArray class inherits this function from the
 * QTRSensors class.
 */

/*! \fn int QTRSensors::readLine(unsigned int *sensor_values, unsigned char readMode = QTR_EMITTERS_ON, unsigned char whiteLine = 0)
 * \brief Returns an estimated position of a line under the sensor array.
 *
 * \param sensorValues Array to populate with sensor readings.
 * \param readMode     Read mode (`QTR_EMITTERS_OFF`, `QTR_EMITTERS_ON`, or
 *                     `QTR_EMITTERS_ON_AND_OFF`).
 * \param whiteLine   0 to detect a dark line on a light surface; 1 to detect
 *                     a light line on a dark surface.
 * \return An estimated line position.
 *
 * This function operates the same as `readCalibrated()`, but with a feature
 * designed for line following: it returns an estimated position of the line.
 * The estimate is made using a weighted average of the sensor indices
 * multiplied by 1000, so that a return value of 0 indicates that the line is
 * directly below sensor 0 (or was last seen by sensor 0 before being lost), a
 * return value of 1000 indicates that the line is directly below sensor 1, 2000
 * indicates that it's below sensor 2, etc. Intermediate values indicate that
 * the line is between two sensors. The formula is:
 *
 * \f[
 *   \newcommand{sv}[1]{\mathtt{sensorValues[#1]}}
 *   \text{return value} =
 *     \frac{(0 \times \sv{0}) + (1000 \times \sv{1}) + (2000 \times \sv{2}) + \ldots}
 *          {\sv{0} + \sv{1} + \sv{2} + \ldots}
 * \f]
 *
 * As long as your sensors aren't spaced too far apart relative to the line,
 * this returned value is designed to be monotonic, which makes it great for use
 * in closed-loop PID control. Additionally, this method remembers where it last
 * saw the line, so if you ever lose the line to the left or the right, its line
 * position will continue to indicate the direction you need to go to reacquire
 * the line. For example, if sensor 5 is your rightmost sensor and you end up
 * completely off the line to the left, this function will continue to return
 * 5000.
 *
 * By default, this function assumes a dark line (high values) on a light
 * background (low values). If your line is light on dark, set the optional
 * second argument \a whiteLine to true. In this case, each sensor value will be
 * replaced by the maximum possible value minus its actual value before the
 * averaging.
 *
 * The ZumoReflectanceSensorArray class inherits this function from the
 * QTRSensors class.
 */

 
// documentation for inherited member variables

/*!
 * \property unsigned int * QTRSensors::calibratedMinimumOn
 * \brief The calibrated minimum values measured for each sensor, with emitters
 *        on.
 *
 * This pointer is unallocated and set to 0 until `calibrate()` is called, and
 * then allocated to exactly the size required. Depending on the \a readMode
 * argument to `calibrate()`, only the On or Off values might be allocated, as
 * required. This variable is made public so that you can use the calibration
 * values for your own calculations and do things like saving them to EEPROM,
 * performing sanity checking, etc.
 *
 * The ZumoReflectanceSensorArray class inherits this variable from the
 * QTRSensors class.
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