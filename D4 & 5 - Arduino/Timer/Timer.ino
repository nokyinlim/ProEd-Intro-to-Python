#include <Servo.h>

Servo servo;

void setup() {
    servo.attach(10);
    servo.write(0);
    delay(1000);
}

void loop() {

    while (digitalRead(2) == 1) {
        delay(100);
    }

    delay(1000);

    servo.write(0);

    servo.write(180);

    delay(1000);
}






































