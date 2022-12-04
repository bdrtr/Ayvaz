int MSG[27],receive_data;
#include "SoftwareSerial.h"
#include "Receiver.h"
#include "Lora.h"


void setup() {

  Serial.begin(9600);
  Serial2.begin(115200);

  //LORA INITIALIZE
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);

  
  Serial1.begin(115200);
  AyvazLoRa.begin();
  DS18B20.begin(); // Sicaklik sensoru baslatildi.
  DS18B20.getAddress(DS18B20_adres, 0); // Sensor adresi atandi. 0, 1 adet sensor kullanildigi anlamindadir.
  DS18B20.setResolution(DS18B20_adres, 12); // Sensor cozunurlugu ayarlandi. Cozunurluk arttikca yavas calisir. (9,10,11,12) degerleri verilebilir.
 
  // Lora ayarlari yapiliyor
  LoraE22Ayarlar();
  
  // Lora Modu (0,0 -> Lora calistirma modu)
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
 
  delay(500);
  Serial.println("Ayvaz 2023 - AKS\nbaşlıyoruz...");

  //LORA END
}


void loop() {

  
    if (Serial1.available()){  
      receiver();
      sendWLora();
    }
}
