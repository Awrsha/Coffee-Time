void setup() {
  // Set pin 2 as an output pin (this can be used to control a digital device, such as an LED)
  pinMode(2, OUTPUT);
  
  // Set pin 5 as an output pin for PWM control (pulse-width modulation for variable brightness)
  pinMode(5, OUTPUT);
  
  // Set pin 6 as an output pin (can be used for additional devices or components)
  pinMode(6, OUTPUT);
  
  // Initially set pin 2 to HIGH, turning on any connected device (e.g., LED)
  digitalWrite(2, HIGH);
}

void loop() {
  // Write an analog value of 0 to pin 5 (PWM LOW) - this turns off the LED or sets it to minimum brightness
  analogWrite(5, 0);
  
  // Wait for 1 second (1000 milliseconds) before changing the brightness
  delay(1000);

  // Write an analog value of 125 to pin 5 (about half brightness for LED)
  analogWrite(5, 125);

  // Wait for another second before changing the brightness again
  delay(1000);

  // Write an analog value of 255 to pin 5 (PWM HIGH) - this sets the LED to maximum brightness
  analogWrite(5, 255);

  // Wait for 1 second before repeating the cycle
  delay(1000);
}
