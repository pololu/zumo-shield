#include <OrangutanLCD.h>
#include <OrangutanPushbuttons.h>
#include <OrangutanBuzzer.h>

/*
 * OrangutanBuzzerExample2: for the Orangutan SV-xx8, Orangutan LV-168,
 *    and 3pi robot
 *
 * This example uses the OrangutanBuzzer library to play a series of notes on
 * the buzzer.  It uses the OrangutanPushbuttons library to allow the user
 * select which melody plays.
 *
 * This example demonstrates the use of the OrangutanBuzzer::play() method,
 * which plays the specified melody entirely in the background, requiring
 * no further action from the user once the method is called.  The CPU
 * is then free to execute other code while the melody plays.
 *
 * http://www.pololu.com/docs/0J17/5.b
 * http://www.pololu.com
 * http://forum.pololu.com
 */

OrangutanLCD lcd;
OrangutanPushbuttons buttons;
OrangutanBuzzer buzzer;

#include <avr/pgmspace.h>  // this lets us refer to data in program space (i.e. flash)
// store this fugue in program space using the PROGMEM macro.  
// Later we will play it directly from program space, bypassing the need to load it 
// all into RAM first.
const char fugue[] PROGMEM = 
  "! O5 L16 agafaea dac+adaea fa<aa<bac#a dac#adaea f"
  "O6 dcd<b-d<ad<g d<f+d<gd<ad<b- d<dd<ed<f+d<g d<f+d<gd<ad"
  "L8 MS <b-d<b-d MLe-<ge-<g MSc<ac<a ML d<fd<f O5 MS b-gb-g"
  "ML >c#e>c#e MS afaf ML gc#gc# MS fdfd ML e<b-e<b-"
  "O6 L16ragafaea dac#adaea fa<aa<bac#a dac#adaea faeadaca"
  "<b-acadg<b-g egdgcg<b-g <ag<b-gcf<af dfcf<b-f<af"
  "<gf<af<b-e<ge c#e<b-e<ae<ge <fe<ge<ad<fd"
  "O5 e>ee>ef>df>d b->c#b->c#a>df>d e>ee>ef>df>d"
  "e>d>c#>db>d>c#b >c#agaegfe f O6 dc#dfdc#<b c#4";


void setup()                    // run once, when the sketch starts
{
  lcd.print("Press a");
  lcd.gotoXY(0, 1);
  lcd.print("button..");
}

void loop()                     // run over and over again
{
  // wait here for one of the three buttons to be pushed
  unsigned char button = buttons.waitForButton(ALL_BUTTONS);
  lcd.clear();
  
  if (button == TOP_BUTTON)
  {
    buzzer.playFromProgramSpace(fugue);

    lcd.print("Fugue!");
    lcd.gotoXY(0, 1);
    lcd.print("flash ->");
  }
  if (button == MIDDLE_BUTTON)
  {
    buzzer.play("! V8 cdefgab>cbagfedc");
    lcd.print("C Major");
    lcd.gotoXY(0, 1);
    lcd.print("RAM ->");
  }
  if (button == BOTTOM_BUTTON)
  {
    if (buzzer.isPlaying())
    {
      buzzer.stopPlaying();
      lcd.print("stopped");
    }
    else
    {
      buzzer.playNote(NOTE_A(5), 200, 15);
      lcd.print("note A5"); 
    }
  }
}
