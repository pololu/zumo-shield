/*! \file ZumoBuzzer.h
 *
 * See the ZumoBuzzer class reference for more information about this library.
 *
 * \class ZumoBuzzer ZumoBuzzer.h
 * \brief Play beeps and music with buzzer
 *
 * The ZumoBuzzer library allows various sounds to be played through the buzzer
 * on the Zumo Shield, from simple beeps to complex tunes. The buzzer is
 * controlled using a PWM output of Timer 2 (on the Arduino Uno and other
 * ATmega328/168 boards) or Timer 4 (on the Arduino Leonardo and other
 * ATmega32U4 boards), so it will conflict with any other uses of that timer.
 *
 * Note durations are timed using a timer overflow interrupt
 * (`TIMER2_OVF`/`TIMER4_OVF`), which will briefly interrupt execution of your
 * main program at the frequency of the sound being played. In most cases, the
 * interrupt-handling routine is very short (several microseconds). However,
 * when playing a sequence of notes in `PLAY_AUTOMATIC` mode (the default mode)
 * with the `play()` command, this interrupt takes much longer than normal
 * (perhaps several hundred microseconds) every time it starts a new note. It is
 * important to take this into account when writing timing-critical code.
 *
 * The ZumoBuzzer library is fully compatible with the OrangutanBuzzer functions
 * in the [Pololu AVR C/C++ Library](http://www.pololu.com/docs/0J18), so any
 * sequences and melodies written for OrangutanBuzzer functions will also work
 * with the equivalent ZumoBuzzer functions.
 */

#ifndef ZumoBuzzer_h
#define ZumoBuzzer_h

#define PLAY_AUTOMATIC 0
#define PLAY_CHECK     1

//                                             n
// Equal Tempered Scale is given by f  = f  * a
//                                   n    o
//
//  where f  is chosen as A above middle C (A4) at f  = 440 Hz
//         o                                        o
//  and a is given by the twelfth root of 2 (~1.059463094359)

/*! \anchor note_macros
 *
 * \name Note Macros
 * \a x specifies the octave of the note
 * @{
 */
#define NOTE_C(x)       ( 0 + (x)*12)
#define NOTE_C_SHARP(x) ( 1 + (x)*12)
#define NOTE_D_FLAT(x)  ( 1 + (x)*12)
#define NOTE_D(x)       ( 2 + (x)*12)
#define NOTE_D_SHARP(x) ( 3 + (x)*12)
#define NOTE_E_FLAT(x)  ( 3 + (x)*12)
#define NOTE_E(x)       ( 4 + (x)*12)
#define NOTE_F(x)       ( 5 + (x)*12)
#define NOTE_F_SHARP(x) ( 6 + (x)*12)
#define NOTE_G_FLAT(x)  ( 6 + (x)*12)
#define NOTE_G(x)       ( 7 + (x)*12)
#define NOTE_G_SHARP(x) ( 8 + (x)*12)
#define NOTE_A_FLAT(x)  ( 8 + (x)*12)
#define NOTE_A(x)       ( 9 + (x)*12)
#define NOTE_A_SHARP(x) (10 + (x)*12)
#define NOTE_B_FLAT(x)  (10 + (x)*12)
#define NOTE_B(x)       (11 + (x)*12)

/*! \brief silences buzzer for the note duration */
#define SILENT_NOTE   0xFF

/*! \brief frequency bit that indicates Hz/10<br>
 * e.g. \a frequency = `(445 | DIV_BY_10)` gives a frequency of 44.5 Hz
 */
#define DIV_BY_10     (1 << 15)
/*! @} */

class ZumoBuzzer
{
  public:

    // constructor
  ZumoBuzzer();

