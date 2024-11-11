# School Crossing Sign (for Autonomous Vehicles)

This project demonstrates a simple computer vision-based system that detects school crossing signs using a camera feed and sends signals to an Arduino to trigger actions based on the detected signs. The setup is designed as a proof-of-concept for autonomous vehicles or driver-assistance systems, which could alert the vehicle or driver when a school crossing sign is detected.

## Table of Contents
- [Overview](#overview)
- [Project Structure](#project-structure)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Setup Instructions](#setup-instructions)
- [Usage](#usage)
- [Code Explanation](#code-explanation)
- [Future Enhancements](#future-enhancements)
- [License](#license)

## Overview
This project involves using a camera to monitor the road for school crossing signs. When a crossing sign is detected, the system:
- Highlights the detected sign on the video feed.
- Sends a signal to an Arduino board to perform an action (e.g., alerting the vehicle to slow down or stop).

The detection is performed using OpenCV’s Haar Cascade Classifier, which processes each video frame to detect crossing signs in real-time.

## Project Structure
- **main.py**: The primary Python code that captures video, performs detection, and communicates with Arduino.
- **crossing.xml**: The Haar Cascade XML file for detecting school crossing signs.
- **README.md**: Documentation file.
- **Arduino Code**: Code to read signals from the computer and control outputs based on the signals.

## Hardware Requirements
- Arduino (or compatible microcontroller)
- Camera module (USB camera or NVIDIA Jetson onboard camera)
- Computer with a Python environment (NVIDIA Jetson is recommended for optimal video processing performance)
- LED or another indicator (optional, connected to the Arduino to test responses)

## Software Requirements
- Python 3.x
- OpenCV (install using `pip install opencv-python`)
- PySerial (install using `pip install pyserial`)
- Arduino IDE for uploading code to the Arduino

## Setup Instructions
1. **Arduino Setup**:
   - Connect an LED or another indicator to pin 5 of the Arduino board.
   - Upload the Arduino code (see below) to the board.

2. **Python Environment Setup**:
   - Install required libraries:
     ```bash
     pip install opencv-python pyserial
     ```
   - Place the `crossing.xml` file (Haar Cascade for crossing signs) in the project directory.

3. **Camera Setup**:
   - Use a compatible camera. For NVIDIA Jetson platforms, use the GStreamer pipeline for optimized performance, or adjust the camera source for other systems.

## Usage
1. Run the Python script to start capturing video and processing frames for school crossing signs.
   ```bash
   python final.py
   ```
2. Monitor the video output. If a crossing sign is detected, the bounding box will appear around it with the label "Crossing Sign".
3. The Arduino receives a signal and responds by controlling the connected LED or other indicators.

## Code Explanation

### Python Code (`final.py`)
The Python code captures the video stream, processes each frame to detect school crossing signs, and communicates with the Arduino over serial.

- **Video Capture**: Initializes the camera feed using OpenCV and GStreamer pipeline.
- **Detection**: Uses Haar Cascade to detect crossing signs. If a sign is detected, a bounding box is drawn.
- **Serial Communication**: Sends a signal ('a') to the Arduino when a crossing sign is detected. If no sign is detected, a continuous 't' signal is sent as a default state.

## Future Enhancements
- **Improved Detection**: Train a custom machine learning model for better accuracy in detecting crossing signs.
- **Multithreading**: Implement multithreading in Python to enhance video capture and processing speed.
- **Additional Indicators**: Integrate sounds or visual warnings to simulate a real autonomous vehicle response.

## License
This project is open-source under the [MIT License](https://github.com/Awrsha/Coffee-Time/blob/master/LICENSE). Feel free to use, modify, and distribute for personal and commercial use.
