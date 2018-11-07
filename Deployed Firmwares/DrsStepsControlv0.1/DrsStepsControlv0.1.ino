#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBoards.h>        //includes necessaris per el modul d'ethernet i OSC

#include <RTPButton.h>
#include <RTPRelay.h>
#include "EthernetResetInitSeq.h"
#include "PinsAndConstants.h"

EthernetUDP Udp;                //objecte per a connexió udp
IPAddress selfIp(192, 168, 1, 32);  //172, 16, 17, 172 //ip de la teensy i port on escoltem
const unsigned int inPort  = 3322;
IPAddress outIp(192, 168, 1, 10); //ip destí i port on enviarem //192, 168, 1, 10
const unsigned int outPort = 3321;
byte mac[] = { 0x04, 0xE9, 0xE5, 0x03, 0x94, 0x2E }; //mac, patillera

RTPButton inputsArray[N_INPUTS] = {
  RTPButton(0, DR_STEP_PIN_1ST, NORMAL), RTPButton(1, DR_STEP_PIN_2ND, NORMAL),
  RTPButton(2, DR_STEP_PIN_3RD, NORMAL), RTPButton(3, DR_STEP_PIN_4TH, NORMAL),
  RTPButton(4, DR_STEP_PIN_5TH, NORMAL), RTPButton(5, DR_STEP_PIN_6TH, NORMAL),
  RTPButton(6, DR_STEP_PIN_7TH, NORMAL), RTPButton(7, DR_STEP_PIN_8TH, NORMAL)
};

RTPRelay relaysArray[N_RELAYS] = {
  RTPRelay(TUBE_HOLDER_RELAY_PIN, true), RTPRelay(LIGHT_RELAY_PIN, true)
};

void setup() {
  Serial.begin(115200);     //velocitat de comunicació amb el port serie
  EthernetResetInitSeq();   //funció (definidad més abaix) de secuencia necesaria per inicialitzar modul ethernet!!
  Ethernet.begin(mac, selfIp);  //arranquem el modul d'ethernet
  Udp.begin(inPort);        //arranquem el port on escoltarem en Udp
}

void loop() {
  OSCMsgReceive();    //esperem a rebre missatges OSC
  for (int i = 0; i < N_INPUTS; i++)
    inputsArray[i].callback(actOnStepsCallbacks);
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
    bundleIN.route("/open", actOnOpenRelay);  //"Rutejem el rebut a l'etiqueta i la funció que cidrem mes abaix.
    bundleIN.route("/close", actOnCloseRelay);
  }
}

void actOnOpenRelay(OSCMessage &msg, int addrOffset) {
  int relayID = msg.getInt(0);    //desempaquetem els Integers que ens venen
  Serial.print("Opening relay #");
  Serial.println(relayID);
  if (relayID == TUBE_HOLDER_RELAY_ID) {
    relaysArray[TUBE_HOLDER_RELAY_ID].setState(true);
    for (int i = 0; i < 60; i++) {
      relaysArray[LIGHT_RELAY_ID].switchState();
      delay(100);
    }
    relaysArray[LIGHT_RELAY_ID].setState(false);
    relaysArray[TUBE_HOLDER_RELAY_ID].setState(false);
  }
}

void actOnCloseRelay(OSCMessage &msg, int addrOffset) {
  int relayIndex = msg.getInt(0);    //desempaquetem els Integers que ens venen
  Serial.print("Closing relay #");
  Serial.println(relayIndex + 1);
  relaysArray[relayIndex].setState(false);
}

void actOnStepsCallbacks(int ID, String callbackString) {
  if (callbackString == "CLICK" || callbackString == "DECLICK") {
    Serial.print("STEP #");
    Serial.print(ID + 1);
    Serial.print(" ");
    Serial.println(callbackString);
    OSCMessage msg("/state");      //creem un missatge OSC amb l'etiqueta /response
    for (int i = 0; i < N_INPUTS; i++) {
      msg.add(!inputsArray[i].pressed());
      Serial.print(!inputsArray[i].pressed());
    }
    Serial.println();
    Udp.beginPacket(outIp, outPort);      //Comenvem un paquet de transmissio Udp
    msg.send(Udp);                  //l'enviem
    Udp.endPacket();              //tanquem el paquet
    msg.empty();
  }
}