  /*! \brief Plays the specified frequency for the specified duration.
   *
   * \param freq     Frequency to play in Hz (or 0.1 Hz if the `DIV_BY_10` bit
   *                 is set).
   * \param duration Duration of the note in milliseconds.
   * \param volume   Volume of the note (0--15).
   *
   * The \a frequency argument must be between 40 Hz and 10 kHz. If the most
   * significant bit of \a frequency is set, the frequency played is the value
   * of the lower 15 bits of \a frequency in units of 0.1 Hz. Therefore, you can
   * play a frequency of 44.5 Hz by using a \a frequency of `(DIV_BY_10 | 445)`.
   * If the most significant bit of \a frequency is not set, the units for
   * frequency are Hz. The \a volume argument controls the buzzer volume, with
   * 15 being the loudest and 0 being the quietest. A \a volume of 15 supplies 
   * the buzzer with a 50% duty cycle PWM at the specified \a frequency.
   * Lowering \a volume by one halves the duty cycle (so 14 gives a 25% duty
   * cycle, 13 gives a 12.5% duty cycle, etc). The volume control is somewhat 
   * crude (especially on the ATmega328/168) and should be thought of as a bonus
   * feature.
   *
   * This function plays the note in the background while your program continues
   * to execute. If you call another buzzer function while the note is playing,
   * the new function call will overwrite the previous and take control of the
   * buzzer. If you want to string notes together, you should either use the
   * `play()` function or put an appropriate delay after you start a note
   * playing. You can use the `is_playing()` function to figure out when the
   * buzzer is through playing its note or melody.
   *
   * ### Example ###
   *
   * ~~~{.ino}
   * ZumoBuzzer buzzer;
   *
   * ...
   *
   * // play a 6 kHz note for 250 ms at a lower volume
   * buzzer.playFrequency(6000, 250, 12);
   *
   * // wait for buzzer to finish playing the note
   * while (buzzer.isPlaying());
   *
   * // play a 44.5 Hz note for 1 s at full volume
   * buzzer.playFrequency(DIV_BY_10 | 445, 1000, 15);
   * ~~~
   *
   * \warning \a frequency &times; \a duration / 1000 must be no greater than
     0xFFFF (65535). This means you can't use a max duration of 65535 ms for
     frequencies greater than 1 kHz. For example, the maximum duration you can
     use for a frequency of 10 kHz is 6553 ms. If you use a duration longer than
     this, you will produce an integer overflow that can result in unexpected
     behavior.
   */
  static void playFrequency(unsigned int freq, unsigned int duration,
                unsigned char volume);

  /*! \brief Plays the specified note for the specified duration.
   *
   *  \param note     Note to play (see \ref note_macros "Note Macros").
   *  \param duration Duration of the note in milliseconds.
   *  \param volume   Volume of the note (0--15).
   *
   * The \a note argument is an enumeration for the notes of the equal tempered
   * scale (ETS). See \ref note_macros "Note Macros" for more information. The
   * \a volume argument controls the buzzer volume, with 15 being the loudest 
   * and 0 being the quietest. A \a volume of 15 supplies the buzzer with a 50% 
   * duty cycle PWM at the specified \a frequency. Lowering \a volume by one 
   * halves the duty cycle (so 14 gives a 25% duty cycle, 13 gives a 12.5% duty 
   * cycle, etc). The volume control is somewhat crude (especially on the
   * ATmega328/168) and should be thought of as a bonus feature.
   *
   * This function plays the note in the background while your program continues
   * to execute. If you call another buzzer function while the note is playing,
   * the new function call will overwrite the previous and take control of the
   * buzzer. If you want to string notes together, you should either use the
   * `play()` function or put an appropriate delay after you start a note
   * playing. You can use the `is_playing()` function to figure out when the
   * buzzer is through playing its note or melody.
   */
  static void playNote(unsigned char note, unsigned int duration,
          unsigned char volume);

