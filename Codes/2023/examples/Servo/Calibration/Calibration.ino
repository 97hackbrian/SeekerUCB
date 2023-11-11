/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/

#include <Servo.h>

Servo servo1;

Servo servo2;



void setup() {
  servo1.attach(A2);
  servo2.attach(A3);
  pinMode(3,OUTPUT);
  digitalWrite(3,LOW);
}

void loop() {
///arriba
servo1.write(0);
servo2.write(190);
delay(2000);
digitalWrite(3,LOW);
delay(5000);


///abajo
servo1.write(190);
servo2.write(0);
digitalWrite(3,HIGH);
delay(5000);

}
