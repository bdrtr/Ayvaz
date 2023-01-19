uint8_t receive_data;
uint8_t buffer[30]; // stm32 den gelen 30 bytlık veri
uint32_t toplam=0; // bataryadan gelen verilerin toplamı
uint32_t batarya_toplam=0; // bataryadan gelen max 255 değerli verinin 5 volt(değer)'a indirilmesi
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          String ifade;
#include "SoftwareSerial.h"
#include "Nextion.h"
#include "Receiver.h"
#include "Lora.h"
#include "SpeedCalculator.h"
#include "TempCalculator.h"

int Hall_effect_pin = 2;

void setup() {

  Serial.begin(9600);   // bilgisayar
  Serial3.begin(115200);  // STM32 için 3

  //LORA INITIALIZE
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  AyvazLoRa.begin();

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

void Soc() {
  toplam=0;
  for (int i=0;i<20;i++) {
    toplam+=buffer[i]; //batarya verilerinin toplamı
  }

  batarya_toplam = toplam/51; // batarya verilerinin toplamının max 5 voltluk degerlere cevrilmesi

  if (batarya_toplam <59) {
    buffer[27] = 0; // 59'dan kucukse batarya 0
  } 

  else if (batarya_toplam <84) {
    buffer[27] = map(batarya_toplam,59, 84, 0, 100); // batarya değeri hesaplanması
  }
 
  else {
    buffer[27] = 100; // batarya değeri 84'ten buyukse %100'dür.
  }

}

void loop() {

  receiver(); //stm32 den verilen alınması
  Soc(); //batarya değerinin hesaplanması
  getTemp(); // sıcaklık verilerinin alınması 
  buffer[26] = speed; //hızın gönderilmesi
  buffer[28] = santigrat;
  buffer[29] = batarya_toplam;
  SendLora(); //lora'ya veri gönderilmesi
}
