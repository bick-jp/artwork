// import the library
#include <Servo.h>

// create an instance of the Servo library
Servo myServo;

const int piezo = A0;      // pin the piezo is attached to

boolean busy = false;
unsigned long intervals[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// variable for the piezo value
int knockVal;

// variables for the low limits of the knock value
const int quietKnock = 250;
const int largeKnock = 100000;

void setup() {
  // attach the servo to pin 9
  myServo.attach(9);

  // start serial communication for debugging
  Serial.begin(9600);

  // move the servo to the unlocked position
  myServo.write(90);

  // print status to the Serial Monitor
  Serial.println("Set up done");
}

void loop() {

  // check the value of the piezo
  knockVal = analogRead(piezo);

  if (checkForKnock(knockVal) == true && busy == false) {
    busy = true;
    unsigned long start = millis();
    Serial.println("timer start");
    Serial.println(start);
    Serial.println("");
    delay(100);
    int counter = 0;
    unsigned long temp = start;
      while (millis() < start + 5000) { // 5sec=5000ms
        knockVal = analogRead(piezo);
        if (checkForKnock(knockVal) == true && counter < 20) {
          intervals[counter] = millis()-temp;
          Serial.println(intervals[counter]);
          temp += intervals[counter];
          counter++;
        }
      }
    Serial.println("Timer end");

    Serial.println("Play start");
    myServo.write(25);
    delay(100);
    myServo.write(90);
    delay(100); 
    for(int i = 0; i < 20 && intervals[i] != 0; i++) {
      Serial.println(intervals[i]);
      if (long(intervals[i]-200) >= 0) {
        Serial.println("OK");
        delay(intervals[i]-200);
      } else {
        Serial.println("Too short");
        delay(1);
      }
      myServo.write(25);
      delay(100);
      myServo.write(90);
      delay(100);      

      intervals[i] = 0;
    }
    
    busy = false;
    myServo.write(90);
    delay(100);

  }
  
}

// this function checks to see if a detected knock is within max and min range
boolean checkForKnock(int value) {
  
  // if the value of the knock is greater than the minimum, and larger
  // than the maximum
  if (value > quietKnock) {
    delay(100);

    return true;
  }
  // if the knock is not within range
  else {
    //Serial.println(value);
    return false;
  }
}