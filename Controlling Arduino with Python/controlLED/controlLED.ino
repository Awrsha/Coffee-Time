// Declare a variable to store incoming serial data
char incomingData;

void setup() {
  // Initialize the serial communication at a baud rate of 9600
  Serial.begin(9600);
  
  // Set pin 13 as an output pin, typically connected to an LED
  pinMode(13, OUTPUT);
}

void loop() {
  // Check if there is any data available to read from the serial port
  if (Serial.available() > 0) {
    // Read the incoming byte of data and store it in incomingData
    incomingData = Serial.read();
    
    // If the received character is 'a', turn the LED on
    if (incomingData == 'a') {
      digitalWrite(13, HIGH);  // Set pin 13 to HIGH (LED ON)
    }
    // If the received character is 'b', turn the LED off
    else if (incomingData == 'b') {
      digitalWrite(13, LOW);   // Set pin 13 to LOW (LED OFF)
    }
  }
}
