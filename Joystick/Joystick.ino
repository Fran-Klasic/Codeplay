#define GPIO41 41
#define GPIO39 39
#define GPIO40 40
#define GPIO14 14

void setup() {
  // put your setup code here, to run once:
    pinMode(GPIO41,INPUT_PULLUP);
    pinMode(GPIO39,INPUT_PULLUP);
    pinMode(GPIO40,INPUT_PULLUP);
    pinMode(GPIO14,INPUT_PULLUP);
    Serial.Begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
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

    if(Button1Value){

    }
    if(Button2Value){
      
    }
    if(Button3Value){
      
    }
    if(Button4Value){
      
    }
}