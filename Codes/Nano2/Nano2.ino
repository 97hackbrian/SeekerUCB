#include "Wire.h"
#include "MotorSeeker.h"
#include "Servo.h"
Servo servo1;
Servo servo2;

#define RPWMA 11
#define LPWMA 10
#define ENA 9
#define RPWMB 6
#define LPWMB 5
#define ENB 4

#define ho 3

int Val = 0;
int S_Motor = 0;

int valores[9];

MotorSeeker motor(RPWMA, LPWMA, ENA, RPWMB, LPWMB, ENB);

void receiveData(int byteCount);
void setup() {
  // put your setup code here, to run once:
  Wire.begin(1); // Inicia la comunicación I2C como esclavo con dirección 8
  Wire.onReceive(receiveData); // Configura la función para manejar datos recibidos
  
  servo1.attach(A2);
  servo2.attach(A3);
  pinMode(ho,OUTPUT);
  digitalWrite(ho,LOW);

  servo1.write(0);
  servo2.write(190);
  delay(2000);
  
  Serial.begin(9600);
}

void loop() {
  int LsX = valores[0];
  int LsY = valores[1];
  int Dir2  = valores[2];
  int Dir1  = valores[3];
  int G_I = valores[4];
  int G_D = valores[5];
  int D_B = valores[6];
  int U_B = valores[7];
  int C_B = valores[8];
  if (Dir1 && !Dir2)  { Val = 1 ;}
  else if (!Dir1 && Dir2) { Val = -1;}
  else { Val = 0;}
  int DirRigh = map (LsX,  0 , 127 , 240 , 50  ) ;
  int DirLeft = map (LsX,  -127 , 0 , 50 , 240 ) ;
  DirLeft = constrain (DirLeft, 50 , 240 );
  DirRigh = constrain (DirRigh ,50, 240 );

  if (!G_D && !G_I ){
    motor.move((Val * DirRigh)  , (Val * DirLeft));
  }
  else {
    int G_I = valores[4];
    int G_D = valores[5];
    if (G_D && !G_I ){
      motor.move(255, -255);
    }
    else if (!G_D && G_I ) {
      motor.move(-255,255);
    }
  }
  /* for (int i = 0; i <6 ; i++) { // Ajusta este bucle según la cantidad de valores esperados
    Serial.print(valores[i]);
    if (i < 5) {
      Serial.print(" "); // Imprimir un espacio entre valores, excepto después del último valor
    }
  }
  Serial.println();
*/
  if(D_B==true){
    //bajar
    servo1.write(190);
    servo2.write(0);
  }
  else if(U_B==true){
    ///arriba
    servo1.write(0);
    servo2.write(190);
  }

  if(C_B==true){
    digitalWrite(ho,HIGH);
  }
  else{
    digitalWrite(ho,LOW);
  }

}


void receiveData(int byteCount) {
  String receivedData = "";

  while (Wire.available()) {
    char c = Wire.read();
    receivedData += c;
  }

 // Ajusta el tamaño según la cantidad de valores esperados

  int currentIndex = 0;
  String currentValue = "";

  for (int i = 0; i < receivedData.length(); i++) {
    char c = receivedData.charAt(i);

    if (c == ',') {
      valores[currentIndex] = currentValue.toInt();
      currentValue = "";
      currentIndex++;

      if (currentIndex >= 6) { // Ajusta este límite según la cantidad de valores esperados
        break;
      }
    } else {
      currentValue += c;
    }
  }

  // Guardar el último valor si hay uno
  if (currentIndex < 6) {
    valores[currentIndex] = currentValue.toInt();
  }
/*
  // Imprimir los valores enteros separados por espacios
  for (int i = 0; i < 4; i++) { // Ajusta este bucle según la cantidad de valores esperados
    Serial.print(valores[i]);
    if (i < 3) {
      Serial.print(" "); // Imprimir un espacio entre valores, excepto después del último valor
    }
  }
  Serial.println(); // Imprimir un salto de línea al final
  */

}