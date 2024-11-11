import cv2
import serial

# Initialize the serial communication with Arduino on the specified port
# Replace "/dev/ttyACM0" with your port, if different
arduino = serial.Serial("/dev/ttyACM0", 9600)

# Set up the camera with custom settings for NVIDIA Jetson platforms
# The GStreamer pipeline enables high-performance video capture
webcam = cv2.VideoCapture("nvarguscamerasrc ! "
                          "video/x-raw(memory:NVMM), "
                          "width=(int)640, height=(int)480, "
                          "format=(string)NV12, framerate=(fraction)60/1 ! "
                          "nvvidconv flip-method=2 ! "
                          "video/x-raw, width=(int)640, height=(int)480, format=(string)BGRx ! "
                          "videoconvert ! "
                          "video/x-raw, format=(string)BGR ! appsink")

def crossing(webcam):
    """
    Detects school crossing signs in real-time video feed and sends signals to Arduino.
    
    Parameters:
    - webcam: OpenCV video capture object for the camera.
    """
    
    # Load the Haar Cascade classifier for detecting school crossing signs
    school_crossing = cv2.CascadeClassifier("crossing.xml")
    
    while True:
        # Capture frame-by-frame from the video feed
        control, frame = webcam.read()
        
        # Convert the captured frame to grayscale for better detection accuracy
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        
        # Detect school crossing signs in the grayscale frame
        # Parameters: scaleFactor=1.1, minNeighbors=4 for refining detection
        result = school_crossing.detectMultiScale(gray, 1.1, 4)
        
        # Send a continuous signal 't' to Arduino, which can serve as a default state
        arduino.write(b't')
        
        # Loop through each detected crossing sign's bounding box
        for (x, y, w, h) in result:
            # Label the detected sign and draw a rectangle around it
            cv2.putText(frame, "Crossing Sign", (x, y - 10), cv2.FONT_HERSHEY_COMPLEX, 1, (255, 0, 0), 3)
            cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 4)
            
            # Send an 'a' signal to Arduino indicating a crossing sign detection
            arduino.write(b'a')
        
        # Display the processed frame with the detections
        cv2.imshow("School Crossing Sign", frame)
        
        # Break the loop if 'q' is pressed to stop the detection
        if cv2.waitKey(10) & 0xFF == ord('q'):
            break

# Run the crossing detection continuously
while True:
    crossing(webcam)
