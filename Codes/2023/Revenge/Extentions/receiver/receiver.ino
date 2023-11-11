#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include "ESP32Servo.h"
Servo servo1;
#define SIGNAL_TIMEOUT 1000 

unsigned long lastRecvTime = 0;

//Right motor
#define MOTORLEFT_PWM     16
#define MOTORLEFT_DIR_A   4    
#define MOTORLEFT_DIR_B   0

//Left motor
#define MOTORRIGH_PWM    17
#define MOTORRIGH_DIR_A  12        
#define MOTORRIGH_DIR_B  14


const int PWMFreq = 7000; /* 1 KHz */
const int PWMResolution = 8;
const int rightMotorPWMSpeedChannel = 4;
const int leftMotorPWMSpeedChannel = 5;

unsigned long lastTimeStamp = 0;

void setMotorLeft(int value);
void setMotorRigh(int value);
void motor(int left, int righ);
void freno(boolean left, boolean righ, int value);

int Val = 0;
int S_Motor = 0;


struct PacketData
{

  byte M_SLx; 
  byte M_SLy;
  byte M_SRx;
  byte M_SRy;
  byte M_L2;
  byte M_R2;
  byte M_L1;
  byte M_R1;

  byte M_Up;
  byte M_Right;
  byte M_Down;
  byte M_Left;

  byte M_Square;
  byte M_Cross;
  byte M_Circle;
  byte M_Triangle;
  byte M_PL3;
  byte M_PR3;
  byte M_share;
  byte M_options;

};
PacketData receiverData;



// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) 
{
  if (len == 0)
  {
    return;
  }
  memcpy(&receiverData, incomingData, sizeof(receiverData));
  int LSx =     receiverData.M_SLx;
  int LSy =     receiverData.M_SLy;
  int RSx =     receiverData.M_SRx;
  int RSy =     receiverData.M_SRy;
  int L2 = receiverData.M_L2;
  int R2 = receiverData.M_R2;
  int R1 =      receiverData.M_R1;
  int L1 =      receiverData.M_L1;
  int PUp =     receiverData.M_Up;
  int Pleft =   receiverData.M_Right;
  int PDown =   receiverData.M_Down;
  int PRight =  receiverData.M_Left;
  int PSquare = receiverData.M_Square;
  int PCross  = receiverData.M_Cross;
  int PCircle = receiverData.M_Circle;
  int PTriangle=receiverData.M_Triangle;
  int PL3 = receiverData.M_PL3;
  int PR3 = receiverData.M_PR3;
  int share = receiverData.M_share;
  int options = receiverData.M_options;

  Serial.print(LSx);
  Serial.print("  ");
  Serial.print(LSy);
  Serial.print("  ");
  Serial.print(RSx);
  Serial.print("  ");
  Serial.print(RSy);
  Serial.print("  ");
  Serial.print(L2);
  Serial.print("  ");
  Serial.print(R2);
  Serial.print("  ");
  Serial.print(L1);
  Serial.print("  ");
  Serial.print(R1);
  Serial.print("  ");
  Serial.print(PUp);
  Serial.print("  ");
  Serial.print(Pleft);
  Serial.print("  ");
  Serial.print(PDown);
  Serial.print("  ");
  Serial.print(PRight);
  Serial.print("  ");
  Serial.print(PSquare);
  Serial.print("  ");
  Serial.print(PCross);
  Serial.print("  ");
  Serial.print(PCircle);
  Serial.print("  ");
  Serial.print(PTriangle);
  Serial.print("  ");
  Serial.print(PL3);
  Serial.print("  ");
  Serial.print(PR3);
  Serial.print("  ");
  Serial.print(share);
  Serial.print("  ");
  Serial.println(options);
  Wire.beginTransmission(1);
  String dataToSend =String(L2) + "," + String(R2) + ","+ String(L1) + ","+ String(R1) + ","+ String(PUp) + ","+ String(Pleft) + ","+ String(PDown) + ","+ String(PRight) + ","+ String(PSquare) + ","+ String(PCross) + ","+ String(PCircle) + "," +String(PTriangle) + "," +String(PL3) + ","+String(PR3) + ","+String(share) + ","+String(options) + ",";

  
     Wire.write(dataToSend.c_str());
    Wire.endTransmission();
//bajar
   if(share==true){
    //generatePWM(MOTORLEFT_PWM, 250);
    //generatePWM(MOTORRIGH_PWM, 250);
    ledcWrite(leftMotorPWMSpeedChannel, 200);
    ledcWrite(rightMotorPWMSpeedChannel, 200);
    digitalWrite(MOTORLEFT_DIR_A,HIGH);
    digitalWrite(MOTORLEFT_DIR_B,LOW);
   }
   else if(options==true){
    ledcWrite(leftMotorPWMSpeedChannel, 255);
    ledcWrite(rightMotorPWMSpeedChannel, 255);
    digitalWrite(MOTORLEFT_DIR_A,LOW);
    digitalWrite(MOTORLEFT_DIR_B,HIGH);
   }
   
   else{
    digitalWrite(MOTORLEFT_DIR_A,LOW);
    digitalWrite(MOTORLEFT_DIR_B,LOW);
   }

   if(PTriangle==true){
    servo1.write(140);
  }
  else{
    servo1.write(7);
  }
   

  lastRecvTime = millis(); 
}


