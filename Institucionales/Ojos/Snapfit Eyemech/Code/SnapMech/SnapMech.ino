
#include <Servo.h>

Servo lookLR;  // create servo object to control a servo
Servo lookUD;  // create servo object to control a servo
Servo lidBL;  // create servo object to control a servo
Servo lidTL;  // create servo object to control a servo
Servo lidBR;  // create servo object to control a servo
Servo lidTR;  // create servo object to control a servo

int BUTPin = 7;
int UDPin = A0;
int LRPin = A1;

int UDState = 90;
int LRState = 90;
int buttonState;
int lidMod = 0;


// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  //Serial.begin(9600);
  pinMode(BUTPin, INPUT_PULLUP);
  
  lookLR.attach(3);
  lookUD.attach(5);
  lidBL.attach(6);
  lidTL.attach(9);
  lidBR.attach(10);
  lidTR.attach(11);
}

void loop() {

  buttonState =  digitalRead(BUTPin);
  UDState = map(analogRead(UDPin), 0, 1023, 50, 130);
  lidMod = (40 - UDState)/2;
  LRState = map(analogRead(LRPin), 0, 1023, 40, 140);  
  
  //Serial.println(buttonState);

  lookUD.write(UDState);
  lookLR.write(LRState);

  if (buttonState == 0){
    lidBL.write(90);
    lidTL.write(90);
    lidBR.write(90);
    lidTR.write(90);
  } else {
    lidBL.write(160+lidMod);
    lidTL.write(70+lidMod);
    lidBR.write(30-lidMod);
    lidTR.write(110-lidMod);
  }



  /*
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    myservo1.write(pos);
    myservo2.write(pos);
    myservo3.write(pos);
    myservo4.write(pos);
    myservo5.write(pos);
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    myservo1.write(pos);
    myservo2.write(pos);
    myservo3.write(pos);
    myservo4.write(pos);
    myservo5.write(pos);
    delay(15);                       // waits 15 ms for the servo to reach the position
  }*/

}
