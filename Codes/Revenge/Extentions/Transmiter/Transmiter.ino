#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t receiverMacAddress[] = {0xA0,0xB7,0x65,0x55,0xFC,0xF0};  //A0:B7:65:55:FC:F0

//3C:71:BF:FB:C3:F8


int valores[20];

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
PacketData data;

void datos();
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  else
  {
    Serial.println("Succes: Initialized ESP-NOW");
  }

  //esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverMacAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
  else
  {
    Serial.println("Succes: Added peer");
  } 

  pinMode(22,INPUT_PULLUP);
  pinMode(23,INPUT_PULLUP);
}

void loop() {
  datos();

  data.M_SLx = valores[0]; 
  data.M_SLy= valores[1];
  data.M_SRx= valores[2];
  data.M_SRy= valores[3];

  data.M_L2= valores[4];
  data.M_R2= valores[5];
  data.M_L1= valores[6];
  data.M_R1= valores[7];

  data.M_Up= valores[8];
  data.M_Right= valores[9];
  data.M_Down= valores[10];
  data.M_Left= valores[11];

  data.M_Square= valores[12];
  data.M_Cross= valores[13];
  data.M_Circle= valores[14];
  data.M_Triangle= valores[15];
  
  data.M_PL3= valores[16];
  data.M_PR3= valores[17];
  data.M_share= valores[18];
  data.M_options= valores[19];

  esp_err_t result = esp_now_send(receiverMacAddress, (uint8_t *) &data, sizeof(data));
  if (result == ESP_OK) 
  {
    //Serial.println("Sent with success");
  }
  else 
  {
    Serial.println("Error sending the data");
  }    
  
  delay(50);

}

void datos(){
    if (Serial.available()) {
    String receivedData = Serial.readStringUntil('\n'); // Leer datos hasta encontrar un salto de línea
    // Procesar los datos recibidos
    //Serial.println("Datos recibidos: " + receivedData);
    
    // Separar la cadena en valores enteros// Ajusta el tamaño según la cantidad de valores esperados
    int currentIndex = 0;
    String currentValue = "";
    
    for (int i = 0; i < receivedData.length(); i++) {
      char c = receivedData.charAt(i);
      
      if (c == ',') {
        valores[currentIndex] = currentValue.toInt();
        currentValue = "";
        currentIndex++;
        
        if (currentIndex >= 20) { // Ajusta este límite según la cantidad de valores esperados
          break;
        }
      } else {
        currentValue += c;
      }
    }
    
    // Guardar el último valor si hay uno
    if (currentIndex < 20) {
      valores[currentIndex] = currentValue.toInt();
    }
    
    // Imprimir los valores enteros
  for (int i = 0; i < 20; i++) { // Ajusta este bucle según la cantidad de valores esperados
    Serial.print(valores[i]);
    if (i < 19) {
      Serial.print(" "); // Imprimir un espacio entre valores, excepto después del último valor
    }
  }
  Serial.println(); 
  }

}
