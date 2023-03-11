uint8_t buffer[30]; // stm32 den gelen 26 bayte'lık veri ilk 25 değerde saklanır
uint32_t toplam=0; // bataryadan gelen verilerin toplamı
uint32_t batarya_toplam=0; // bataryadan gelen max 255 değerli verinin 5 volt(değer)'a indirgenmesi
uint32_t time=0;
char myString[] ="";

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          String ifade;

#include "Receiver.h"
//#include "nextion.h"
#include "Nextion.h"
#include "Lora.h"
#include "SD.h"
#include "SpeedCalculator.h"
#include "TempCalculator.h"

int Hall_effect_pin = 2; //hal effetct için harici kesme pini
NexNumber mot_sic_txt = NexNumber(0, 14, "mot_sic_txt");
NexNumber nex_hiz_txt = NexNumber(0, 11, "hiz_txt");
NexGauge nex_hiz_cubuk = NexGauge(0, 1, "hiz_cubuk");

void setup() {
  
  Serial.begin(9600);   // bilgisayar
  Serial3.begin(115200);  // STM32 için 3
  nexInit();

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

void saveSDCard() {
  time = millis(); //çalışmaya başladığından bir itibar o anki milisaniye
  File file = SD.open("copyTelemetri.txt", FILE_WRITE); //SDCART'daki bu dosyaya kaydet
  /*
  file.print(time);
  file.print(*(float*)telemetryDatas.speed);
  file.print(", ");
  file.print(*(float*)telemetryDatas.temp);
  file.print(", ");
  file.print(*(float*)telemetryDatas.SOC);
  file.print(", ");
  file.print(*(float*)telemetryDatas.engine_temp);
  file.print(", ");
  file.print(*(float*)telemetryDatas.total_battery);
  file.print("\n");
  file.close();

  */
  sprintf(myString,"timer=%d speed=%f, temp=%f, soc=%f, engine_temp=%f, total_bat=%f \n",
                                                                                   time,
                                                                                   *(float*)telemetryDatas.speed,
                                                                                   *(float*)telemetryDatas.temp,
                                                                                   *(float*)telemetryDatas.SOC,
                                                                                   *(float*)telemetryDatas.engine_temp,
                                                                                   *(float*)telemetryDatas.total_battery
                                                                                   );
  file.print(myString);
  file.close();
  Serial.println(myString);
}

void loop() {
  //saveSDCard();  
  receiver(); //stm32 den verilen alınması
  Soc(); //batarya değerinin hesaplanması
  getTemp(); // sıcaklık verilerinin alınması 
  //mot_sic_txt.setValue(findHighTemperature());
  buffer[26] = speed; //hızın gönderilmesi
  buffer[28] = santigrat;
  buffer[29] = batarya_toplam;
  mot_sic_txt.setValue(santigrat);
  nex_hiz_txt.setValue(buffer[20]);
  //Serial.println(buffer[20]);
  //Serial.println(buffer[21]);
  nex_hiz_cubuk.setValue(speed);
  SendLora(); //lora'ya veri gönderilmesi
}
