/*
#include <OneWire.h>
#include <DallasTemperature.h>



//DeviceAddress DS18B20_adres; // sıcaklık
 // sıcaklık
  DeviceAddress DS18B20_adres; // sıcaklık
  OneWire OneWire(5); // Kullanilacak dijital pin secildi.
  DallasTemperature DS18B20(&OneWire);
   // Sicaklik sensoru baslatildi.
  DS18B20.getAddress(DS18B20_adres, 0); // Sensor adresi atandi. 0, 1 adet sensor kullanildigi anlamindadir.
  DS18B20.setResolution(DS18B20_adres, 12); // Sensor cozunurlugu ayarlandi. Cozunurluk arttikca yavas calisir. (9,10,11,12) degerleri verilebilir.
  

void getTemp() {
  DS18B20.requestTemperatures(); // Sensore sicaklik istegi gonderiliyor.
  santigrat = DS18B20.getTempC(DS18B20_adres); // Sicaklik santigrat cinsinden okunuyor.
}
*/


