#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <RTPSmooth.h>

#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

RTPSmooth adcAverage;

void setup() {
  Serial.begin(115200);
  initializeSD();
  Serial.println("SD Initialized!");
}

void loop() {
  String dataString = "";
  int average = adcAverage.smooth(analogRead(A0));
  float firebeetleVcc = ( average * (3.3 / 1023.0) ) * 2;
  dataString += String(firebeetleVcc);
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
  dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
  }
  else {
    Serial.println("error opening datalog.txt");
  }
  delay(10);
}
