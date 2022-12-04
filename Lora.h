//Transmitter

#include "LoRa_E22.h"
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>


OneWire OneWire(5); // Kullanilacak dijital pin secildi.
DallasTemperature DS18B20(&OneWire);
DeviceAddress DS18B20_adres; // sıcaklık


float santigrat, fahrenheit; // sıcaklık
 
 
//SoftwareSerial mySerial(17, 16); // rx tx
LoRa_E22 AyvazLoRa(&Serial2); // rx2 ve tx2 için
 
#define M0 13
#define M1 12
 
#define Adres 1   //0--65000 arası bir değer girebilirsiniz. Diğer Modüllerden FARKLI olmalı
#define Kanal 20    //Frekans değeri
//E22 için 0--80 arasında bir sayı giriniz. Diğer Modüllerle AYNI olmalı.
//E22 900 için Frekans = 850+kanal değeri.
#define Netid 63  //0--65000 arası bir değer girebilirsiniz. Diğer Modüllerle AYNI olmalı.
 
#define GonderilecekAdres 2

int LENGTH = 31;


// Batarya verileri
struct MessageBattery {
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
  byte SOC[5];
  byte SOH[5];
  byte temp1[5];
  byte temp2[5];
  byte temp3[5];
  byte temp4[5];
  byte temp5[5];
  byte temp6[5];
  byte temp7[5];
  byte temp8[5];
  byte speed[5];
} dataBattery;

/*
// Sicaklik verileri
struct MessageTemperature{

} dataTemp;
*/
/*
struct MessageSpeed{
  byte speed[9];  
} dataSpeed;
*/

void initBatteryValues(int MSG[], int LENGTH){

  *(float*)dataBattery.bat1 = MSG[0];
   *(float*)dataBattery.bat2 = MSG[1];
   *(float*)dataBattery.bat3 = MSG[2];
   *(float*)dataBattery.bat4 = MSG[3];
   *(float*)dataBattery.bat5 = MSG[4];
   *(float*)dataBattery.bat6 = MSG[5];
   *(float*)dataBattery.bat7 = MSG[6];
   *(float*)dataBattery.bat8 = MSG[7];
   *(float*)dataBattery.bat9 = MSG[8];
   *(float*)dataBattery.bat10 = MSG[9];
   *(float*)dataBattery.bat11 = MSG[10];
   *(float*)dataBattery.bat12 = MSG[11];
   *(float*)dataBattery.bat13 = MSG[12];
   *(float*)dataBattery.bat14 = MSG[13];
   *(float*)dataBattery.bat15 = MSG[14];
   *(float*)dataBattery.bat16 = MSG[15];
   *(float*)dataBattery.bat17 = MSG[16];
   *(float*)dataBattery.bat18 = MSG[17];
   *(float*)dataBattery.bat19 = MSG[18];
   *(float*)dataBattery.bat20 = MSG[19];
   *(float*)dataBattery.SOC = MSG[20];
   *(float*)dataBattery.SOH = MSG[21];
   *(float*)dataBattery.temp1 = MSG[22];
   *(float*)dataBattery.temp2 = MSG[23];
   *(float*)dataBattery.temp3 = MSG[24];
   *(float*)dataBattery.temp4 = MSG[25];
   *(float*)dataBattery.temp5 = MSG[26];
   *(float*)dataBattery.temp6 = MSG[27];
   *(float*)dataBattery.temp7 = MSG[28];
   *(float*)dataBattery.temp8 = MSG[29];
   *(float*)dataBattery.speed = MSG[30];
}
 

 
void sendWLora() {
 
 DS18B20.begin();
DS18B20.getAddress(DS18B20_adres, 0); // Sensor adresi atandi. 0, 1 adet sensor kullanildigi anlamindadir.
DS18B20.setResolution(DS18B20_adres, 12); // Sensor cozunurlugu ayarlandi. Cozunurluk arttikca yavas calisir. (9,10,11,12) degerleri verilebilir.
 
  DS18B20.requestTemperatures(); // Sensore sicaklik istegi gonderiliyor.
  santigrat = DS18B20.getTempC(DS18B20_adres); // Sicaklik santigrat cinsinden okunuyor.
  fahrenheit = DS18B20.toFahrenheit(santigrat); // Sicaklik fahrenheit cinsine cevrildi.
  Serial.print("sıcaklık \n");
  Serial.print(santigrat);
  Serial.print("\n");

  /* Ekrana Basiliyor */
  initBatteryValues(MSG, LENGTH);

  // Ilk olarak batarya verileri gonderiliyor.
  ResponseStatus rsBattery = AyvazLoRa.sendFixedMessage(0, GonderilecekAdres, Kanal, &dataBattery, sizeof(MessageBattery));
  Serial.println(rsBattery.getResponseDescription());

/*
  // Sicaklik verileri gonderiliyor.
  ResponseStatus rsTemp = AyvazLoRa.sendFixedMessage(0, GonderilecekAdres, Kanal, &dataTemp, sizeof(MessageTemperature));
  Serial.println(rsTemp.getResponseDescription());
*/
/*
  ResponseStatus rsSpeed = AyvazLoRa.sendFixedMessage(0, GonderilecekAdres, Kanal, &dataSpeed, sizeof(MessageSpeed));
  Serial.println(rsSpeed.getResponseDescription());
  */
  delay(300);
}


void LoraE22Ayarlar() {
 
  digitalWrite(M0, LOW);
  digitalWrite(M1, HIGH);
 
  ResponseStructContainer c = AyvazLoRa.getConfiguration();
  Configuration configuration = *(Configuration*)c.data;
 
  //DEĞİŞEBİLEN AYARLAR
  // Üstte #define kısmında ayarlayınız
  configuration.ADDL = lowByte(Adres);
  configuration.ADDH = highByte(Adres);
  configuration.NETID = Netid;
  configuration.CHAN = Kanal;
 
  //SEÇENEKLİ AYARLAR
  //configuration.SPED.airDataRate = AIR_DATA_RATE_010_24;  //Veri Gönderim Hızı 2,4 varsayılan
  //configuration.SPED.airDataRate = AIR_DATA_RATE_000_03;  //Veri Gönderim Hızı 0,3 En uzak Mesafe
  configuration.SPED.airDataRate = AIR_DATA_RATE_111_625; //Veri Gönderim Hızı 62,5 En Hızlı

  configuration.OPTION.subPacketSetting = SPS_240_00;//veri paket büyüklüğü 240 byte Varsayılan
  //configuration.OPTION.subPacketSetting = SPS_064_10; //veri paket büyüklüğü 64 byte
  //configuration.OPTION.subPacketSetting = SPS_032_11;  //veri paket büyüklüğü 32 en hızlısı
 
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