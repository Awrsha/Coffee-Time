# Import the pySerial library for serial communication
import serial

# Set up the serial port with the correct COM port and baud rate (9600 bps)
port = serial.Serial('com5', 9600)

# Continuously prompt the user for input to control the LED
while True:
    # Prompt user input for controlling the LED
    data_in = input("Press the letter 'a' to turn on the LED, or 'b' to turn it off: ")
    
    # If the user inputs 'a', send the character 'a' to the Arduino
    if data_in == 'a':
        port.write(b'a')  # Send the byte 'a' to turn the LED on
    
    # If the user inputs 'b', send the character 'b' to the Arduino
    elif data_in == 'b':
        port.write(b'b')  # Send the byte 'b' to turn the LED off
