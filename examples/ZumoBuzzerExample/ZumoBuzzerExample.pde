#include <OrangutanLCD.h>
#include <OrangutanPushbuttons.h>
#include <OrangutanBuzzer.h>

/*
 * OrangutanBuzzerExample: for the Orangutan SV-xx8, Orangutan LV-168,
 *    and 3pi robot
 *
 * This example uses the OrangutanBuzzer library to play a series of notes on
 * the buzzer.  It also uses the OrangutanLCD library to display the notes it is
 * playing, and it uses the OrangutanPushbuttons library to allow the user to
 * stop/reset the melody with the top pushbutton.
 *
 * http://www.pololu.com/docs/0J17/5.b
 * http://www.pololu.com
 * http://forum.pololu.com
 */

#define MELODY_LENGTH 95

// These arrays take up a total of 285 bytes of RAM (out of a 1k limit)
unsigned char note[MELODY_LENGTH] = 
{
  NOTE_E(5), SILENT_NOTE, NOTE_E(5), SILENT_NOTE, NOTE_E(5), SILENT_NOTE, NOTE_C(5), NOTE_E(5),
  NOTE_G(5), SILENT_NOTE, NOTE_G(4), SILENT_NOTE,

  NOTE_C(5), NOTE_G(4), SILENT_NOTE, NOTE_E(4), NOTE_A(4), NOTE_B(4), NOTE_B_FLAT(4), NOTE_A(4), NOTE_G(4),
  NOTE_E(5), NOTE_G(5), NOTE_A(5), NOTE_F(5), NOTE_G(5), SILENT_NOTE, NOTE_E(5), NOTE_C(5), NOTE_D(5), NOTE_B(4),

  NOTE_C(5), NOTE_G(4), SILENT_NOTE, NOTE_E(4), NOTE_A(4), NOTE_B(4), NOTE_B_FLAT(4), NOTE_A(4), NOTE_G(4),
  NOTE_E(5), NOTE_G(5), NOTE_A(5), NOTE_F(5), NOTE_G(5), SILENT_NOTE, NOTE_E(5), NOTE_C(5), NOTE_D(5), NOTE_B(4),

  SILENT_NOTE, NOTE_G(5), NOTE_F_SHARP(5), NOTE_F(5), NOTE_D_SHARP(5), NOTE_E(5), SILENT_NOTE,
  NOTE_G_SHARP(4), NOTE_A(4), NOTE_C(5), SILENT_NOTE, NOTE_A(4), NOTE_C(5), NOTE_D(5),

  SILENT_NOTE, NOTE_G(5), NOTE_F_SHARP(5), NOTE_F(5), NOTE_D_SHARP(5), NOTE_E(5), SILENT_NOTE,
  NOTE_C(6), SILENT_NOTE, NOTE_C(6), SILENT_NOTE, NOTE_C(6),

  SILENT_NOTE, NOTE_G(5), NOTE_F_SHARP(5), NOTE_F(5), NOTE_D_SHARP(5), NOTE_E(5), SILENT_NOTE,
  NOTE_G_SHARP(4), NOTE_A(4), NOTE_C(5), SILENT_NOTE, NOTE_A(4), NOTE_C(5), NOTE_D(5),

  SILENT_NOTE, NOTE_E_FLAT(5), SILENT_NOTE, NOTE_D(5), NOTE_C(5)
};

unsigned int duration[MELODY_LENGTH] =
{
  100, 25, 125, 125, 125, 125, 125, 250, 250, 250, 250, 250,

  375, 125, 250, 375, 250, 250, 125, 250, 167, 167, 167, 250, 125, 125,
  125, 250, 125, 125, 375,

  375, 125, 250, 375, 250, 250, 125, 250, 167, 167, 167, 250, 125, 125,
  125, 250, 125, 125, 375,

  250, 125, 125, 125, 250, 125, 125, 125, 125, 125, 125, 125, 125, 125,

  250, 125, 125, 125, 250, 125, 125, 200, 50, 100, 25, 500,

  250, 125, 125, 125, 250, 125, 125, 125, 125, 125, 125, 125, 125, 125,

  250, 250, 125, 375, 500
};

OrangutanLCD lcd;
OrangutanPushbuttons buttons;
OrangutanBuzzer buzzer;
unsigned char currentIdx;

void setup()                    // run once, when the sketch starts
{
  currentIdx = 0;
  lcd.print("Music!");
}

void loop()                     // run over and over again
{
  // if we haven't finished playing the song and 
  // the buzzer is ready for the next note, play the next note
  if (currentIdx < MELODY_LENGTH && !buzzer.isPlaying())
  {
    // play note at max volume
    buzzer.playNote(note[currentIdx], duration[currentIdx], 15);
    
    // optional LCD feedback (for fun)
    lcd.gotoXY(0, 1);                           // go to start of the second LCD line
    lcd.print((unsigned int)note[currentIdx]);  // print integer value of the current note
    lcd.print("  ");                            // overwrite any left over characters
    currentIdx++;
  }

  // Insert some other useful code here...
  // the melody will play normally while the rest of your code executes
  // as long as it executes quickly enough to keep from inserting delays
  // between the notes.
  
  // For example, let the top user pushbutton function as a stop/reset melody button
  if (buttons.isPressed(TOP_BUTTON))
  {
    buzzer.stopPlaying(); // silence the buzzer
    if (currentIdx < MELODY_LENGTH)
      currentIdx = MELODY_LENGTH;        // terminate the melody
    else
      currentIdx = 0;                    // restart the melody
    buttons.waitForRelease(TOP_BUTTON);  // wait here for the button to be released
  }
}
