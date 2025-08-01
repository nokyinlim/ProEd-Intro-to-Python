

/*
Nok Yin Lim - 28/7/2025
Servo control
*/


#include <Servo.h>


#define ModeSwitch 2

#define RightSpeed 5
#define LeftSpeed 6
#define RightDirection 7
#define LeftDirection 8

#define SwitchMode 2 // Mode switch Input
#define MotorStandby 3 // Place H-Bridge in standby if LOW, run if HIGH


Servo servo1;

void setup() {

    servo1.attach(10);
    servo1.write(90);

    // init values
    pinMode(2, INPUT);
    pinMode(3, OUTPUT);
    pinMode(RightSpeed, OUTPUT);
    pinMode(LeftSpeed, OUTPUT);
    pinMode(RightDirection, OUTPUT);
    pinMode(LeftDirection, OUTPUT);

    // Set the initial state of the motors
    analogWrite(RightSpeed, 0); // Set initial speed to 0
    analogWrite(LeftSpeed, 0); // Set initial speed to 0
    digitalWrite(RightDirection, HIGH); // Set initial direction to forward
    digitalWrite(LeftDirection, HIGH); // Set initial direction to forward
    digitalWrite(MotorStandby, HIGH); // Set motor to run mode

    delay(500);
}

void loop() {

    while (digitalRead(ModeSwitch) == 1) {
        // Wait for the mode switch to be pressed
        delay(100);
    }
    // int distanceCm = 60;
    analogWrite(RightSpeed, 255);
    analogWrite(LeftSpeed, 255);

    delay(250); // Convert distance to time in milliseconds (0.69 m/s)
    analogWrite(RightSpeed, 0);
    analogWrite(LeftSpeed, 0);
}

