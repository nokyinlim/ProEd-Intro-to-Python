/*
Turn 90 degrees clockwise
*/

#define RightSpeed 5
#define LeftSpeed 6
#define RightDirection 7
#define LeftDirection 8


void setup() {}

void loop() {
    analogWrite(RightSpeed, 255);
    analogWrite(LeftSpeed, 255);
    digitalWrite(RightDirection, HIGH);
    digitalWrite(LeftDirection, LOW);

    delay(1000); // Wait for 1 second to complete the turn
    
    analogWrite(RightSpeed, 0);
    analogWrite(LeftSpeed, 0);
}