#define BUTTON1 41 
#define BUTTON2 39
#define BUTTON3 40
#define BUTTON4 14
#define PINGMS 20


typedef struct {
  int16_t buttons;
  int16_t joy_x;
  int16_t joy_y;
  int16_t tilt_x;
  int16_t tilt_y;
  int16_t tilt_z;
} joystick_packet_t;
joystick_packet_t joystick = {0};

void powerUp(){
  tone(GPIO42, 1000);
  delay(1000);
  noTone(GPIO42);
  delay(1000);
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

void setup() {
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);
  Serial.begin(115200);
  powerUp();
}

void loop() {
  joystick.buttons = buttonCliks();

  Serial.write((uint8_t*)&joystick, sizeof(joystick));
  delay(PINGMS);
}