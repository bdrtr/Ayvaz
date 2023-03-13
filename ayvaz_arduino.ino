uint8_t buffer[30]; // stm32 den gelen 26 bayte'lık veri ilk 25 değerde saklanır
uint32_t toplam=0; // bataryadan gelen verilerin toplamı
uint32_t batarya_toplam=0; // bataryadan gelen max 255 değerli verinin 5 volt(değer)'a indirgenmesi
uint32_t time=0;
char myString[]="";
char Bat_prog[]="";

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          String ifade;

#include "Receiver.h"
//#include "nextion.h"
#include "Nextion.h"
#include "Lora.h"
#include "SD.h"
#include "SpeedCalculator.h"
#include "TempCalculator.h"

int Hall_effect_pin = 2; //hal effect için harici kesme pini

//NEXTION VERIABLES
NexNumber mot_sic_txt = NexNumber(0, 14, "mot_sic_txt");
NexNumber nex_hiz_txt = NexNumber(0, 11, "hiz_txt");
NexGauge nex_hiz_cubuk = NexGauge(0, 1, "hiz_cubuk");
NexNumber sar_km_txt = NexNumber(0, 12, "sar_km_txt");
NexGauge sar_cubuk = NexGauge(0, 2, "sar_cubuk");
NexNumber sarj_yuzde = NexNumber(0, 13, "sarj_yuzde");
NexNumber bat_sic_txt = NexNumber(0, 15, "bat_sic_txt");
NexNumber bat_seviye_txt = NexNumber(0, 10, "bat_seviye_txt");
NexProgressBar bat_doluluk = NexProgressBar(0, 8, "bat_doluluk");
NexPicture mot_sic_u_txt = NexPicture(0, 18, "mot_sic_u_txt");
NexPicture bat_sarj_u_txt = NexPicture(0, 17, "bat_sarj_u_txt");
NexPicture bat_sic_u_txt = NexPicture(0, 16, "bat_sic_u_txt");
NexNumber nex_soc = NexNumber(1, 21, "soc");
NexGauge sarj_cubuk = NexGauge(0,2,"sarj_cubuk");
//

void setup() {
  
  Serial.begin(9600);   // bilgisayar
  Serial3.begin(115200);  // STM32 için 3
  nexInit();

  //SDCARD ADD TITLE
  File file = SD.open("copyTelemetri.txt", FILE_WRITE);
  file.println("time ,speed, temp, soc, engine_temp, total_battery");
  file.close();
  ///

  
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

  sprintf(myString,"%u %f, %f, %f, %f, %f \n",
                                                                                   time,
                                                                                   speed,
                                                                                   santigrat,
                                                                                   buffer[27],
                                                                                   santigrat,
                                                                                   batarya_toplam
                                                                                   );
  file.print(myString);
  file.close();
  //Serial.println(myString);
}

void sendNextion() {
  mot_sic_txt.setValue(santigrat);
  nex_hiz_txt.setValue(speed);
  nex_hiz_cubuk.setValue(speed);
  bat_sic_txt.setValue(buffer[20]);
  bat_doluluk.setValue(buffer[27]);
  bat_seviye_txt.setValue(buffer[27]);
  sarj_yuzde.setValue(buffer[27]);
  nex_soc.setValue(buffer[27]);
  sarj_cubuk.setValue(map(buffer[27],0,100,360,180));
  for (int i=0;i<20;i++) {
    double num = buffer[i]/5.05;
    sprintf(myString,"b%d.val=%d%d",i+1,int(num)/10, (int(num)%10));
    sendCommand(myString);
  }
  
}

void loop() { 
  receiver(); //stm32 den verilen alınması
  Soc(); //batarya değerinin hesaplanması
  getTemp(); // sıcaklık verilerinin alınması 
  //mot_sic_txt.setValue(findHighTemperature());
  buffer[26] = speed; //hızın gönderilmesi
  buffer[28] = santigrat;
  buffer[29] = batarya_toplam;
  sendNextion();
  SendLora(); //lora'ya veri gönderilmesi
  //saveSDCard(); 
  
}
