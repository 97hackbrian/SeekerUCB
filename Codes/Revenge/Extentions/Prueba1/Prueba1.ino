#include <PS4Controller.h>
//A0:B7:65:55:FC:F2
unsigned long lastTimeStamp = 0;

void notify()
{ 
  int LSx =     PS4.LStickX();
  int LSy =     PS4.LStickY();
  int RSx =     PS4.RStickX();
  int RSy =     PS4.RStickY();
  int DirRigh = PS4.R2();
  int DirLeft = PS4.L2();
  int R1 =      PS4.R1();
  int L1 =      PS4.L1();
  int PUp =     PS4.Up();
  int Pleft =   PS4.Left();
  int PDown =   PS4.Down();
  int PRight =  PS4.Right();
  int PSquare = PS4.Square();
  int PCross  = PS4.Cross();
  int PCircle = PS4.Circle();
  int PTriangle=PS4.Triangle();
  int PL3 = PS4.L3();
  int PR3 = PS4.R3();
  int share = PS4.Share();
  int options = PS4.Options();
  String dataToSend = 
  String(LSx) + "," + String(LSy) + "," +
  String(RSx) + "," + String(RSy) + "," +
  String(DirLeft) + "," + String(DirRigh) + "," +
  String(L1) + "," + String(R1) + "," +
  
  String(PUp) + "," + String(PRight) + "," +
  String(PDown) + "," + String(Pleft) + "," +
  String(PSquare) + "," + String(PCross) + "," +
  String(PCircle) + "," + String(PTriangle) + ","+
  String(PL3) + "," + String(PR3) + ","+
  String(share) + "," + String(options) + ",";

   if (millis() - lastTimeStamp > 50)
  {
    Serial.println(dataToSend);
    lastTimeStamp = millis();
  }
}
void onConnect()
{
  Serial.println("Connected!.");
}

void onDisConnect()
{
  Serial.println("Disconnected!.");    
}


void setup() {
  Serial.begin(115200);
  PS4.attach(notify);
  PS4.attachOnConnect(onConnect);
  PS4.attachOnDisconnect(onDisConnect);
  PS4.begin();
  Serial.println("Ready.");

}

void loop() {
  // put your main code here, to run repeatedly:

}
