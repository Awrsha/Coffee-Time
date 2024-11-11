void setup() {
pinMode(2,OUTPUT);
pinMode(5,OUTPUT);
pinMode(6,OUTPUT);
digitalWrite(2,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
analogWrite(5,0); // 0 = LOW --- 255 = HIGH
delay(1000);
analogWrite(5,125);
delay(1000);
analogWrite(5,255);
delay(1000);
}
