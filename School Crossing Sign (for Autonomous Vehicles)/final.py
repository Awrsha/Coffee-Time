import cv2
import serial

arduino=serial.Serial("/dev/ttyACM0",9600)
webcam=cv2.VideoCapture("nvarguscamerasrc ! "
        "video/x-raw(memory:NVMM), "
        "width=(int)640, height=(int)480, "
        "format=(string)NV12, framerate=(fraction)60/1 ! "
        "nvvidconv flip-method=2 ! "
        "video/x-raw, width=(int)640, height=(int)480, format=(string)BGRx ! "
        "videoconvert ! "
        "video/x-raw, format=(string)BGR ! appsink")

def crossing(webcam):
    school_crossing=cv2.CascadeClassifier("crossing.xml")
    while True:
        control,frame=webcam.read()
        gray=cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
        result=school_crossing.detectMultiScale(gray,1.1,4)
        arduino.write(b't')
        for (x,y,w,h) in result:
            cv2.putText(frame,"Crossing Sign",(x,y-10),cv2.FONT_HERSHEY_COMPLEX,1,(255,0,0),3)
            cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,0),4)
            arduino.write(b'a')
        if(cv2.waitKey(10) & 0xFF==ord('q')):
            break
        cv2.imshow("School Crossing Sign",frame)

while True:
    crossing(webcam)
