import serial

port=serial.Serial('com5',9600)

while True:
    data_in=input("Press the letter 'a' to turn on the LED, press the letter 'b' to turn it off:")
    if (data_in=='a'):
        port.write(b'a')
    elif (data_in=='b'):
        port.write(b'b')