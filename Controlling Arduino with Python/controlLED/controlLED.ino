char incomingData;
void setup() {
  Serial.begin(9600);
  pinMode(13,OUTPUT);

}

void loop() {
  if(Serial.available()>0){
    incomingData=Serial.read();
    if(incomingData=='a'){
      digitalWrite(13,HIGH);
      }
     else if(incomingData=='b'){
      digitalWrite(13,LOW);
      }
    }

}
