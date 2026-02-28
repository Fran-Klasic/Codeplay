

#define GPIO41 41
#define GPIO39 39
#define GPIO40 40
#define GPIO14 14

//Joystick data
typedef struct {
  int16_t buttons;
  int16_t joy_x;
  int16_t joy_y;
  int16_t tilt_x;
  int16_t tilt_y;
  int16_t tilt_z;
} joystick_packet_t;
joystick_packet_t joystick = {0};

void setup() {
  pinMode(GPIO41, INPUT_PULLUP);
  pinMode(GPIO39, INPUT_PULLUP);
  pinMode(GPIO40, INPUT_PULLUP);
  pinMode(GPIO14, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  int Button1Value = !digitalRead(GPIO41);
  int Button2Value = !digitalRead(GPIO39);
  int Button3Value = !digitalRead(GPIO40);
  int Button4Value = !digitalRead(GPIO14);

  int buttons = 0b00000;
  if (Button1Value) {
    buttons |= 0b00001;
  }
  if (Button2Value) {
    buttons |= 0b00100;
  }
  if (Button3Value) {
    buttons |= 0b00010; 
  }
  if (Button4Value) {  
    buttons |= 0b01000;
  }

  joystick.buttons= buttons;
  Serial.write((uint8_t*)&joystick, sizeof(joystick));
  delay(10);
}