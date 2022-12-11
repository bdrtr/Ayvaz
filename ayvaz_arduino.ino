int MSG[31],receive_data;

#include "SoftwareSerial.h"
#include "Nextion.h"
//#include "Receiver.h"
#include "Lora.h"
#include "SpeedCalculator.h"
//#include <DallasTemperature.h>
#include "TempCalculator.h"

int Hall_effect_pin = 2;
 

//float santigrat;

void setup() {

  Serial.begin(9600); // bilgisayar
  Serial2.begin(9600); // loradan gelen 

  //TEMP INITIALIZE
    OneWire OneWire(5);  // Kullanilacak dijital pin secildi.
  DallasTemperature DS18B20(&OneWire);
  DeviceAddress DS18B20_adres;
  //TEMP END

  //LORA INITIALIZE
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  FixajSS.begin();
 
  LoraE22Ayarlar();
 
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
 
  delay(500);
  Serial.println("başlıyoruz.. Fixaj.com");
  //LORA END

  // HALL INITIALIZE
    pinMode(Hall_effect_pin,INPUT_PULLUP);
    Timer1.initialize();// kesme varsayılan olarak 1 saniye sayar
    Timer1.attachInterrupt(speedCal); // bu fonksiyon taşmadan sonra çağrılır
    attachInterrupt(digitalPinToInterrupt(Hall_effect_pin),speedInc, RISING); // hall effect yükselen kenarsa
  //HALL END

}

void loop() {

  /*
    if (Serial2.available()){
        Serial.print("speed \n");
        Serial.print(speed);  
        MSG[27] = speed;
      receiver();
      sendWLora();
    }
  */
    Serial.print("speed \n");
    Serial.print(speed); 
    Serial.print("\n");
    getTemp();
    
    for (int i=0;i<31;i++) {
      MSG[i]=i;
    }
    MSG[0] = speed;
    MSG[1] = santigrat;

    sendLora();

}
