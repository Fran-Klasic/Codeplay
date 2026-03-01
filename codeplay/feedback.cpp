#include "feedback.h"
#include "led.h"
// Stores the timestamp of the last feedback activation.
// Used to stop sound and vibration after a fixed duration.
static unsigned long firstCallTime = 0;

// Defines all available feedback melodies.

// Timestamp when current note started
static unsigned long noteStartTime = 0;

// Index of the note currently being played
static int currentNoteIndex = 0;

// True while a melody is actively playing
static bool melodyPlaying = false;

// Currently selected melody
static MelodyType activeMelody = MELODY_NONE;


// Each melody consists of:
//  - frequency array (Hz)
//  - duration array (ms)
//  - length of melody

// Default tone – single steady 1 second beep

// Default tone – single steady beep
const int defaultMelody[] = { 1000 };
const int defaultDuration[] = { 1000 };
const int defaultLength = 1;

// Coin collected – happy short jingle
const int coinMelody[] = { 1319, 1568, 2093 };
const int coinDuration[] = { 80, 80, 120 };
const int coinLength = 3;

// Heal – rising warm tone
const int healMelody[] = { 800, 1000, 1200 };
const int healDuration[] = { 120, 120, 180 };
const int healLength = 3;

// Enemy killed – sharp victory tone
const int killMelody[] = { 1800, 2000, 2300 };
const int killDuration[] = { 70, 70, 150 };
const int killLength = 3;

// Damage – descending warning
const int damageMelody[] = { 1000, 900, 800 };
const int damageDuration[] = { 100, 100, 150 };
const int damageLength = 3;

// Death – dramatic falling tone
const int deathMelody[] = { 800, 600, 400, 200 };
const int deathDuration[] = { 200, 200, 250, 400 };
const int deathLength = 4;

#define yellowFlashTimer  200
#define greenBreathingTimer 500
#define brightGreenFlashTimer 150
#define fastRedBlinkTimer 600

#define fastRedBlinkSpeed 150
#define brightGreenBlinkSpeed 250
#define yellowFlashBlinkSpeed 50

enum EffectType {
  EFFECT_IDLE,
  EFFECT_COIN,
  EFFECT_HEAL,
  EFFECT_KILL,
  EFFECT_DAMAGE,
  EFFECT_DEATH
};
EffectType currentEffect = EFFECT_IDLE;
unsigned long effectStart = 0;
uint8_t rainbowHue = 0;


// Controls buzzer and vibration motor feedback.
// - firstCall = true  -> starts feedback with selected tone
// - firstCall = false -> stops feedback after FEEDBACK_DURATION_MS
void userFeedback(bool start, MelodyType melody) {

  unsigned long now = millis();

  // Start new melody
  if (start) {
    activeMelody = melody;
    currentNoteIndex = 0;
    melodyPlaying = true;
    noteStartTime = now;
    digitalWrite(VIBROMOTOR, HIGH);
  }

  if (!melodyPlaying)
    return;

  const int* notes = nullptr;
  const int* durations = nullptr;
  int length = 0;

  //Chooses what melody to activate
  switch (activeMelody) {
    case MELODY_DEFAULT:
      notes = defaultMelody;
      durations = defaultDuration;
      length = defaultLength;
      break;

    case MELODY_COIN:
      notes = coinMelody;
      durations = coinDuration;
      length = coinLength;
      break;

    case MELODY_HEAL:
      notes = healMelody;
      durations = healDuration;
      length = healLength;
      break;

    case MELODY_KILL:
      notes = killMelody;
      durations = killDuration;
      length = killLength;
      break;

    case MELODY_DAMAGE:
      notes = damageMelody;
      durations = damageDuration;
      length = damageLength;
      break;

    case MELODY_DEATH:
      notes = deathMelody;
      durations = deathDuration;
      length = deathLength;
      break;

    default:
      melodyPlaying = false;
      return;
  }

  // Play current note
  if (currentNoteIndex < length) {

    if (now - noteStartTime >= durations[currentNoteIndex]) {
      currentNoteIndex++;
      noteStartTime = now;

      if (currentNoteIndex < length) {
        tone(BUZZER, notes[currentNoteIndex]);
      } else {
        noTone(BUZZER);
        digitalWrite(VIBROMOTOR, LOW);
        melodyPlaying = false;
      }
    } else {
      tone(BUZZER, notes[currentNoteIndex]);
    }
  }
}

// Checks for incoming game events and triggers different tones
// depending on the received event type.
void checkGameEvents() {

  if (!Serial.available())
    return;

  uint8_t event = Serial.read();
  effectStart = millis();
  switch (event) {
    // Coin collected
    case 0x01:  
      userFeedback(true, MELODY_COIN);
      currentEffect = EFFECT_COIN;
      break;

    // Heal received
    case 0x02:  
      userFeedback(true, MELODY_HEAL);
      currentEffect = EFFECT_HEAL;
      break;

    // Enemy killed
    case 0x03:  
      userFeedback(true, MELODY_KILL);
      currentEffect = EFFECT_KILL;
      break;

    // Damage received
    case 0x04:  
      userFeedback(true, MELODY_DAMAGE);
      currentEffect = EFFECT_DAMAGE;
      FastLED.show();
      break;

    // Player death
    case 0x05:  
      userFeedback(true, MELODY_DEATH);
      currentEffect = EFFECT_DEATH;
      break;

    default:
      currentEffect = EFFECT_IDLE;
    }
  }
  
void backToIdle(int now, int Timer){
  if (now - effectStart > Timer) {
          currentEffect = EFFECT_IDLE;
  }
}
void updateEffect() {
  unsigned long now = millis();
  

  switch (currentEffect) {

    // Rainbow animation
    case EFFECT_IDLE:   
      leds[0] = GetCustomColor(millis());
      FastLED.show();
      break;

    // Yellow flash
    case EFFECT_COIN:   
      if ((now / yellowFlashBlinkSpeed) % 2 == 0){
        leds[0] = CRGB::Yellow;
      }else{
        leds[0] = CRGB::Black;
      }
      FastLED.show();
      backToIdle(now,yellowFlashTimer);
      break;

    // Green breathing
    case EFFECT_HEAL:   
      {
      if ((now / brightGreenBlinkSpeed) % 2 == 0){
        leds[0] = CRGB::Green;
      }else{
        leds[0] = CRGB::Black;
      }
        FastLED.show();
        backToIdle(now,greenBreathingTimer);
      }
      break;

    // Bright green flash
    case EFFECT_KILL:   
      leds[0] = CRGB::Green;
      FastLED.show();
      backToIdle(now,brightGreenFlashTimer);
      break;

    // Fast red blink
    case EFFECT_DAMAGE:   
      if ((now / fastRedBlinkSpeed) % 2 == 0){
        leds[0] = CRGB::Red;
      }else{
        leds[0] = CRGB::Black;
      }
      FastLED.show();
      backToIdle(now,fastRedBlinkTimer);
      break;

    
    // Red fade out
    case EFFECT_DEATH:   
      {
        
        uint8_t fade = 255 - (now - effectStart) / 8;
        // Fill all LEDs with the current fade value as red
        fill_solid(leds, NUM_LEDS, CRGB(fade, 0, 0));
        FastLED.show();

        if (fade <= 8) {
          currentEffect = EFFECT_IDLE;
        }
      }
      break;

  }
}