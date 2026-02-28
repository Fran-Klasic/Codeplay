

void setup() {
  // put your setup code here, to run once:
    pinMode(41,INPUT_PULLUP);
    pinMode(39,INPUT_PULLUP);
    pinMode(40,INPUT_PULLUP);
    pinMode(14,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
    int Button1Value=digitalRead(41);
    int Button2Value=digitalRead(39);
    int Button3Value=digitalRead(40);
    int Button4Value=digitalRead(14);

    if(Button1Value){

    }
    if(Button2Value){
      
    }
    if(Button3Value){
      
    }
    if(Button4Value){
      
    }
}