  /*! \brief Plays the specified sequence of notes.
   *
   *  \param sequence Char array containing a sequence of notes to play.
   *
   * If the play mode is `PLAY_AUTOMATIC` (default), the sequence of notes will
   * play with no further action required by the user. If the play mode is
   * `PLAY_CHECK`, the user will need to call `playCheck()` in the main loop to
   * initiate the playing of each new note in the sequence. The play mode can be
   * changed while the sequence is playing. The sequence syntax is modeled after
   * the PLAY commands in GW-BASIC, with just a few differences.
   *
   * The notes are specified by the characters **C**, **D**, **E**, **F**,
   * **G**, **A**, and **B**, and they are played by default as "quarter notes"
   * with a length of 500 ms. This corresponds to a tempo of 120 beats/min.
   * Other durations can be specified by putting a number immediately after the
   * note. For example, C8 specifies C played as an eighth note, with half the
   * duration of a quarter note. The special note **R** plays a rest (no sound).
   * The sequence parser is case-insensitive and ignores spaces, which may be
   * used to format your music nicely.
   *
   * Various control characters alter the sound:
   * <table>
   * <tr><th>Control character(s)</th><th>Effect</th></tr>
   * <tr><td><strong>A--G</strong></td>
   *     <td>Specifies a note that will be played.</td></tr>
   * <tr><td><strong>R</strong></td>
   *     <td>Specifies a rest (no sound for the duration of the note).</td></tr>
   * <tr><td><strong>+</strong></strong> or <strong>#</strong> after a note</td>
   *     <td>Raises the preceding note one half-step.</td></tr>
   * <tr><td><strong>-</strong> after a note</td>
   *     <td>Lowers the preceding note one half-step.</td></tr>
   * <tr><td><strong>1--2000</strong> after a note</td>
   *     <td>Determines the duration of the preceding note. For example, C16
   *         specifies C played as a sixteenth note (1/16th the length of a
   *         whole note).</td></tr>
   * <tr><td><strong>.</strong> after a note</td>
   *     <td>"Dots" the preceding note, increasing the length by 50%. Each
   *         additional dot adds half as much as the previous dot, so that "A.."
   *         is 1.75 times the length of "A".</td></tr>
   * <tr><td><strong>></strong> before a note</td>
   *     <td>Plays the following note one octave higher.</td></tr>
   * <tr><td><strong><</strong> before a note</td>
   *     <td>Plays the following note one octave lower.</td></tr>
   * <tr><td><strong>O</strong> followed by a number</td>
   *     <td>Sets the octave. (default: **O4**)</td></tr>
   * <tr><td><strong>T</strong> followed by a number</td>
   *     <td>Sets the tempo in beats per minute (BPM). (default: **T120**)</td></tr>
   * <tr><td><strong>L</strong> followed by a number</td>
   *     <td>Sets the default note duration to the type specified by the number:
   *         4 for quarter notes, 8 for eighth notes, 16 for sixteenth notes,
   *         etc. (default: **L4**)</td></tr>
   * <tr><td><strong>V</strong> followed by a number</td>
   *     <td>Sets the music volume (0--15). (default: **V15**)</td></tr>
   * <tr><td><strong>MS</strong></td>
   *     <td>Sets all subsequent notes to play play staccato -- each note is
   *     played for 1/2 of its allotted time, followed by an equal period of
   *     silence.</td></tr>
   * <tr><td><strong>ML</strong></td>
   *     <td>Sets all subsequent notes to play legato -- each note is played for
   *     full length. This is the default setting.</td></tr>
   * <tr><td><strong>!</strong></td>
   *     <td>Resets the octave, tempo, duration, volume, and staccato setting to
   *     their default values. These settings persist from one `play()` to the
   *     next, which allows you to more conveniently break up your music into
   *     reusable sections.</td></tr>
   * </table>
   *
   * This function plays the string of notes in the background while your
   * program continues to execute. If you call another buzzer function while the
   * melody is playing, the new function call will overwrite the previous and
   * take control of the buzzer. If you want to string melodies together, you
   * should put an appropriate delay after you start a melody playing. You can
   * use the `is_playing()` function to figure out when the buzzer is through
   * playing the melody.
   *
   * ### Example ###
   *
   * ~~~{.ino}
   * ZumoBuzzer buzzer;
   *
   * ...
   *
   * // play a C major scale up and back down:
   * buzzer.play("!L16 V8 cdefgab>cbagfedc");
   * while (buzzer.isPlaying());
   *
   * // the first few measures of Bach's fugue in D-minor
   * buzzer.play("!T240 L8 agafaea dac+adaea fa<aa<bac#a dac#adaea f4");
   * ~~~
   */
  static void play(const char *sequence);