void setUpPinModes()
{
  pinMode(MOTORRIGH_PWM   , OUTPUT);
  pinMode(MOTORRIGH_DIR_A , OUTPUT);
  pinMode(MOTORRIGH_DIR_B , OUTPUT);
  
  pinMode(MOTORLEFT_PWM   , OUTPUT);
  pinMode(MOTORLEFT_DIR_A , OUTPUT);
  pinMode(MOTORLEFT_DIR_B , OUTPUT);

  //Set up PWM for motor speed

  ledcSetup(rightMotorPWMSpeedChannel, PWMFreq, PWMResolution);
  ledcSetup(leftMotorPWMSpeedChannel, PWMFreq, PWMResolution);  

  ledcAttachPin(MOTORRIGH_PWM ,  rightMotorPWMSpeedChannel);
  ledcAttachPin(MOTORLEFT_PWM ,  leftMotorPWMSpeedChannel);  
  motor(0,0);
}


void generatePWM(int pin, int value) {
  const unsigned long period = 1000;  // Período de la señal PWM en microsegundos (1000 µs = 1 kHz)
  const unsigned long onTime = (value * period) / 255;
  const unsigned long offTime = period - onTime;

  digitalWrite(pin, HIGH);
  delayMicroseconds(onTime);
  digitalWrite(pin, LOW);
  delayMicroseconds(offTime);
}

void setup() 
{
  servo1.attach(27);
  setUpPinModes();
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  //Wire.beginTransmission(1); // Replace 8 with the appropriate I2C address of the receiving device
  Wire.begin(); 
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}


void loop()
{
  //Wire.beginTransmission(1);
  //Check Signal lost.
  unsigned long now = millis();
  if ( now - lastRecvTime > SIGNAL_TIMEOUT ) 
  {
    Serial.println("No Signal");  
  }
}


void setMotorLeft(int value)
{
  if ( value >= 0 )
  {
    digitalWrite(MOTORRIGH_DIR_A, HIGH);
    digitalWrite(MOTORRIGH_DIR_B, LOW);
  }
  else
  {
    digitalWrite(MOTORRIGH_DIR_A, LOW);
    digitalWrite(MOTORRIGH_DIR_B, HIGH);
    value *= -1;
  }
    ledcWrite(leftMotorPWMSpeedChannel, abs(value));
}

void setMotorRigh(int value) {
  if ( value >= 0 )
  {
    digitalWrite(MOTORLEFT_DIR_A, LOW);
    digitalWrite(MOTORLEFT_DIR_B, HIGH);
  }
  else
  {
    digitalWrite(MOTORLEFT_DIR_A, HIGH);
    digitalWrite(MOTORLEFT_DIR_B, LOW);
    value *= -1;
  }

  ledcWrite(rightMotorPWMSpeedChannel, abs(value));
}


void motor(int left, int righ)
{
  setMotorLeft(left);
  setMotorRigh(righ);
}

void freno(boolean left, boolean righ, int value)
{
//  digitalWrite(STANDBY,HIGH);

  if ( left )
  {
    digitalWrite(MOTORRIGH_DIR_A,HIGH);
    digitalWrite(MOTORRIGH_DIR_B,HIGH);

    ledcWrite(rightMotorPWMSpeedChannel, abs(value));
  }

  if ( righ )
  {
    digitalWrite(MOTORLEFT_DIR_A,HIGH);
    digitalWrite(MOTORLEFT_DIR_B,HIGH);

    ledcWrite(leftMotorPWMSpeedChannel, abs(value));
  }
}

void sendViaI2C(const PacketData &data)
{
  
Wire.beginTransmission(1); // Replace 8 with the appropriate I2C address of the receiving device
  Wire.write((uint8_t *)&data, sizeof(data));
  Wire.endTransmission();
}
