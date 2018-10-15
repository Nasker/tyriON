#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBoards.h>        //includes necessaris per el modul d'ethernet i OSC

#include <RTPButton.h>
#include "EthernetResetInitSeq.h"
#include "PinsAndConstants.h"

EthernetUDP Udp;                //objecte per a connexió udp
IPAddress selfIp(192, 168, 1, 31);  //172, 16, 17, 172 //ip de la teensy i port on escoltem
const unsigned int inPort  = 3312;
IPAddress outIp(192, 168, 1, 131); //ip destí i port on enviarem
const unsigned int outPort = 3311;
byte mac[] = { 0x04, 0xE9, 0xE5, 0x03, 0x94, 0x5E }; //mac, patillera

RTPButton buttonsArray[N_BUTTONS] = {
  RTPButton(DOOR_PIN_1ST, NORMAL), RTPButton(DOOR_PIN_2ND, NORMAL),
  RTPButton(DOOR_PIN_3RD, NORMAL), RTPButton(DOOR_PIN_4TH, NORMAL),
  RTPButton(DOOR_PIN_5TH, NORMAL), RTPButton(DOOR_PIN_6TH, NORMAL),
  RTPButton(DOOR_PIN_7TH, NORMAL), RTPButton(DOOR_PIN_8TH, NORMAL)
};

void setup() {
  Serial.begin(115200);     //veloctitat de comunicació amb el port serie
  EthernetResetInitSeq();   //funció (definidad més abaix) de secuencia necesaria per inicialitzar modul ethernet!!
  Ethernet.begin(mac, selfIp);  //arranquem el modul d'ethernet
  Udp.begin(inPort);        //arranquem el port on escoltarem en Udp
}

void loop() {
  for (int i = 0; i < N_BUTTONS; i++)
    buttonsArray[i].callback(actOnButtonCallbacks);
}

void actOnButtonCallbacks(String callbackString) {
  if (callbackString == "CLICK" || callbackString == "DECLICK") {
    OSCMessage msg("/state");      //creem un missatge OSC amb l'etiqueta /response
    for (int i = 0; i < N_BUTTONS; i++)
      msg.add(buttonsArray[i].pressed());
    Udp.beginPacket(outIp, outPort);      //Comenvem un paquet de transmissio Udp
    msg.send(Udp);                  //l'enviem
    Udp.endPacket();              //tanquem el paquet
    msg.empty();
  }
}
