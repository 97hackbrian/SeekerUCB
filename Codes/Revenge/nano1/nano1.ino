// PROMEDIO 
#define pulse_ip 11
#define buzzer 3
#define ledPin 13
#define flag_hooter 10
//
float m_s=8.80;//variable mina_superficial
float m_e=8.68;//variable mina_enterrada

float ontime, offtime, duty, period, dutyans;
float duty_limite = 7.6;
const int num_lect = 35; // Número de muestras init 10-20
float lecturas[num_lect];
int n = 0;

void readSensor(void);

void setup()
{
  pinMode(pulse_ip, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ledPin,OUTPUT);
  pinMode(flag_hooter,INPUT);
  Serial.begin(19200);
  for (int i = 0; i < num_lect; i++) {
    lecturas[i] = 0;
  }
}

void loop()
{
  readSensor();
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
  if (duty == 0 || duty >= 35) {
    duty = dutyans;
  }
  dutyans = duty;
  //PROMEDIO
  lecturas[n] = duty;
  n = (n + 1) % num_lect;
  float sum = 0;
  for (int i = 0; i < num_lect; i++) {
    sum += lecturas[i];
  }
  float prom = sum / num_lect;
  //FIN PROMEDIO
  
  Serial.println(prom);

  if ((prom > duty_limite) && digitalRead(flag_hooter)==LOW) {
    //digitalWrite(buzzer, HIGH);
    digitalWrite(buzzer,HIGH);
    //delay(100);
    //erial.print("MINA_SUPERFICIAL");
    //Serial.println(prom);
  } 
  /*
  else if ((prom<=m_e)&&(prom>duty_limite)) {
    digitalWrite(buzzer, HIGH);
    //digitalWrite(ledPin,LOW);
    Serial.print("MINA_ENTERRADA");
    Serial.println(prom);
  }*/

  else{
    digitalWrite(buzzer, LOW);
    digitalWrite(ledPin,LOW);
  }
}
