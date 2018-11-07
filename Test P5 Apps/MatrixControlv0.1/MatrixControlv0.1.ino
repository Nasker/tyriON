#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBoards.h>        //includes necessaris per el modul d'ethernet i OSC
#include <Wire.h>
#include "Adafruit_MCP23017.h"

#include "EthernetResetInitSeq.h"
#include "PinsAndConstants.h"

EthernetUDP Udp;                //objecte per a connexió udp
IPAddress selfIp(192, 168, 1, 34);  //172, 16, 17, 172 //ip de la teensy i port on escoltem
const unsigned int inPort  = 3342;
IPAddress outIp(192, 168, 1, 130); //ip destí i port on enviarem  192, 168, 1, 20
const unsigned int outPort = 3341;
byte mac[] = { 0x04, 0xE9, 0xE5, 0x03, 0x94, 0x4E }; //mac, patillera

Adafruit_MCP23017 mcpRead0;
Adafruit_MCP23017 mcpRead1;
Adafruit_MCP23017 mcpWrite0;
Adafruit_MCP23017 mcpWrite1;

int mcpRead0Last;
int mcpRead1Last;

void setup() {
  Serial.begin(115200);     //velocitat de comunicació amb el port serie
  EthernetResetInitSeq();   //funció (definidad més abaix) de secuencia necesaria per inicialitzar modul ethernet!!
  Ethernet.begin(mac, selfIp);  //arranquem el modul d'ethernet
  Udp.begin(inPort);        //arranquem el port on escoltarem en Udp

  mcpRead0.begin(mcpRead0Adress);
  mcpRead1.begin(mcpRead1Adress);
  mcpWrite0.begin(mcpWrite0Adress);
  mcpWrite1.begin(mcpWrite1Adress);
  for (int i = 0; i < N_PINS_CHIP; i++) {
    mcpWrite0.pinMode(i, OUTPUT);
    mcpWrite1.pinMode(i, OUTPUT);
  }
  mcpRead0Last = mcpRead0.readGPIOAB();
  mcpRead1Last = mcpRead1.readGPIOAB();
}

void loop() {
  mcpWrite0.writeGPIOAB(mcpRead0.readGPIOAB());
  mcpWrite1.writeGPIOAB(mcpRead1.readGPIOAB());
  actOnGPIOReadChanges();
  //delay(50);
}

void actOnGPIOReadChanges() {
  if (mcpRead0.readGPIOAB() != mcpRead0Last || mcpRead1.readGPIOAB() != mcpRead1Last) {
    OSCMessage msg("/state");      //creem un missatge OSC amb l'etiqueta /response
    for (int i = 0; i < N_PINS_CHIP; i++) {
      msg.add(mcpRead0.digitalRead(i));
    }
    for (int i = 0; i < N_PINS_CHIP; i++) {
      msg.add(mcpRead1.digitalRead(i));
    }
    Udp.beginPacket(outIp, outPort);      //Comenvem un paquet de transmissio Udp
    msg.send(Udp);                  //l'enviem
    Udp.endPacket();              //tanquem el paquet
    msg.empty();
    mcpRead0Last = mcpRead0.readGPIOAB();
    mcpRead1Last = mcpRead1.readGPIOAB();
  }
}
