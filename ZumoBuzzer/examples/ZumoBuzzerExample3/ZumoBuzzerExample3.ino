#include <ZumoBuzzer.h>
#include <Pushbutton.h>

/*
 * This example uses the ZumoBuzzer library to play a series of notes on
 * the Zumo Shield's piezo buzzer.
 *
 * This example demonstrates the use of the ZumoBuzzer::playMode()
 * and ZumoBuzzer::playCheck() methods, which allow you to select
 * whether the melody sequence initiated by ZumoBuzzer::play() is
 * played automatically in the background by the timer interrupt, or if
 * the play is driven by the playCheck() method in your main loop. The
 * melody begins playing after the user pushbutton on the Zumo is pressed
 * and released.
 *
 * Automatic play mode should be used if your code has a lot of delays
 * and is not time critical.  In this example, automatic mode is used
 * to allow the melody to keep playing while we blink the yellow user LED.
 *
 * Play-check mode should be used during parts of your code that are
 * time critical.  In automatic mode, the timer interrupt is very slow
 * when it loads the next note, and this can delay the execution of your code.
 * Using play-check mode allows you to control when the next note is
 * loaded so that it doesn't occur in the middle of some time-sensitive
 * measurement.  In our example we use play-check mode to keep the melody
 * going while performing timing measurements using Arduino's micros()
 * function.  After the measurements, the maximum time measured is printed
 * over the serial connection.
 *
 * Immediately below are three #define statements that allow you to alter
 * the way this program runs.  You should have one of the three lines
 * uncommented while commenting out the other two:  
 *
 * If only WORKING_CORRECTLY is uncommented, the program should run in its
 * ideal state, using automatic play mode during the LED-blinking phase
 * and using play-check mode during the timing phase.  The maximum recorded
 * time should be close to 250, as expected.
 *
 * If only ALWAYS_AUTOMATIC is uncommented, the program will use automatic
 * play mode during both the LED-blinking phase and the timing phase.  Here
 * you will see the effect this has on the time measurements (instead of 250,
 * you should see a maximum reading as high as 600).
 *
 * If only ALWAYS_CHECK is uncommented, the program will be in play-check
 * mode during both the LED-blinking phase and the timing phase.  Here you
 * will see the effect that the LED-blinking delays have on play-check
 * mode (the sequence will be very choppy while the LED is blinking, but
 * sound normal during the timing phase).  The maximum timing reading should
 * be close to 250, as expected.
 */
 
// *** UNCOMMENT ONE OF THE FOLLOWING PRECOMPILER DIRECTIVES ***
// (the remaining two should be commented out)
#define WORKING_CORRECTLY    // this is the right way to use playMode()
//#define ALWAYS_AUTOMATIC   // playMode() is always PLAY_AUTOMATIC (timing is inaccurate)
//#define ALWAYS_CHECK       // playMode() is always PLAY_CHECK (delays interrupt the sequence)

#define LED_PIN 13

ZumoBuzzer buzzer;
Pushbutton button(ZUMO_BUTTON);

#include <avr/pgmspace.h>
const char rhapsody[] PROGMEM = "O6 T40 L16 d#<b<f#<d#<f#<bd#f#"
  "T80 c#<b-<f#<c#<f#<b-c#8"
  "T180 d#b<f#d#f#>bd#f#c#b-<f#c#f#>b-c#8 c>c#<c#>c#<b>c#<c#>c#c>c#<c#>c#<b>c#<c#>c#"
  "c>c#<c#>c#<b->c#<c#>c#c>c#<c#>c#<b->c#<c#>c#"
  "c>c#<c#>c#f>c#<c#>c#c>c#<c#>c#f>c#<c#>c#"
  "c>c#<c#>c#f#>c#<c#>c#c>c#<c#>c#f#>c#<c#>c#d#bb-bd#bf#d#c#b-ab-c#b-f#d#";


void setup()                    // run once, when the sketch starts
{
  Serial.begin(115200);
  while (!Serial); // wait for serial port to connect (needed for Leonardo)
  Serial.println("Press button to start...");
  button.waitForButton();
  pinMode(LED_PIN, OUTPUT);
  buzzer.playFromProgramSpace(rhapsody);
}

void loop()                     // run over and over again
{
  // allow the sequence to keep playing automatically through the following delays
#ifndef ALWAYS_CHECK
  buzzer.playMode(PLAY_AUTOMATIC);
#else
  buzzer.playMode(PLAY_CHECK);
#endif
  Serial.println("blink!");
  int i;
  for (i = 0; i < 8; i++)
  {
#ifdef ALWAYS_CHECK
    buzzer.playCheck();
#endif
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
  }
  
  Serial.println("timing...");
  // turn off automatic playing so that our time-critical code won't be interrupted by
  // the buzzer's long timer1 interrupt.  Otherwise, this interrupt could throw off our
  // timing measurements.  Instead, we will now use playCheck() to keep the sequence
  // playing in a way that won't throw off our measurements.
#ifndef ALWAYS_AUTOMATIC
  buzzer.playMode(PLAY_CHECK);
#endif
  unsigned int maxTime = 0;
  unsigned long startTimeMicros;
  unsigned int elapsed;
  for (i = 0; i < 8000; i++)
  {
    startTimeMicros = micros();
    elapsed = 0;
    
    while (elapsed < 250)    // time for ~250 us
      elapsed = micros() - startTimeMicros;
    if (elapsed > maxTime)
      maxTime = elapsed;    // if the elapsed time is greater than the previous max, save it
#ifndef ALWAYS_AUTOMATIC
    buzzer.playCheck();   // check if it's time to play the next note and play it if so
#endif
  }
  Serial.print("max elapsed = ");
  Serial.print(maxTime);
  Serial.println(" us");
}
