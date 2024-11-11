char incomingData;  // Variable to store incoming serial data

void setup() {
  // Set up digital pins as output pins
  pinMode(2, OUTPUT);   // Configure pin 2 as output for potential future use
  pinMode(5, OUTPUT);   // Configure pin 5 as output (controls PWM for LED brightness)
  pinMode(6, OUTPUT);   // Configure pin 6 as output for potential future use
  
  // Initialize pin 2 to HIGH to turn on any connected component initially
  digitalWrite(2, HIGH);

  // Begin serial communication at a baud rate of 9600 bps
  Serial.begin(9600);
}

void loop() {
  // Check if there is incoming serial data
  if (Serial.available() > 0) {
    // Read the incoming byte of data
    incomingData = Serial.read();

    // If the received data is 'a', set pin 5 to a lower PWM value (dim light)
    if (incomingData == 'a') {
      analogWrite(5, 75);  // Set pin 5 to 75, which dims the LED or connected device
    }
    // For any other data, set pin 5 to maximum PWM value (full brightness)
    else {
      analogWrite(5, 255); // Set pin 5 to 255, providing full brightness
    }
  }
}
