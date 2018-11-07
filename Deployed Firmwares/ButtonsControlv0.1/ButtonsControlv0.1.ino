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
IPAddress selfIp(192, 168, 1, 36);  //172, 16, 17, 172 //ip de la teensy i port on escoltem
const unsigned int inPort  = 3362;
IPAddress outIp(192, 168, 1, 10); //ip destí i port on enviarem  192, 168, 1, 10
const unsigned int outPort = 3361;
byte mac[] = { 0x04, 0xE9, 0xE5, 0x03, 0x94, 0x6E }; //mac, patillera

RTPButton buttonsArray[N_INPUTS] = {
  RTPButton(0, BUTTON_PIN_1ST, PULLUP), RTPButton(1, BUTTON_PIN_2ND, PULLUP),
  RTPButton(2, BUTTON_PIN_3RD, PULLUP), RTPButton(3, BUTTON_PIN_4TH, PULLUP),
  RTPButton(4, BUTTON_PIN_5TH, PULLUP)
};

void setup() {
  Serial.begin(115200);     //velocitat de comunicació amb el port serie
  Serial.println("Initializing setup");
  EthernetResetInitSeq();   //funció (definidad més abaix) de secuencia necesaria per inicialitzar modul ethernet!!
  Ethernet.begin(mac, selfIp);  //arranquem el modul d'ethernet
  Udp.begin(inPort);        //arranquem el port on escoltarem en Udp
  Serial.println("Ending setup");
}

void loop() {
  for (int i = 0; i < N_INPUTS; i++)
    buttonsArray[i].callback(actOnButtonCallbacks);
}

void actOnButtonCallbacks(int ID, String callbackString) {
  if (callbackString == "CLICK" || callbackString == "DECLICK") {
    Serial.print("BUTTON #");
    Serial.print(ID + 1);
    Serial.print(" ");
    Serial.println(callbackString);
    OSCMessage msg("/state");      //creem un missatge OSC amb l'etiqueta /response
    for (int i = 0; i < N_INPUTS; i++)
      msg.add(buttonsArray[i].pressed());
    Udp.beginPacket(outIp, outPort);      //Comenvem un paquet de transmissio Udp
    msg.send(Udp);                  //l'enviem
    Udp.endPacket();              //tanquem el paquet
    msg.empty();
  }
}
