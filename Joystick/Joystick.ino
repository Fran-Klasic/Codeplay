typedef struct {
  int16_t buttons;
  int16_t joy_x;
  int16_t joy_y;
  int16_t tilt_x;
  int16_t tilt_y;
  int16_t tilt_z;
} joystick_packet_t;

int 

void setup() {
  // put your setup code here, to run once:
    pinMode(41,INPUT_PULLUP);
    pinMode(39,INPUT_PULLUP);
    pinMode(40,INPUT_PULLUP);
    pinMode(14,INPUT_PULLUP);
}

void loop() {
    int Button1Value=digitalRead(41);
    int Button2Value=digitalRead(39);
    int Button3Value=digitalRead(40);
    int Button4Value=digitalRead(14);   

    Serial.print("\nButton 1: ");
    Serial.print(Button1Value);
    Serial.print("\nButton 2: ");
    Serial.print(Button2Value);
    Serial.print("\nButton 3: ");
    Serial.print(Button3Value);
    Serial.print("\nButton 4: ");
    Serial.print(Button4Value);
    
    button = 0b00000;
    if(Button1Value){
      button = 0b00001;
    }
    if(Button2Value){
      button = 0b00010;
    }
    if (Button3Value) {
      button = 0b00100;
    }
    if (Button4Value) {
      button = 0b01000;
    }
    
    
   
}