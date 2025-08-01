#include <Servo.h>
#include <HCSR04.h>

/*
Nok Yin Lim - 28/7/2025
Driving with functions
Utility functions for driving a rover.

Includes obstacle detection using ultrasonic sensors (HC-SR04)


*/

// define pins as needed


#define ModeSwitch 2


// Use the ultrasonic sensor to detect obstacles and items in front of the robot
#define EchoPin 12
#define TrigPin 13


#define RightSpeed 5
#define LeftSpeed 6
#define RightDirection 7
#define LeftDirection 8

#define LTR analogRead(A0)
#define LTM analogRead(A1)
#define LTL analogRead(A2)


#define MotorStandby 3 // Place H-Bridge in standby if LOW, run if HIGH

byte triggerPin = 13;
byte echoCount = 1;
byte* echoPins = new byte[echoCount]{12}; // Echo pins for the ultrasonic sensors

double* distances = new double[echoCount]; // Distances measured by the ultrasonic sensors



// States

short currentTemperature = 22.5; // Current temperature in degrees Celsius

bool autoStopWhenObstacleDetected = true; // Automatically stop when an obstacle is detected

bool restoreSpeedAfterObstacleNotDetected = true; // Restore the speed of the motors after an obstacle is no longer detected
int* interruptedMotorSpeed = new int[2]{0, 0}; // Speed of the motors when interrupted by an obstacle.


Servo servo1;



bool modeSwitchState = digitalRead(ModeSwitch);

int leftMotorSpeed = 0;
int rightMotorSpeed = 0;

// Number of increments for the servo to rotate and take measurements of.
// Default value of 7 will rotate the servo to 0, 30, 60, 90, 120, 150, and 180 degrees.
int ServoIncrementCount = 7; 

double dist;

int* distanceMeasurements = new int[ServoIncrementCount]{0, 0, 0, 0, 0, 0}; // Distances measured by the ultrasonic sensors at each increment


void setup() {

    pinMode(ModeSwitch, INPUT);
    pinMode(MotorStandby, OUTPUT);
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


    Serial.begin(9600);

    HCSR04.begin(triggerPin, echoPins, echoCount);

    servo1.attach(10);
    // servo1.write(90);

    delay(500);

    // Start moving
}




void forwards(int distanceCm) {
    // Move the robot forward for a certain distance
    // Max speed is 255 = 2.26 feet per second = 0.69 meters per second
    
    analogWrite(RightSpeed, 255);
    analogWrite(LeftSpeed, 255);

    delay(1000);
    
    // delay(del * 1000); // Convert distance to time in milliseconds (0.69 m/s)
    analogWrite(RightSpeed, 0);
    analogWrite(LeftSpeed, 0);
}

void turn(int turnAngle) {
    // The distance between the wheels is exactly 12 centimeters.
    // The robot will move at 69cm/s, with one set of wheels moving forward and the other set moving backward.

    // 90 degree turn is equal to 290ms delay.
    // Positive turnAngle will turn to the left (anticlockwise)

    
    int turnTime = (abs(turnAngle) * 290) / 90;
    turnTime = (int)turnTime; // Ensure turnTime is a whole number

    if (turnAngle > 0) {
        // Turn right
        digitalWrite(RightDirection, HIGH);
        digitalWrite(LeftDirection, LOW);
        analogWrite(RightSpeed, 255);
        analogWrite(LeftSpeed, 255);
    } else {
        // Turn left
        analogWrite(RightSpeed, 255);
        analogWrite(LeftSpeed, 255);
        digitalWrite(RightDirection, LOW);
        digitalWrite(LeftDirection, HIGH);
    }
    delay(turnTime);
    analogWrite(RightSpeed, 0);
    analogWrite(LeftSpeed, 0);
}

void loop() {
    while (digitalRead(ModeSwitch) == 1) {
        // Wait for the mode switch to change
        delay(100);
    }

    Serial.print(LTR);
    Serial.print("-");
    Serial.print(LTM);
    Serial.print("-");
    Serial.print(LTL);
    Serial.println();

    // forwards(30);
    // forwards(90); // Turns to the left
}

