#include "Wire.h"
#define pulse_ip 11
#define buzzer 3
#define ledPin 13
#define flag_buzzer 10
#define alpha 0.04
#define nom 4

int valores[16];
void receiveData(int byteCount);

float ontime, offtime, duty, period, dutyans, prom=0;
float duty_limite = 8.5;
const int num_lect = 500; // Número de muestras
float lecturas[num_lect];
int n = 0;

void readSensor(void);
void promSensor(void);
void Buzzer(void);

void setup()
{
  pinMode(nom,OUTPUT);
  Wire.begin(1); // Inicia la comunicación I2C como esclavo con dirección 8
  Wire.onReceive(receiveData); // Configura la función para manejar datos recibidos
  
  pinMode(pulse_ip, INPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(19200);
  for (int i = 0; i < num_lect; i++) {
    lecturas[i] = 0;
  }
  //Calibracion del sensor
  delay(1000);
  for (int i=0;i<num_lect;i++){
      readSensor();
      promSensor();
  }
  duty_limite=prom*1.055;
  Serial.println(duty_limite);
  delay(2000);
  //Fin calibracion del sensor
}

void loop()
{

  int  Hoo= valores[12];
  int  NoM= valores[13];

  if(NoM==true){
    digitalWrite(nom,HIGH);
    digitalWrite(buzzer, HIGH);
  }
  else{
    digitalWrite(nom,LOW);
    
  }
  
  readSensor();
  promSensor();
  Buzzer(Hoo);
  delay(5);
}

void readSensor()
{
  duty = 0;
  ontime = pulseIn(pulse_ip, HIGH);
  offtime = pulseIn(pulse_ip, LOW);
  period = ontime + offtime;
  if (period != 0) {
    duty = (ontime / period) * 100;
  }
  if (duty == 0 || duty >= 15) {
    duty = dutyans;
  }
  dutyans = duty;
}

void promSensor()
{
  prom = (alpha*duty) + ((1-alpha)*prom);
}

void Buzzer(int Hoo){
  Serial.print(prom);
  Serial.print(" ");
  Serial.print(duty_limite);
  Serial.print(" ");
  Serial.println(duty);
  if (prom > duty_limite||Hoo==true) {
    digitalWrite(buzzer, HIGH);
  } else {
    digitalWrite(buzzer, LOW);
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
