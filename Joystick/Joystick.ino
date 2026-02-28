#define BUTTON1 41 
#define BUTTON2 39
#define BUTTON3 40
#define BUTTON4 14
#define PINGMS 20
#define BUZZER 42
#define VIBROMOTOR 21
unsigned long myTime;

int FirstCallTime;

typedef struct {
  int16_t buttons;
  int16_t joy_x;
  int16_t joy_y;
  int16_t tilt_x;
  int16_t tilt_y;
  int16_t tilt_z;
} joystick_packet_t;
joystick_packet_t joystick = {0};


void userFeedback(bool IsFirstCall){
  int currentTime = millis();
  if(IsFirstCall){
    tone(BUZZER, 1000 );
    digitalWrite(VIBROMOTOR, HIGH);
    FirstCallTime=millis();
  }else if(currentTime>=FirstCallTime+1000){
    noTone(BUZZER);
    digitalWrite(VIBROMOTOR, LOW);
  }
}

int buttonCliks(){
  int Button1Value = !digitalRead(BUTTON1);
  int Button2Value = !digitalRead(BUTTON2);
  int Button3Value = !digitalRead(BUTTON3);
  int Button4Value = !digitalRead(BUTTON4);

  int buttons = 0b00000;
  buttons |= Button1Value << 0;
  buttons |= Button2Value << 1;
  buttons |= Button3Value << 2;
  buttons |= Button4Value << 3;
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

void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(VIBROMOTOR, OUTPUT );
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);
  Serial.begin(115200);
  userFeedback(true);
}

void loop() {
  userFeedback(false);
  joystick.buttons = buttonCliks();
  joystick.joy_x = getJoystickX();
  joystick.joy_y = getJoystickY();
  Serial.write((uint8_t*)&joystick, sizeof(joystick));
  delay(PINGMS);
}