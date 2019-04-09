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

EthernetUDP Udp;                  //objecte per a connexió udp
IPAddress selfIp(192, 168, 1, 35);  //172, 16, 17, 172 //ip de la teensy i port on escoltem
const unsigned int inPort  = 3352;
IPAddress outIp(192, 168, 1, 120); //ip destí i port on enviarem  192, 168, 1, 143
const unsigned int outPort = 9697;
byte mac[] = { 0x04, 0xE9, 0xE5, 0x03, 0x94, 0x5E }; //mac, patillera

LIDARLite lidar;
RTPSmooth smoothie;
RTPSmartRange lidarRange(0, 1, 500, 0, 500);

bool enableSendOSC = false;

void setup() {
  Serial.begin(115200);     //velocitat de comunicació amb el port serie
  Serial.println("STARTING SETUP");
  EthernetResetInitSeq();   //funció (definidad més abaix) de secuencia necesaria per inicialitzar modul ethernet!!
  Ethernet.begin(mac, selfIp);  //arranquem el modul d'ethernet
  Udp.begin(inPort);        //arranquem el port on escoltarem en Udp
  lidar.begin(4, true);
  Serial.println("ENDING SETUP");
}

void loop() {
 /* if(Wire.status() == I2C_TIMEOUT) 
    Wire.resetBus();
  */
  if (millis() > 300000) SCB_AIRCR = 0x05FA0004;   //Reinicia passats 5 minuts, no es elegant però pot evitar penjades inesperades penjantlo abans
  //OSCMsgReceive();    //esperem a rebre missatges OSC
  //if (enableSendOSC) {
  int smoothedReading = smoothie.smooth(lidar.distance());
  lidarRange.getCurrentStep(smoothedReading);
  lidarRange.stepChanged(actOnRangeCallback);
  delay(10);
  //}
}

void OSCMsgReceive() {
  OSCBundle bundleIN;       //paquet generic de missatges OSC, pot contenir whatever
  int size = Udp.parsePacket(); //mirem si hi ha dades als paquets, si rebem algo
  if (size > 0) {   //si rebem algo...
    //Serial.print("Message Received with size: ");
    //Serial.println(size);
    while (size--) {
      bundleIN.fill(Udp.read()); //plenem el bundle amb el que llegim al port Udp
    }
    bundleIN.route("/activateSendOSC", actOnActivateSendOSC);
  }
}

void actOnActivateSendOSC(OSCMessage &msg, int addrOffset) {
  Serial.println("Activate Send OSC Message Received!");
  enableSendOSC = msg.getInt(0) == 1;
}

void actOnRangeCallback(int id, String callbackString, int currentStep, int currentZone) {
  Serial.print("-ID: ");
  Serial.print(id);
  Serial.print("\t");
  Serial.print(callbackString);
  Serial.print("\t-Step: ");
  Serial.println(currentStep);

  OSCMessage msg("/distance");      //creem un missatge OSC amb l'etiqueta /response
  msg.add(currentStep);
  Udp.beginPacket(outIp, outPort);      //Comenvem un paquet de transmissio Udp
  msg.send(Udp);                  //l'enviem
  Udp.endPacket();              //tanquem el paquet
  msg.empty();
}
