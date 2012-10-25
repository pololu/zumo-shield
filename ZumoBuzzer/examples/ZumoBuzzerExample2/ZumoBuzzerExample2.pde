#include <ZumoBuzzer.h>
#include <Pushbutton.h>

/*
 * This example uses the ZumoBuzzer library to play a series of notes on
 * the buzzer.  It uses the Pushbutton library to allow the user to
 * select which melody plays.
 *
 * This example demonstrates the use of the ZumoBuzzer::play() method,
 * which plays the specified melody entirely in the background, requiring
 * no further action from the user once the method is called.  The CPU
 * is then free to execute other code while the melody plays.
 */

ZumoBuzzer buzzer;
Pushbutton button(ZUMO_BUTTON);

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
}

void loop()                     // run over and over again
{
  static unsigned char mode = 0;
  
  // wait here for the button to be pushed
  button.waitForButton();
  
  // perform an action depending on the current mode
  switch (mode)
  {      
    case 0:
      // play fugue from flash (program space)
      buzzer.playFromProgramSpace(fugue);
      break;

    case 1:
      // play scale from RAM
      buzzer.play("! V10 cdefgab>cbagfedc");
      break;
      
    case 2:
      // stop any currently playing melody
      buzzer.stopPlaying();
      // play note A5
      buzzer.playNote(NOTE_A(5), 200, 15);
      break;
  }
  
  // increment mode and wrap around
  if (++mode > 2)
    mode = 0;
}
