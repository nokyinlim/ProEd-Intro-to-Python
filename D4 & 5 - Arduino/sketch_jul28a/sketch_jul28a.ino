

/*
 * Nok Yin Lim - 28/7/2025
 * Simple rover movement tutorial sketch.
 */

// Pin number definitions:
#define camera1 0
#define camera2 1
#define SwitchMode 2 // Mode switch Input
#define MotorStandby 3 // Place H-Bridge in standby if LOW, run if HIGH
#define RSpeedMotor 5
#define LSpeedMotor 6 // Speed of the respective motors
#define RDirectionMotor 7
#define LDirectionMotor 8 // Control direction of motor. HIGH: forward; LOW: backward
#define USDetectorDirection 10
#define USDetector 13
#define USPulse 14
#define RLineSensor 0xa0
#define MLineSensor 0xa1
#define LLineSensor 0xa2

void turn(int turnTime, int turnSpeed = 255, bool clockwise = true) {
  // Turn the rover a certain number of degrees
  // clockwise = true for clockwise, false for anticlockwise

  // In order to turn the rover, the directions of the motors must be opposite of the other motor
  
  if (clockwise) {
    digitalWrite(RDirectionMotor, HIGH);
    digitalWrite(LDirectionMotor, LOW);
  } else {
    digitalWrite(RDirectionMotor, LOW);
    digitalWrite(LDirectionMotor, HIGH);
  }
  
  // Set speed to a predefined value
  analogWrite(RSpeedMotor, turnSpeed); // Full speed
  analogWrite(LSpeedMotor, turnSpeed); // Full speed

  delay(turnTime * 1000); // Wait for the turn duration

  // Stop the motors after turning  
  analogWrite(RSpeedMotor, 0);
  analogWrite(LSpeedMotor, 0);
}

void forward(
  float timePeriod, // Time is in seconds
  int movementSpeed = 255, 
  float leftMotor = 1, // 1 for on, 0 for off. Values in between will be multiplied by the speed
  float rightMotor = 1,
  bool reverse = false,
  bool stopAfterMovement = true
) {
  // Moves the rover forward for the specified length of time.

  // If reverse is true, it will move backward instead.
  if (reverse) {
    digitalWrite(RDirectionMotor, LOW);
    digitalWrite(LDirectionMotor, LOW);
  } else {
    digitalWrite(RDirectionMotor, HIGH);
    digitalWrite(LDirectionMotor, HIGH);
  }
  
  // Set the speed of the motors
  analogWrite(RSpeedMotor, movementSpeed * rightMotor);
  analogWrite(LSpeedMotor, movementSpeed * leftMotor);
  
  delay(timePeriod * 1000);

  if (stopAfterMovement) {
    analogWrite(RSpeedMotor, 0);
    analogWrite(LSpeedMotor, 0);
  }

}

bool accelerate(int accelerationTime, bool decelerate = false, int startSpeed = 0, int endSpeed = 255) {
  // Accelerates the rover to full speed in a given time period

  int speedIncrement = 1; // Speed increment for acceleration

  // If decelerate is true, it will decelerate instead of accelerating
  if (decelerate) {
    speedIncrement = -1; // Speed increment for deceleration
  }

  int currentSpeed = startSpeed; // Current speed of the left motor
  
  analogWrite(LSpeedMotor, currentSpeed);
  analogWrite(RSpeedMotor, currentSpeed);

  int steps = (endSpeed - startSpeed) / speedIncrement; // Number of steps to reach full speed

  if (steps <= 0) {
    // The acceleration or deceleration is not valid since the end speed is not greater than the start speed
    Serial.println("Invalid acceleration params. Please check the start and end speeds. \nStart speed: " + String(startSpeed) + ", End speed: " + String(endSpeed));
    return false;
  }

  int stepTime = accelerationTime * 1000 / steps; // Time for each step
  
  for (int i = 0; i < steps; i++) {
    currentSpeed += speedIncrement; // Increment the speed

    // Ensure the speed does not exceed the limits
    if (currentSpeed < 0) {
      currentSpeed = 0; // Minimum speed is 0
    } else if (currentSpeed > 255) {
      currentSpeed = 255; // Maximum speed is 255
    }
    analogWrite(LSpeedMotor, currentSpeed);
    analogWrite(RSpeedMotor, currentSpeed);

    delay(stepTime); // Wait for the step time
  }
  return true;
}

void stop() {
  analogWrite(LSpeedMotor, 0);
  analogWrite(RSpeedMotor, 0);
}

void setup() {
  // init values
  pinMode(SwitchMode, INPUT);
  pinMode(MotorStandby, OUTPUT);
  pinMode(RSpeedMotor, OUTPUT);
  pinMode(LSpeedMotor, OUTPUT);
  pinMode(RDirectionMotor, OUTPUT);
  pinMode(LDirectionMotor, OUTPUT);

  // Set the initial state of the motors
  analogWrite(RSpeedMotor, 0); // Set initial speed to 0
  analogWrite(LSpeedMotor, 0); // Set initial speed to 0
  digitalWrite(RDirectionMotor, HIGH); // Set initial direction to forward
  digitalWrite(LDirectionMotor, HIGH); // Set initial direction to forward
  digitalWrite(MotorStandby, HIGH); // Set motor to run mode

} 

void movementTutorial() {
  #define FORWARD_SPEED 64
  #define TURN_SPEED 64

  forward(2, FORWARD_SPEED);

  forward(2, FORWARD_SPEED, true, true, true); // Move backward for 2 seconds

  forward(2, FORWARD_SPEED, true, false);

  forward(2, FORWARD_SPEED, false, true);

  turn(2, TURN_SPEED);
  turn(2, TURN_SPEED, false);
}

void speedControl() {
  
  forward(2, 255, 0.25, 0.5);

  forward(2, 255, 0.5, 0.25);

  delay(2000);

  accelerate(2, false, 0, 255); // Accelerate to full speed
  delay(2000);
  accelerate(2, true, 255, 0); // Decelerate

  stop();

}

void loop() {
  
  // Move the rover in the following way
  /*
  forward 2s
  backward 2s
  left forward 2s
  right forward 2s
  spin clockwise 2s
  spin anticlockwise 2s
  */


  while (digitalRead(SwitchMode) == 1) {
    // The motor is in standby; do nothing
    delay(100);
  }

  delay(2000);
  
  speedControl();

}
