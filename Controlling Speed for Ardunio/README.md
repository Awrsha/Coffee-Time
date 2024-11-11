# Arduino PWM LED Brightness Control

This project demonstrates how to control the brightness of an LED on an Arduino board using PWM (Pulse-Width Modulation). The LED brightness is adjusted in three levels (off, medium, and maximum brightness) using the `analogWrite()` function with different PWM values.

## Project Structure

- `LED_PWM_Control.ino`: Arduino code to control LED brightness using PWM on pin 5.
- `README.md`: Project description, setup instructions, and usage guide.

## Requirements

- Arduino board (such as Uno or Nano)
- LED connected to pin 5 through a resistor (to control brightness via PWM)
- Optional additional LEDs or components connected to pins 2 and 6

## Setup Instructions

### Hardware Setup

1. **Connect the LED to Arduino**: Connect the positive (longer) leg of an LED to pin 5 on the Arduino through a current-limiting resistor (typically 220Ω or 330Ω). Connect the negative leg of the LED to the ground (GND) on the Arduino.
2. Optionally, you can connect additional LEDs or components to pins 2 and 6 to expand control.

### Software Setup

1. **Upload the Code**: Open `LED_PWM_Control.ino` in the Arduino IDE, select the correct board and port, and upload the code.

## Usage

Once the code is uploaded, the LED connected to pin 5 will cycle through different brightness levels:
- **Off**: LED brightness set to 0 using `analogWrite(5, 0)`.
- **Medium Brightness**: LED brightness set to 125 (about 50%) using `analogWrite(5, 125)`.
- **Full Brightness**: LED brightness set to 255 (100%) using `analogWrite(5, 255)`.

Each brightness level lasts for 1 second before moving to the next level in the cycle.

## Code Overview

### Pin Configuration

- **Pin 2**: Configured as an output pin, initially set to HIGH. This can control additional components.
- **Pin 5**: Configured for PWM control, used to adjust the LED brightness by varying the PWM duty cycle.
- **Pin 6**: Configured as an output pin for possible future use with other components.

### Main Loop

The main `loop()` function cycles through three brightness levels for the LED:
1. **Brightness 0** (LED off): `analogWrite(5, 0)`
2. **Brightness 125** (about 50% brightness): `analogWrite(5, 125)`
3. **Brightness 255** (maximum brightness): `analogWrite(5, 255)`

Each level is held for 1 second before moving to the next.

## Troubleshooting

- **LED Not Lighting Up**: Check the connections and ensure the resistor is placed correctly. Also, verify the LED is not connected in reverse.
- **Incorrect Brightness Levels**: Verify that pin 5 is connected to an LED and that `analogWrite()` values are correctly set (0 for off, 255 for full brightness).

## License

This project is open-source under the MIT License. Feel free to modify and use it for your projects.

## Acknowledgments

- [Arduino Documentation](https://www.arduino.cc/reference/en/) for the `analogWrite()` and `digitalWrite()` functions.
