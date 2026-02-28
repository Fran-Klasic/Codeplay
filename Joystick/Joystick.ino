#include <FastLED.h>
#include "esp_sleep.h"
#include "driver/rtc_io.h"

#define BUTTON1 41
#define BUTTON2 39
#define BUTTON3 40
#define BUTTON4 14
#define BUTTON5 10

#define TIME_UNTIL_SLEEP 30000
#define EVENT_TIME 1000

#define PINGMS 20
#define BUZZER 42
#define VIBROMOTOR 21

#define LED_PIN 2
#define NUM_LEDS 1
#define BRIGHTNESS 100
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB


#define JOY_PIN_X 11
#define JOY_PIN_Y 12


unsigned long FirstBuzzerCallTime;
unsigned long FirstSleepCallTime;


typedef struct {
  int16_t buttons;
  int16_t joy_x;
  int16_t joy_y;
  int16_t tilt_x;
  int16_t tilt_y;
  int16_t tilt_z;
} joystick_packet_t;
joystick_packet_t joystick = { 0 };

CRGB leds[NUM_LEDS];
CRGB GetCustomColor(unsigned long time) {
  //Periods in milliseconds
  const float redPeriod = 1000.0;
  const float greenPeriod = 2000.0;
  const float bluePeriod = 3000.0;

  //Convert time -> angle [0,2PI]
  float redAngle = (time % (unsigned long)redPeriod) * TWO_PI / redPeriod;
  float greenAngle = (time % (unsigned long)greenPeriod) * TWO_PI / greenPeriod;
  float blueAngle = (time % (unsigned long)bluePeriod) * TWO_PI / bluePeriod;

  //sin() returns [-1,1] -> shift to [0,255]
  uint8_t red = floor((sin(redAngle) * 0.5 + 0.5) * 256);
  uint8_t green = floor((sin(greenAngle) * 0.5 + 0.5) * 256);
  uint8_t blue = floor((sin(blueAngle) * 0.5 + 0.5) * 256);

  return CRGB(red, green, blue);
}

void userFeedback(bool IsFirstCall) {
  unsigned long currentTime = millis();
  if (IsFirstCall) {
    tone(BUZZER, 1000);
    digitalWrite(VIBROMOTOR, HIGH);
    FirstBuzzerCallTime = millis();
  }
  if (currentTime >= FirstBuzzerCallTime + EVENT_TIME) {
    noTone(BUZZER);
    digitalWrite(VIBROMOTOR, LOW);
  }
}

int buttonCliks() {
  int Button1Value = !digitalRead(BUTTON1);
  int Button2Value = !digitalRead(BUTTON2);
  int Button3Value = !digitalRead(BUTTON3);
  int Button4Value = !digitalRead(BUTTON4);
  int Button5Value = !digitalRead(BUTTON5);

  int buttons = 0b00000;
  buttons |= Button1Value << 0;
  buttons |= Button2Value << 1;
  buttons |= Button3Value << 2;
  buttons |= Button4Value << 3;
  buttons |= Button5Value << 4;
  return buttons;
}

int getJoystickX() {

  //Get raw value 0-1023
  int Joy_X_raw = analogRead(JOY_PIN_X);
  //Map phsical position of joystic axis from [0, 4095] to wanted value [-100, 100]
  int Joy_X_val = map(Joy_X_raw, 0, 4095, -100, 100);

  if (Joy_X_val >= -50 && Joy_X_val <= 50)
    Joy_X_val = 0;
  return Joy_X_val;
}

int getJoystickY() {
  //Get raw value 0-1023
  int Joy_Y_raw = analogRead(JOY_PIN_Y);
  //Map phsical position of joystic axis from [0, 4095] to wanted value [-100, 100]
  int Joy_Y_val = map(Joy_Y_raw, 0, 4095, -100, 100);

  if (Joy_Y_val >= -50 && Joy_Y_val <= 50)
    Joy_Y_val = 0;

  return Joy_Y_val;
}

void SleepCheck(bool IsFirstCall) {
  int currentTime = millis();
  if (IsFirstCall) {
    FirstSleepCallTime = millis();
  } else if (currentTime >= FirstSleepCallTime + TIME_UNTIL_SLEEP) {
    leds[0] = CRGB::Black;
    FastLED.show();
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_14, 0);
    rtc_gpio_pullup_en(GPIO_NUM_14);
    esp_deep_sleep_start();
  }
}

void eventCheck(){
  if(Serial.available()>0){
  int event = Serial.read();
  switch(event){
    case 0x01:
      userFeedback(true);
      break;
    case 0x02:
      userFeedback(true);
      break;
    case 0x03:
      userFeedback(true);
      break;
    case 0x04:
      userFeedback(true);
      break;
    case 0x05:
      userFeedback(true);
      break;  
  }
  }
  
}

void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(VIBROMOTOR, OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);
  pinMode(BUTTON5, INPUT_PULLUP);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  SleepCheck(true);
  Serial.begin(115200);
  userFeedback(true);
}

void loop() {
  delay(PINGMS);
  SleepCheck(false);
 
  leds[0] = GetCustomColor(millis());
  FastLED.show();
  eventCheck();
  joystick.buttons = buttonCliks();
  joystick.joy_x = getJoystickX();
  joystick.joy_y = getJoystickY();

  if (joystick.buttons > 0 || joystick.joy_x != 0 || joystick.joy_y != 0) {
    SleepCheck(true);
  }
  userFeedback(false);


  Serial.write((uint8_t*)&joystick, sizeof(joystick));
}