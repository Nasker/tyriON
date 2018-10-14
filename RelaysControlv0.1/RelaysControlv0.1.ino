#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBoards.h>        //includes necessaris per el modul d'ethernet i OSC

#include <RTPRelay.h>
#include "EthernetResetInitSeq.h"
#include "PinsAndConstants.h"

EthernetUDP Udp;                //objecte per a connexió udp
IPAddress selfIp(192, 168, 1, 31);  //172, 16, 17, 172 //ip de la teensy i port on escoltem
const unsigned int inPort  = 3312;
IPAddress outIp(192, 168, 1, 131); //ip destí i port on enviarem
const unsigned int outPort = 3311;
byte mac[] = { 0x04, 0xE9, 0xE5, 0x03, 0x94, 0x5E }; //mac, patillera

RTPRelay relaysArray[N_RELAYS] = {
  RTPRelay(0), RTPRelay(1),
  RTPRelay(2), RTPRelay(3),
  RTPRelay(4), RTPRelay(5),
};

RTPRelay switchingLights(7);

void setup() {
  Serial.begin(115200);     //veloctitat de comunicació amb el port serie
  EthernetResetInitSeq();   //funció (definidad més abaix) de secuencia necesaria per inicialitzar modul ethernet!!
  Ethernet.begin(mac, selfIp);  //arranquem el modul d'ethernet
  Udp.begin(inPort);        //arranquem el port on escoltarem en Udp
}

void loop() {
  OSCMsgReceive();    //esperem a rebre missatges OSC
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
    bundleIN.route("/open", actOnOpenDoor);  //"Rutejem el rebut a l'etiqueta i la funció que cidrem mes abaix.
    bundleIN.route("/close", actOnCloseDoor);
    bundleIN.route("/switchLights", actOnSwitchLights);
  }
}

void actOnOpenDoor(OSCMessage &msg, int addrOffset) {
  int relayIndex = msg.getInt(0);    //desempaquetem els Integers que ens venen
  relaysArray[relayIndex].setState(true);
}

void actOnCloseDoor(OSCMessage &msg, int addrOffset) {
  int relayIndex = msg.getInt(0);    //desempaquetem els Integers que ens venen
  relaysArray[relayIndex].setState(false);
}

void actOnSwitchLights(OSCMessage &msg, int addrOffset) {
  switchingLights.switchState();
}

