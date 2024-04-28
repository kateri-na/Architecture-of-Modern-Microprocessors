#include <Arduino.h>
#include <TM1637Display.h>
#include <DHT.h>

#define CLK 2
#define DIO 3

TM1637Display display(CLK, DIO);

const uint8_t fahrenheit[]={
  SEG_A | SEG_B | SEG_F | SEG_G,
  SEG_A | SEG_E | SEG_F | SEG_G
};

const uint8_t percent[]={
  SEG_A | SEG_B | SEG_C | SEG_F | SEG_G,
  SEG_C | SEG_D | SEG_E | SEG_F | SEG_G
};

#define DHTTYPE DHT11
#define DHTPIN 5

DHT dht = DHT(DHTPIN, DHTTYPE);

void setup() {
  display.setBrightness(0x0f);
  dht.begin();
  delay(5000);
}

void loop() {
  int temp=dht.readTemperature(true);
  display.showNumberDec(temp,false,2,0);
  display.setSegments(fahrenheit,2,2);
  delay(2000);
  display.clear();
  int hum = dht.readHumidity();
  display.showNumberDec(hum, false, 2, 0);
  display.setSegments(percent, 2,2);
  delay(2000);
  display.clear();
}
