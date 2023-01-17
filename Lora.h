/*
 *
 *
 * Ayvaz 2023 ARAC KONTROL SISTEMI
 * LoRa Transmitter (verici) kodları
 *
 *
*/

#include "LoRa_E22.h"
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <TimerOne.h>
#include <Nextion.h>

SoftwareSerial mySerial(10, 11);  // LoRa 1 pinleri
LoRa_E22 AyvazLoRa(&mySerial);  // LoRa 1 tanimlandi

// LoRa 1 konfigurasyon pinleri
#define M0 13
#define M1 12

#define Adres 1  //0--65000 arası bir değer girebilirsiniz. Diğer Modüllerden FARKLI olmalı
//#define Adres2 2
#define Kanal 20  //Frekans değeri
//E22 için 0--80 arasında bir sayı giriniz. Diğer Modüllerle AYNI olmalı.
//E22 900 için Frekans = 850+kanal değeri.
#define Netid 63  //0--65000 arası bir değer girebilirsiniz. Diğer Modüllerle AYNI olmalı.

#define GonderilecekAdres 3  // Target LoRa
// Nextion Degiskenleri
/*
NexNumber nex_hiz_txt = NexNumber(0, 12, "hiz_txt");
NexNumber nex_sarj_yuzde = NexNumber(0, 14, "sarj_yuzde");
NexNumber nex_sarj_km_txt = NexNumber(0, 13, "sarj_km_txt");
NexNumber nex_bat_seviye_txt = NexNumber(0, 11, "bat_seviye_txt");
NexGauge nex_hiz_cubuk = NexGauge(0, 1, "hiz_cubuk");
NexGauge nex_sarj_cubuk = NexGauge(0, 2, "sarj_cubuk");
NexProgressBar nex_bat_doluluk = NexProgressBar(0, 9, "bat_doluluk");
*/


struct Message {
  byte bat1[5];
  byte bat2[5];
  byte bat3[5];
  byte bat4[5];
  byte bat5[5];
  byte bat6[5];
  byte bat7[5];
  byte bat8[5];
  byte bat9[5];
  byte bat10[5];
  byte bat11[5];
  byte bat12[5];
  byte bat13[5];
  byte bat14[5];
  byte bat15[5];
  byte bat16[5];
  byte bat17[5];
  byte bat18[5];
  byte bat19[5];
  byte bat20[5];
  byte SOH[5];
  byte temp1[5];
  byte temp2[5];
  byte temp3[5];
  byte temp4[5];
  byte temp5[5];
} data;

// LoRa'ya gonderilecek
struct MessageData {
  byte speed[9];    // hiz
  byte temp[5];     // en yuksek sicaklik
  byte SOC[5];      // batarya gerilimi
  byte energy[5];   // kalan enerji
} telemetryDatas;

// En yuksek sicakligi bulan fonk.
int findHighTemperature(){

  int highTemperature = 0;
  float temps[5] = {*(float*)data.temp1, *(float*)data.temp2, *(float*)data.temp3, *(float*)data.temp4, *(float*)data.temp5};
  for(int i = 0; i < 5; i++){
    if(temps[i] >= highTemperature) highTemperature = temps[i];
  }

  return highTemperature;

}

// STM'den alinan veriler struct icine yerlestirilerek paketler hazirlaniyor.
void initValues(uint8_t myMSG[]) {

  *(float*)data.bat1 = myMSG[0];
  *(float*)data.bat2 = myMSG[1];
  *(float*)data.bat3 = myMSG[2];
  *(float*)data.bat4 = myMSG[3];
  *(float*)data.bat5 = myMSG[4];
  *(float*)data.bat6 = myMSG[5];
  *(float*)data.bat7 = myMSG[6];
  *(float*)data.bat8 = myMSG[7];
  *(float*)data.bat9 = myMSG[8];
  *(float*)data.bat10 = myMSG[9];
  *(float*)data.bat11 = myMSG[10];
  *(float*)data.bat12 = myMSG[11];
  *(float*)data.bat13 = myMSG[12];
  *(float*)data.bat14 = myMSG[13];
  *(float*)data.bat15 = myMSG[14];
  *(float*)data.bat16 = myMSG[15];
  *(float*)data.bat17 = myMSG[16];
  *(float*)data.bat18 = myMSG[17];
  *(float*)data.bat19 = myMSG[18];
  *(float*)data.bat20 = myMSG[19];
  *(float*)data.SOH = myMSG[21];
  *(float*)data.temp1 = myMSG[22];
  *(float*)data.temp2 = myMSG[23];
  *(float*)data.temp3 = myMSG[24];
  *(float*)data.temp4 = myMSG[25];
  *(float*)data.temp5 = myMSG[26];

  *(float*)telemetryDatas.speed = 20;
  *(float*)telemetryDatas.temp = findHighTemperature();
  *(float*)telemetryDatas.SOC = 30;
  *(float*)telemetryDatas.energy = 40;
}

