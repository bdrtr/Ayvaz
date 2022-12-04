int MSG[31],receive_data;

#include "SoftwareSerial.h"
//#include "Receiver.h"
#include "Lora.h"
#include "SpeedCalculator.h"
//#include "TempCalculator.h"

int Hall_effect_pin = 2;
//float santigrat;

void setup() {

  Serial.begin(9600); // bilgisayar
  Serial2.begin(9600); // loradan gelen 
  //Serial1.begin(115200);


  //TEMP INITIALIZE
  //DS18B20.begin();
  //attachInterrupt(digitalPinToInterrupt(5),getTemp, CHANGE);
  //TEMP END
  //LORA INITIALIZE
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);


  AyvazLoRa.begin();

  // Lora ayarlari yapiliyor
  LoraE22Ayarlar();
  
  // Lora Modu (0,0 -> Lora calistirma modu)
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
 
  Serial.println("Ayvaz 2023 - AKS\nbaşlıyoruz...");

  //LORA END
  // HALL INITIALIZE

    pinMode(Hall_effect_pin,INPUT_PULLUP);
    Timer1.initialize();
    Timer1.attachInterrupt(speedCal);
    attachInterrupt(digitalPinToInterrupt(Hall_effect_pin),speedInc, RISING);
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

    for (int i=0;i<31;i++) {
      MSG[i]=i;
    }
    MSG[0] = speed;
    MSG[1] = santigrat;
  
  sendWLora();

}
