# Arduino LED Control via Serial Communication

This project demonstrates how to control an LED on an Arduino board using serial communication between an Arduino and a Python program on a computer. The LED can be turned on or off by sending specific commands from the Python script to the Arduino.

## Project Structure

- `controlLED.ino.ino`: Arduino code to receive serial data and control the LED accordingly.
- `send_data.py`: Python script that sends commands to the Arduino over the serial port based on user input.
- `README.md`: Project description, setup instructions, and usage guide.

## Requirements

- Arduino board (such as Uno or Nano)
- LED connected to pin 13 (built-in LED on most Arduino boards)
- USB cable for connecting the Arduino to the computer
- Python 3.x
- `pySerial` library for Python (install with `pip install pyserial`)

## Setup Instructions

### Hardware Setup

1. Connect the Arduino to your computer via USB.
2. Optionally, connect an external LED to pin 13 and ground on the Arduino. The built-in LED on pin 13 can also be used for this example.

### Software Setup

1. **Upload Arduino Code**: Open the `controlLED.ino.ino` file in the Arduino IDE. Select the correct board and port, then upload the code to the Arduino.
2. **Install Python Requirements**: In the terminal or command prompt, run:
   ```bash
   pip install pyserial
   ```

## Usage

1. **Run the Python Script**: Execute the Python script `send_data.py` in a terminal or command prompt:
   ```bash
   python send_data.py
   ```
2. **Control the LED**: Follow the on-screen instructions in the Python script:
   - Type `'a'` and press Enter to turn on the LED.
   - Type `'b'` and press Enter to turn off the LED.

## Code Overview

### Arduino Code

The Arduino code initializes serial communication at 9600 baud rate. It reads incoming data from the serial port and checks if the received character is `'a'` or `'b'`:
- If the character is `'a'`, it turns on the LED connected to pin 13.
- If the character is `'b'`, it turns off the LED.

### Python Code

The Python script initializes a serial connection to the Arduino and prompts the user to enter `'a'` or `'b'`:
- If the user enters `'a'`, it sends the byte `'a'` to the Arduino to turn on the LED.
- If the user enters `'b'`, it sends the byte `'b'` to the Arduino to turn off the LED.

## Troubleshooting

- **Port Configuration**: Ensure the port specified in `port = serial.Serial('com5', 9600)` in the Python code matches the actual port used by your Arduino. Adjust as necessary (e.g., `COM3` on Windows or `/dev/ttyUSB0` on Linux).
- **Baud Rate Consistency**: Both the Arduino and Python scripts should use the same baud rate (9600).
- **Permission Issues**: On some operating systems, you may need administrative privileges to access the serial port.

## License

This project is open-source and available under the MIT License. You are free to use, modify, and distribute it as needed.

## Acknowledgments

- [Arduino Documentation](https://www.arduino.cc/reference/en/)
- [PySerial Documentation](https://pyserial.readthedocs.io/)
