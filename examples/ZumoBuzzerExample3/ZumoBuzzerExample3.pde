#include <OrangutanLCD.h>
#include <OrangutanLEDs.h>
#include <OrangutanBuzzer.h>

/*
 * OrangutanBuzzerExample3: for the Orangutan LV-168, Orangutan SV-xx8,
 *    or 3pi robot
 *
 * This example uses the OrangutanBuzzer library to play a series of notes on
 * the target's piezo buzzer.
 *
 * This example demonstrates the use of the OrangutanBuzzer::playMode()
 * and OrangutanBuzzer::playCheck() methods, which allow you to select
 * whether the melody sequence initiated by OrangutanBuzzer::play() is
 * played automatically in the background by the Timer1 interrupt, or if
 * the play is driven by the playCheck() method in your main loop.
 *
 * Automatic play mode should be used if your code has a lot of delays
 * and is not time critical.  In this example, automatic mode is used
 * to allow the melody to keep playing while we blink the red user LED.
 *
 * Play-check mode should be used during parts of your code that are
 * time critical.  In automatic mode, the Timer1 interrupt is very slow
 * when it loads the next note, and this can delay the execution of your.
 * Using play-check mode allows you to control when the next note is
 * loaded so that it doesn't occur in the middle of some time-sensitive
 * measurement.  In our example we use play-check mode to keep the melody
 * going while performing timing measurements using Timer2.  After the
 * measurements, the maximum time measured is displayed on the LCD.
 *
 * Immediately below are three #define statements that allow you to alter
 * the way this program runs.  You should have one of the three lines
 * uncommented while commenting out the other two:  
 *
 * If only WORKING_CORRECTLY is uncommented, the program should run in its
 * ideal state, using automatic play mode during the LED-blinking phase
 * and using play-check mode during the timing phase.  The maximum recorded
 * time should be 20, as expected.
 *
 * If only ALWAYS_AUTOMATIC is uncommented, the program will use automatic
 * play mode during both the LED-blinking phase and the timing phase.  Here
 * you will see the effect this has on the time measurements (instead of 20,
 * you should see a maximum reading of around 27 or 28).
 *
 * If only ALWAYS_CHECK is uncommented, the program will be in play-check
 * mode during both the LED-blinking phase and the timing phase.  Here you
 * will see the effect that the LED-blinking delays have on play-check
 * mode (the sequence will be very choppy while the LED is blinking, but
 * sound normal during the timing phase).  The maximum timing reading should
 * be 20, as expected.
 *
 * http://www.pololu.com/docs/0J17/5.b
 * http://www.pololu.com
 * http://forum.pololu.com
 */
 
// *** UNCOMMENT ONE OF THE FOLLOWING PRECOMPILER DIRECTIVES ***
// (the remaining two should be commented out)
#define WORKING_CORRECTLY    // this is the right way to use playMode()
//#define ALWAYS_AUTOMATIC   // playMode() is always PLAY_AUTOMATIC (timing is inaccurate)
//#define ALWAYS_CHECK       // playMode() is always PLAY_CHECK (delays interrupt the sequence)

OrangutanLEDs leds;
OrangutanBuzzer buzzer;
OrangutanLCD lcd;

#include <avr/pgmspace.h>
const char rhapsody[] PROGMEM = "O6 T40 L16 d#<b<f#<d#<f#<bd#f#"
  "T80 c#<b-<f#<c#<f#<b-c#8"
  "T180 d#b<f#d#f#>bd#f#c#b-<f#c#f#>b-c#8 c>c#<c#>c#<b>c#<c#>c#c>c#<c#>c#<b>c#<c#>c#"
  "c>c#<c#>c#<b->c#<c#>c#c>c#<c#>c#<b->c#<c#>c#"
  "c>c#<c#>c#f>c#<c#>c#c>c#<c#>c#f>c#<c#>c#"
  "c>c#<c#>c#f#>c#<c#>c#c>c#<c#>c#f#>c#<c#>c#d#bb-bd#bf#d#c#b-ab-c#b-f#d#";


void setup()                    // run once, when the sketch starts
{
  TCCR2A = 0;         // configure timer2 to run at 78 kHz
  TCCR2B = 0x06;      // and overflow when TCNT2 = 256 (~3 ms)
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
  lcd.gotoXY(0, 0);
  lcd.print("blink!");
  int i;
  for (i = 0; i < 8; i++)
  {
#ifdef ALWAYS_CHECK
    buzzer.playCheck();
#endif
    leds.red(HIGH);
    delay(500);
    leds.red(LOW);
    delay(500);
  }
  
  lcd.gotoXY(0, 0);
  lcd.print("timing");
  lcd.gotoXY(0, 1);
  lcd.print("        ");    // clear bottom LCD line
  // turn off automatic playing so that our time-critical code won't be interrupted by
  // the buzzer's long timer1 interrupt.  Otherwise, this interrupt could throw off our
  // timing measurements.  Instead, we will now use playCheck() to keep the sequence
  // playing in a way that won't throw off our measurements.
#ifndef ALWAYS_AUTOMATIC
  buzzer.playMode(PLAY_CHECK);
#endif
  unsigned char maxTime = 0;
  for (i = 0; i < 8000; i++)
  {
    TCNT2 = 0;
    while (TCNT2 < 20)    // time for ~250 us
      ;
    if (TCNT2 > maxTime)
      maxTime = TCNT2;    // if the elapsed time is greater than the previous max, save it
#ifndef ALWAYS_AUTOMATIC
    buzzer.playCheck();   // check if it's time to play the next note and play it if so
#endif
  }
  lcd.gotoXY(0, 1);
  lcd.print("max=");
  lcd.print((unsigned int)maxTime);
  lcd.print(' ');  // overwrite any left over characters
}