void SendLora() {
  initValues(buffer);
  ResponseStatus rsData = AyvazLoRa.sendFixedMessage(0, GonderilecekAdres, Kanal, &telemetryDatas, sizeof(MessageData));
  Serial.println(rsData.getResponseDescription());
}

/*
void initNextion(){
  int i;
  int toplam = 0;
  for(int i = 0; i < 20; i++){
    toplam += MSG[i];
  }
  int sarj_yuzde = toplam/20;
  int derece = map(sarj_yuzde,0,100,360,180);
  //nex_sarj_cubuk.setValue((uint32_t)derece);
  //nex_sarj_yuzde.setValue((uint32_t)sarj_yuzde);
  //nex_bat_seviye_txt.setValue((uint32_t)sarj_yuzde);
  //nex_bat_doluluk.setValue((uint32_t)sarj_yuzde);
  nex_hiz_cubuk.setValue(78);
  Serial.println(nex_hiz_txt.setValue(78));
  Serial.write("0xFF");
  Serial.write("0xFF");
  Serial.write("0xFF");
}
*/

// LoRa RF Settings
void LoraE22Ayarlar() {

  digitalWrite(M0, LOW);
  digitalWrite(M1, HIGH);

  ResponseStructContainer c;
  c = AyvazLoRa.getConfiguration();
  Configuration configuration = *(Configuration*)c.data;

  //DEĞİŞEBİLEN AYARLAR
  // Üstte #define kısmında ayarlayınız
  configuration.ADDL = lowByte(Adres);
  configuration.ADDH = highByte(Adres);
  configuration.NETID = Netid;
  configuration.CHAN = Kanal;

  //SEÇENEKLİ AYARLAR
  configuration.SPED.airDataRate = AIR_DATA_RATE_010_24;  //Veri Gönderim Hızı 2,4 varsayılan
  //configuration.SPED.airDataRate = AIR_DATA_RATE_000_03;  //Veri Gönderim Hızı 0,3 En uzak Mesafe
  //configuration.SPED.airDataRate = AIR_DATA_RATE_111_625; //Veri Gönderim Hızı 62,5 En Hızlı

  //configuration.OPTION.subPacketSetting = SPS_240_00;  //veri paket büyüklüğü 240 byte Varsayılan
  //configuration.OPTION.subPacketSetting = SPS_064_10; //veri paket büyüklüğü 64 byte
  configuration.OPTION.subPacketSetting = SPS_032_11;  //veri paket büyüklüğü 32 en hızlısı

  configuration.OPTION.transmissionPower = POWER_22;  //Gonderim Gücü max Varsayılan
  //configuration.OPTION.transmissionPower = POWER_13;
  //configuration.OPTION.transmissionPower = POWER_10;  //Geönderim Gücü min

  //GELİŞMİŞ AYARLAR
  configuration.SPED.uartBaudRate = UART_BPS_9600;
  configuration.SPED.uartParity = MODE_00_8N1;
  configuration.TRANSMISSION_MODE.WORPeriod = WOR_2000_011;
  configuration.OPTION.RSSIAmbientNoise = RSSI_AMBIENT_NOISE_DISABLED;

  //configuration.TRANSMISSION_MODE.WORTransceiverControl = WOR_TRANSMITTER;
  configuration.TRANSMISSION_MODE.WORTransceiverControl = WOR_RECEIVER;

  //configuration.TRANSMISSION_MODE.enableRSSI = RSSI_ENABLED;
  configuration.TRANSMISSION_MODE.enableRSSI = RSSI_DISABLED;

  configuration.TRANSMISSION_MODE.fixedTransmission = FT_FIXED_TRANSMISSION;
  //configuration.TRANSMISSION_MODE.fixedTransmission = FT_TRANSPARENT_TRANSMISSION;

  configuration.TRANSMISSION_MODE.enableRepeater = REPEATER_DISABLED;
  //configuration.TRANSMISSION_MODE.enableRepeater = REPEATER_ENABLED;

  configuration.TRANSMISSION_MODE.enableLBT = LBT_DISABLED;
  // configuration.TRANSMISSION_MODE.enableLBT = LBT_ENABLED;

  // Ayarları KAYDET ve SAKLA
  ResponseStatus rs = AyvazLoRa.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
  c.close();
}