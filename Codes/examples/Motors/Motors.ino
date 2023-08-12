#include "MotorSeeker.h"

#define RPWMA 11
#define LPWMA 10
#define ENA 9
#define RPWMB 6
#define LPWMB 5
#define ENB 4

// Create an instance of the MotorSeeker class
MotorSeeker motor(RPWMA, LPWMA, ENA, RPWMB, LPWMB, ENB);

void setup() {
  // No need to repeat pinMode here since it's done in MotorSeeker constructor
}

void loop() {
  // Move the motors, for example:
  motor.move(100, 100); // Move forward with both wheels at speed 100
  delay(3000); // Keep moving for 3 second
  motor.move(0, 0); // Stop the motors
  delay(3000); // Wait for 3 second
}
