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
IPAddress selfIp(192, 168, 1, 31);  //172, 16, 17, 172 //ip de la teensy i port on escoltem
const unsigned int inPort  = 3312;
IPAddress outIp(192, 168, 1, 131); //ip destí i port on enviarem
const unsigned int outPort = 3311;
byte mac[] = { 0x04, 0xE9, 0xE5, 0x03, 0x94, 0x5E }; //mac, patillera

RTPButton inputsArray[N_INPUTS] = {
  RTPButton(DR_STEP_PIN_1ST, NORMAL), RTPButton(DR_STEP_PIN_2ND, NORMAL),
  RTPButton(DR_STEP_PIN_3RD, NORMAL), RTPButton(DR_STEP_PIN_4TH, NORMAL)
};

RTPRelay relaysArray[N_RELAYS] = {
  RTPRelay(RELAY_PIN_1ST), RTPRelay(RELAY_PIN_2ND)
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
    inputsArray[i].callback(actOnButtonCallbacks);
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
  int relayIndex = msg.getInt(0);    //desempaquetem els Integers que ens venen
  Serial.print("Opening relay #");
  Serial.println(relayIndex + 1);
  relaysArray[relayIndex].setState(true);
}

void actOnCloseRelay(OSCMessage &msg, int addrOffset) {
  int relayIndex = msg.getInt(0);    //desempaquetem els Integers que ens venen
  Serial.print("Closing relay #");
  Serial.println(relayIndex + 1);
  relaysArray[relayIndex].setState(false);
}

void actOnButtonCallbacks(String callbackString) {
  if (callbackString == "CLICK" || callbackString == "DECLICK") {
    OSCMessage msg("/state");      //creem un missatge OSC amb l'etiqueta /response
    for (int i = 0; i < N_INPUTS; i++)
      msg.add(inputsArray[i].pressed());
    Udp.beginPacket(outIp, outPort);      //Comenvem un paquet de transmissio Udp
    msg.send(Udp);                  //l'enviem
    Udp.endPacket();              //tanquem el paquet
    msg.empty();
  }
}
