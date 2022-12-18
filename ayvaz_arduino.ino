int MSG[27], receive_data;

#include "SoftwareSerial.h"
#include "Nextion.h"
#include "Receiver.h"
#include "Lora.h"
#include "SpeedCalculator.h"
//#include <DallasTemperature.h>
#include "TempCalculator.h"

int Hall_effect_pin = 2;

void setup() {

  Serial.begin(9600);   // bilgisayar
  Serial3.begin(9600);  // STM32 için 3

  //LORA INITIALIZE
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  AYVAZ.begin();

  LoraE22Ayarlar();

  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);

  //LORA END

  // HALL INITIALIZE
  pinMode(Hall_effect_pin, INPUT_PULLUP);
  Timer1.initialize();                                                        // kesme varsayılan olarak 1 saniye sayar
  Timer1.attachInterrupt(speedCal);                                           // bu fonksiyon taşmadan sonra çağrılır
  attachInterrupt(digitalPinToInterrupt(Hall_effect_pin), speedInc, RISING);  // hall effect yükselen kenarsa
  //HALL END
}

void loop() {

  receiver();
  
  /*
  for (int a = 0; a < 27; a++) {
    Serial.println(MSG[a]);
  }
  stm32 den gelen kodları yazdırma sekansı*/
  delay(200);
  getTemp();

  MSG[0] = speed;
  MSG[1] = santigrat;

  sendLora();
}
