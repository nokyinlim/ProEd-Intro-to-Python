#include <Servo.h>
#include <HCSR04.h>

/*
Nok Yin Lim - 28/7/2025
Driving with functions
Utility functions for driving a robot



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



int createObjectMap(bool shouldStopRobotDuringMeasurement = true) {
    // This function can be used to create a map of objects in front of the robot
    // using the ultrasonic sensors. It will rotate the servo to create a list of distances detected
    // by the ultrasonic sensors.
    // The robot can be stopped during the measurement if shouldStopRobotDuringMeasurement is true.
    
    if (shouldStopRobotDuringMeasurement) {
        // Stop the motors
        analogWrite(RightSpeed, 0);
        analogWrite(LeftSpeed, 0);
        Serial.println("Motors stopped during object map creation.");
    }


    // Rotate the servo to take measurements at each increment
    for (int i = 0; i < ServoIncrementCount; i++) {
        int angle = i * (180 / (ServoIncrementCount - 1));
        servo1.write(angle);
        // delay(2.5 * (180 / (ServoIncrementCount - 1))); // Wait for servo to settle

        distances = HCSR04.measureDistanceCm(currentTemperature);

        distanceMeasurements[i] = distances[0];



        Serial.print("Angle: ");
        Serial.print(angle);
        Serial.print(" | Distance: ");
        if (distanceMeasurements[i] == -1) {
            Serial.println("Out of range");
        } else {
            Serial.print(distanceMeasurements[i]);
            Serial.println(" cm");
        }

        delay(200);
    }

    servo1.write(90); // Return servo to center
    delay(300);

    if (shouldStopRobotDuringMeasurement && restoreSpeedAfterObstacleNotDetected) {
        rightMotorSpeed = interruptedMotorSpeed[0];
        leftMotorSpeed = interruptedMotorSpeed[1];
        analogWrite(RightSpeed, rightMotorSpeed);
        analogWrite(LeftSpeed, leftMotorSpeed);
        Serial.println("Motor speeds restored.");
    }

    int maxIndex = 0;
    for (int i = 1; i < ServoIncrementCount; i++) {
        if (distanceMeasurements[i] > distanceMeasurements[maxIndex]) {
            maxIndex = i;
        }
    }
    int bestAngle = maxIndex;


    return bestAngle;
}


void forwards(5) {
    // Move the robot forward for 5cm
    // Max speed is 255 = 2.26 feet per second = 0.69 meters per second
    
    analogWrite(RightSpeed, 246);
    analogWrite(LeftSpeed, 255);

    delay(85);
    
    // delay(del * 1000); // Convert distance to time in milliseconds (0.69 m/s)
    analogWrite(RightSpeed, 0);
    analogWrite(LeftSpeed, 0);
}

void forwards(10) {
    // Move the robot forward for 10cm
    // Max speed is 255 = 2.26 feet per second = 0.69 meters per second
    
    analogWrite(RightSpeed, 248);
    analogWrite(LeftSpeed, 255);

    delay(150);
    
    // delay(del * 1000); // Convert distance to time in milliseconds (0.69 m/s)
    analogWrite(RightSpeed, 0);
    analogWrite(LeftSpeed, 0);
}




void forwards(20) {
    // Move the robot forward for 50cm
    // Max speed is 255 = 2.26 feet per second = 0.69 meters per second
    
    analogWrite(RightSpeed, 246);
    analogWrite(LeftSpeed, 255);

    delay(293);
    
    // delay(del * 1000); // Convert distance to time in milliseconds (0.69 m/s)
    analogWrite(RightSpeed, 0);
    analogWrite(LeftSpeed, 0);
}

void forwards(40) {
    // Move the robot forward for a certain distance
    // Max speed is 255 = 2.26 feet per second = 0.69 meters per second
    
    analogWrite(RightSpeed, 248);
    analogWrite(LeftSpeed, 255);

    delay(571);
    
    // delay(del * 1000); // Convert distance to time in milliseconds (0.69 m/s)
    analogWrite(RightSpeed, 0);
    analogWrite(LeftSpeed, 0);
}

void forwards(int distance) {
    // Move the robot forward for a certain distance
    // Max speed is 255 = 2.26 feet per second = 0.69 meters per second

    // takes into account acceleration of the robot
    
    analogWrite(RightSpeed, 246);
    analogWrite(LeftSpeed, 255);

    int delayTime = 0;
    switch (distance) {
        case 5:
            delayTime = 85;
            break;
        case 10:
            delayTime = 150;
            break;
        case 20:
            delayTime = 293;
            break;
        case 40:
            delayTime = 571;
            break;
        default:
            delayTime = 1250
            break;
    }

    delay(delayTime);
    
    // delay(del * 1000); // Convert distance to time in milliseconds (0.69 m/s)
    analogWrite(RightSpeed, 0);
    analogWrite(LeftSpeed, 0);
}

void turn(bool reverse = false, bool small = false, bool custom = false) {
    // The distance between the wheels is exactly 12 centimeters.
    // The robot will move at 69cm/s, with one set of wheels moving forward and the other set moving backward.

    // 90 degree turn is equal to 290ms delay.
    // Positive turnAngle will turn to the left (anticlockwise)

    
    // int turnTime = (abs(turnAngle) * 290) / 90;
    // turnTime = (int)turnTime; // Ensure turnTime is a whole number



    if (reverse) {
        // Turn left
        digitalWrite(RightDirection, HIGH);
        digitalWrite(LeftDirection, LOW);
        analogWrite(RightSpeed, 255);
        analogWrite(LeftSpeed, 255);


        delay(293);
    } else {
        // Turn right
        analogWrite(RightSpeed, 255);
        analogWrite(LeftSpeed, 255);
        digitalWrite(RightDirection, LOW);
        digitalWrite(LeftDirection, HIGH);
        delay(293);
    }
    
    analogWrite(RightSpeed, 0);
    analogWrite(LeftSpeed, 0);
    digitalWrite(RightDirection, HIGH); // Set initial direction to forward
    digitalWrite(LeftDirection, HIGH); // Set initial direction to forward
}

// // Read motor speeds and direction
// rightMotorSpeed = analogRead(RightSpeed);
// leftMotorSpeed = analogRead(LeftSpeed);


// int bestAngle = createObjectMap();

// Serial.print("Best angle to travel in to avoid obstacles: ");
// Serial.println(bestAngle * 30);
// Serial.print("Best distance: ");
// Serial.print(distanceMeasurements[bestAngle]);
// Serial.println(" cm");


// int changeInAngle = 90 - (bestAngle * 30);



// Serial.print("Echo Pin ");
// Serial.print(EchoPin);
// Serial.print(": ");
// distances = HCSR04.measureDistanceCm(currentTemperature);

// dist = distances[0];
// if (dist != -1) {
//     Serial.print(dist);
//     Serial.print(" cm");


//     if (dist < 20) {
//         Serial.print(" - Obstacle detected! Nearest object is within 20cm."); Serial.println();

//         if (autoStopWhenObstacleDetected) {

//             // Store the current speed of the motors
//             interruptedMotorSpeed[0] = rightMotorSpeed;
//             interruptedMotorSpeed[1] = leftMotorSpeed;

//             // Stop the motors
//             analogWrite(RightSpeed, 0);
//             analogWrite(LeftSpeed, 0);
//             Serial.println("Motors stopped due to obstacle detection.");
            
            
//         }
//     }
// }

void loop() {

    



    while (digitalRead(ModeSwitch) == 1) {
        // Wait for the mode switch to change
    }



    // forwards(30);
    // forwards(40); // Turns to the left
    // 5 ~ 5
    // 10 ~ 11
    // 40 ~ 45
    // 20 ~ 22

    delay(75);
    forwards(40); // 40 ~45
    delay(75);

    forwards(40);
    // reverse turn = left
    
    delay(75);
    turn();
    delay(75);

    forwards(40);
    delay(75);
    forwards(40);
    delay(75);

    turn();
    delay(75);

    forwards(40);
    delay(50);
    forwards(5);
    delay(50);

    turn();
    delay(75);

    forwards(40);
    delay(75);

    forwards(20);
    delay(75);


    turn(true);
    delay(75);

    forwards(40);
    delay(75);

    turn(true, true);
    delay(75);

    forwards(40);
    delay(75);


}



/*
    if (small) {
        // Turn left
        digitalWrite(RightDirection, HIGH);
        digitalWrite(LeftDirection, LOW);
        analogWrite(RightSpeed, 255);
        analogWrite(LeftSpeed, 255);


        // delay(180);
        delay(210);

        analogWrite(RightSpeed, 0);
        analogWrite(LeftSpeed, 0);
        digitalWrite(RightDirection, HIGH); // Set initial direction to forward
        digitalWrite(LeftDirection, HIGH); // Set initial direction to forward

        return;
    } else if (custom) {
        // Turn left
        digitalWrite(RightDirection, HIGH);
        digitalWrite(LeftDirection, LOW);
        analogWrite(RightSpeed, 255);
        analogWrite(LeftSpeed, 255);


        delay(210);

        analogWrite(RightSpeed, 0);
        analogWrite(LeftSpeed, 0);
        digitalWrite(RightDirection, HIGH); // Set initial direction to forward
        digitalWrite(LeftDirection, HIGH); // Set initial direction to forward

        return;
    }


    Old code
    delay(75);
    forwards(40); // 40 ~45
    delay(75);

    forwards(40);
    // reverse turn = left
    
    delay(75);
    turn();
    delay(75);

    forwards(40);
    delay(75);
    forwards(40);
    delay(75);

    turn();
    delay(75);

    forwards(40);
    delay(75);
    forwards(5);
    delay(75);

    turn();
    delay(75);

    forwards(40);
    delay(75);

    forwards(20);
    delay(75);


    turn(true);
    delay(75);

    forwards(40);
    delay(75);

    turn(true, true);
    delay(75);

    forwards(40);
    forwards(20);
    delay(75);
*/