  /*! \brief Plays the specified sequence of notes from program space.
   *
   * \param sequence Char array in program space containing a sequence of notes
   *                 to play.
   *
   * A version of `play()` that takes a pointer to program space instead of RAM.
   * This is desirable since RAM is limited and the string must be in program
   * space anyway.
   *
   * ### Example ###
   *
   * ~~~{.ino}
   * #include <avr/pgmspace.h>
   *
   * ZumoBuzzer buzzer;
   * const char melody[] PROGMEM = "!L16 V8 cdefgab>cbagfedc";
   *
   * ...
   *
   * buzzer.playFromProgramSpace(melody);
   * ~~~
   */
  static void playFromProgramSpace(const char *sequence_p);

  /*! \brief Controls whether `play()` sequence is played automatically or
   *         must be driven with `playCheck()`.
   *
   * \param mode Play mode (either `PLAY_AUTOMATIC` or `PLAY_CHECK`).
   *
   * This method lets you determine whether the notes of the `play()` sequence
   * are played automatically in the background or are driven by the
   * `play_check()` method. If \a mode is `PLAY_AUTOMATIC`, the sequence will
   * play automatically in the background, driven by the timer overflow
   * interrupt. The interrupt will take a considerable amount of time to execute
   * when it starts the next note in the sequence playing, so it is recommended
   * that you do not use automatic-play if you cannot tolerate being interrupted
   * for more than a few microseconds. If \a mode is `PLAY_CHECK`, you can
   * control when the next note in the sequence is played by calling the
   * `play_check()` method at acceptable points in your main loop. If your main
   * loop has substantial delays, it is recommended that you use automatic-play
   * mode rather than play-check mode. Note that the play mode can be changed
   * while the sequence is being played. The mode is set to `PLAY_AUTOMATIC` by
   * default.
   */
  static void playMode(unsigned char mode);

  /*! \brief Starts the next note in a sequence, if necessary, in `PLAY_CHECK`
   *         mode.
   *
   *  \return 0 if sequence is complete, 1 otherwise.
   *
   * This method only needs to be called if you are in `PLAY_CHECK` mode. It
   * checks to see whether it is time to start another note in the sequence
   * initiated by `play()`, and starts it if so. If it is not yet time to start
   * the next note, this method returns without doing anything. Call this as
   * often as possible in your main loop to avoid delays between notes in the
   * sequence. This method returns 0 (false) if the melody to be played is
   * complete, otherwise it returns 1 (true).
   */
  static unsigned char playCheck();

  /*! \brief Checks whether a note, frequency, or sequence is being played.
   *
   * \return 1 if the buzzer is current playing a note, frequency, or sequence;
   *         0 otherwise.
   *
   * This method returns 1 (true) if the buzzer is currently playing a
   * note/frequency or if it is still playing a sequence started by `play()`.
   * Otherwise, it returns 0 (false). You can poll this method to determine when
   * it's time to play the next note in a sequence, or you can use it as the
   * argument to a delay loop to wait while the buzzer is busy.
   */
  static unsigned char isPlaying();

  /*! \brief Stops any note, frequency, or melody being played.
   *
   * This method will immediately silence the buzzer and terminate any
   * note/frequency/melody that is currently playing.
   */
  static void stopPlaying();


  private:

  // initializes timer for buzzer control
  static void init2();
  static void init();
};

#endif