//motor A
int in1 = 2;
int in2 = 3;

//motor B
int in3 = 4;
int in4 = 5;

long currentTime;
int speaking;
char serialChar;arduino
int mouthState; // 0 for close and 1 for open
long actionTime, actionType, nextFlop;

void close_mouth(){
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void open_mouth(){
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void flop_tail(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  delay(200);
  digitalWrite(in2, LOW);
  digitalWrite(in1, HIGH);
  delay(200);
  digitalWrite(in2, LOW);
  digitalWrite(in1, LOW);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Serial.begin(9600);
  //the fish is not in a talking state and has the mouth close
  speaking = 0;
  mouthState = 0;
  actionTime = millis();
  actionType = 0; //idle action;
  nextFlop = 0;
}

void updateState(){
  if(Serial.available()){
    serialChar = Serial.read();
    Serial.println(serialChar);
    if(serialChar == 'v'){ //if we detected voice
      speaking = 1;
      Serial.print("Speaking\n");
    }
    if(serialChar == 's'){  //there is no voice
      speaking = 0;
    }
  }
}

void articulate_fish(){
  if(speaking == 1){
    switch(mouthState){
      case 0: //daca gura e inchisa
        if(actionTime < currentTime){ //daca nu aveam o actiune programata
          actionType = 1; 
          actionTime = currentTime + 500;
        }
        break;
      case 1: //daca gura e deschisa
        if(actionTime < currentTime){ //daca nu aveam o actiune programata
          actionType = 0; 
          actionTime = currentTime + 100;
        }
        break;
    } 
  }else{ //inchidem gura cand nu vorbeste
    if(mouthState == 1){
      actionType = 0; 
      actionTime = currentTime + 100;
    }
    if(nextFlop < currentTime ){
      Serial.print("Setting next Flop\n");
      nextFlop = currentTime + 10000;
      flop_tail();
    }
  }


  if(actionTime > currentTime){
    if(actionType == 1){
      open_mouth();
    }
    if(actionType == 0){
      close_mouth();
    }
  }else{
    if(actionType == 1){
      mouthState = 1;
    }
    if(actionType == 0){
      mouthState = 0;
    }
  }
  if(nextFlop < currentTime && speaking == 0){
    Serial.print("Flop\n");
    flop_tail();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  currentTime = millis();
  updateState();
  articulate_fish();
}
