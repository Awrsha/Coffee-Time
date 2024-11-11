char incomingData;
void setup() {
pinMode(2,OUTPUT);
pinMode(5,OUTPUT);
pinMode(6,OUTPUT);
digitalWrite(2,HIGH);
Serial.begin(9600);
}

void loop() {
  if(Serial.available()>0)
  {
    incomingData=Serial.read();
    if(incomingData=='a')
    {
      analogWrite(5,75);
      }
     else{
      analogWrite(5,255);
      }
    }

}