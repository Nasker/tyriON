#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBoards.h>        //includes necessaris per el modul d'ethernet i OSC
#include <Wire.h>
#include <LIDARLite.h>

#include <RTPSmooth.h>
#include <RTPButton.h>
#include <RTPSmartRange.h>

#include "EthernetResetInitSeq.h"
#include "PinsAndConstants.h"

EthernetUDP Udp;                //objecte per a connexió udp
IPAddress selfIp(192, 168, 1, 31);  //172, 16, 17, 172 //ip de la teensy i port on escoltem
const unsigned int inPort  = 3312;
IPAddress outIp(192, 168, 1, 131); //ip destí i port on enviarem
const unsigned int outPort = 3311;
byte mac[] = { 0x04, 0xE9, 0xE5, 0x03, 0x94, 0x5E }; //mac, patillera

LIDARLite lidar;
RTPSmooth smoothie;
RTPSmartRange lidarRange(0, 1, 200, 0, 200);

void setup() {
  Serial.begin(115200);     //veloctitat de comunicació amb el port serie
  EthernetResetInitSeq();   //funció (definidad més abaix) de secuencia necesaria per inicialitzar modul ethernet!!
  Ethernet.begin(mac, selfIp);  //arranquem el modul d'ethernet
  Udp.begin(inPort);        //arranquem el port on escoltarem en Udp
  lidar.begin(4, true);
}

void loop() {
  int smoothedReading = smoothie.smooth(lidar.distance());
  lidarRange.getCurrentStep(smoothedReading);
  lidarRange.stepChanged(actOnRangeCallback);
  Serial.println(smoothedReading);
}

void actOnRangeCallback(int id, String callbackString, int currentStep, int currentZone) {
  Serial.print("-ID: ");
  Serial.print(id);
  Serial.print("\t");
  Serial.print(callbackString);
  Serial.print("\t-Step: ");
  Serial.print(currentStep);

  OSCMessage msg("/state");      //creem un missatge OSC amb l'etiqueta /response
  msg.add(currentStep);
  Udp.beginPacket(outIp, outPort);      //Comenvem un paquet de transmissio Udp
  msg.send(Udp);                  //l'enviem
  Udp.endPacket();              //tanquem el paquet
  msg.empty();
}

