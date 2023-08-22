#include "PS4Controller.h"
#include <Wire.h>
// Aaddres ESP32 CAPI    3c:71:bf:fb:c3:fa
unsigned long lastTimeStamp = 0;

void notify()
{
  int DirRigh = PS4.LStickX();
  int DirLeft = PS4.LStickY();
  int Dir1 = PS4.R2();
  int Dir2 = PS4.L2();
  int GD = PS4.R1();
  int GI = PS4.L1();
  int DB = PS4.Down();
  int UB = PS4.Up();
  int CB = PS4.Circle();
  String dataToSend = String(DirRigh) + "," + String(DirLeft) + "," + String(Dir1) + "," + String(Dir2) + "," + String(GI) + "," + String(GD) + "," + String(DB) + "," + String(UB) + "," + String(CB) + ",";
  Wire.beginTransmission(1);
  // Convierte la cadena a una matriz de bytes
  if (millis() - lastTimeStamp > 30)
  {
    // Serial.println(messageString);

    Wire.write(dataToSend.c_str());
    Wire.endTransmission();
    lastTimeStamp = millis();
  }
  /*
  char messageString[200];
  sprintf(messageString, "%4d,%4d,%4d,%4d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d",
  PS4.LStickX(),
  PS4.LStickY(),
  PS4.RStickX(),
  PS4.RStickY(),
  PS4.Left(),
  PS4.Down(),
  PS4.Right(),
  PS4.Up(),
  PS4.Square(),
  PS4.Cross(),
  PS4.Circle(),
  PS4.Triangle(),
  PS4.L1(),
  PS4.R1(),
  PS4.L2(),
  PS4.R2(),
  PS4.L3(),
  PS4.R3();
  //Only needed to print the message properly on serial monitor. Else we dont need it.
  if (millis() - lastTimeStamp > 50)
  {
    Serial.println(messageString);
    lastTimeStamp = millis();
  }
  */
}
void onConnect()
{
  Serial.println("Connected!.");
}

void onDisConnect()
{
  Serial.println("Disconnected!.");
}

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  PS4.attach(notify);
  PS4.attachOnConnect(onConnect);
  PS4.attachOnDisconnect(onDisConnect);
  PS4.begin();
  Serial.println("Ready.");
}

void loop()
{
  // put your main code here, to run repeatedly:
}
