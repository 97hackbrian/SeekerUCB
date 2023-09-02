#include "Wire.h"
#include "MotorSeeker.h"
//#include "Servo.h"
//#include <SoftPWM.h>
//Servo servo1;
//Servo servo2;

#define RPWMA 11
#define LPWMA 10
#define ENA 9


#define RPWMB 6
#define LPWMB 5
#define ENB 4

#define ho 3
#define flag 14


#define GPWM 12
#define GINB 15
#define GINA 16

int Val = 0;
//int S_Motor = 0;

int valores[16];

MotorSeeker motor(RPWMA, LPWMA, ENA, RPWMB, LPWMB, ENB);

void receiveData(int byteCount);
void setup() {
  // put your setup code here, to run once:
  Wire.begin(1); // Inicia la comunicación I2C como esclavo con dirección 8
  Wire.onReceive(receiveData); // Configura la función para manejar datos recibidos
  
  //servo1.attach(A2);
  //servo2.attach(A3);
  pinMode(ho,OUTPUT);
  pinMode(flag,OUTPUT);

  //pinMode(GPWM,OUTPUT);
  //SoftPWMBegin();
  
  pinMode(GINB,OUTPUT);
  pinMode(GINA,OUTPUT);
  pinMode(GPWM,OUTPUT);
  
  digitalWrite(ho,LOW);

  //servo1.write(0);
  //servo2.write(190);
  delay(2000);
  
  Serial.begin(9600);
}

void loop() {
  int DG = valores[0];
  int UG = valores[1];
  int CJ1  = valores[2];
  int CJ2  = valores[3];
  int Up = valores[4];
  int Left = valores[5];
  int Back = valores[6];
  int Right = valores[7];
  int Elect = valores[8];
  int Run =   valores[9];
  int Circle=valores[10];
  int Gri = valores[11];
  int  Hoo= valores[12];
  int  NoM= valores[13];
  int DG2=valores[14];
  int UG2=valores[15];


  if (CJ1 && !CJ2)  { Val = 110 ;}

  else if (!CJ1 && CJ2) { Val =160 ;}

  else { Val = 0;}

  /*
  int DirRigh = map (LsX,  0 , 127 , 240 , 50  ) ;
  int DirLeft = map (LsX,  -127 , 0 , 50 , 240 ) ;
  DirLeft = constrain (DirLeft, 50 , 240 );
  DirRigh = constrain (DirRigh ,50, 240 );*/

  //if (Run && (Left||Right||Up)&&Val>0 ){
    if(Up&&!Left&&!Right){
      motor.move((Val-15)  , (Val-15));
    }
    else if (Left&&!Up&&!Right)
    {
      motor.move((-Val-13)  , (Val));
    }
    else if (Right&&!Left&&!Up)
    {
      motor.move((Val)  , (-Val-13));
    }
    /*
    int Up = valores[4];
  int Left = valores[5];
  int Right = valores[6];
  int Back = valores[7];
  int Run = valores[8];
    
    */else if (Up&&Right&&!Left)
    {
      motor.move((Val)  , int((Val*0.5)));
    }
    else if (Up&&Left&&!Right)
    {
      motor.move(int((Val*0.5))  , Val);
    }
    else if (Back&&!Left&&!Right&&!Up)
    {
      motor.move(-Val  , -Val);
    }
    else{
      motor.move(0  , 0);
    }
    
  //}

/*
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

//bajar
  if(DG==true&&!Run){
    digitalWrite(ho,LOW);
    //bajar
    //servo1.write(190);
    //servo2.write(0);


    /*
    for(int x=0;x<=190;x++){
      digitalWrite(flag,HIGH);
      digitalWrite(ho,LOW);
      servo1.write(x);
      servo2.write(190-x);
      delay(5);
    }
    */
    digitalWrite(GINA,HIGH);
    digitalWrite(GINB,LOW);
    digitalWrite(GPWM,HIGH);

    //SoftPWMSet(GPWM, 150);
    delay(30);
    digitalWrite(GINA,LOW);
    digitalWrite(GINB,LOW);
    digitalWrite(GPWM,LOW);
    delay(566);
  }
  else{
    digitalWrite(flag,LOW);
  }


  //subir
  if(UG==true&&!Run){
    if(Elect==true){
    digitalWrite(ho,HIGH);
    digitalWrite(flag,HIGH);
  }
  else{
    digitalWrite(ho,LOW);
  }
    ///arriba
    digitalWrite(flag,HIGH);
    digitalWrite(GINA,LOW);
    digitalWrite(GINB,HIGH);
    digitalWrite(GPWM,HIGH);
    //SoftPWMSet(, 170);
    delay(50);
    digitalWrite(GINA,LOW);
    digitalWrite(GINB,LOW);
    digitalWrite(GPWM,LOW);
    delay(500);
  }
  else{
    digitalWrite(flag,LOW);
  }

  if(Elect==true){
    digitalWrite(ho,HIGH);
    digitalWrite(flag,HIGH);
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

      if (currentIndex >= 16) { // Ajusta este límite según la cantidad de valores esperados
        break;
      }
    } else {
      currentValue += c;
    }
  }

  // Guardar el último valor si hay uno
  if (currentIndex < 16) {
    valores[currentIndex] = currentValue.toInt();
  }

  // Imprimir los valores enteros separados por espacios
  for (int i = 0; i < 16; i++) { // Ajusta este bucle según la cantidad de valores esperados
    Serial.print(valores[i]);
    if (i < 3) {
      Serial.print(" "); // Imprimir un espacio entre valores, excepto después del último valor
    }
  }
  Serial.println(); // Imprimir un salto de línea al final
  

}
