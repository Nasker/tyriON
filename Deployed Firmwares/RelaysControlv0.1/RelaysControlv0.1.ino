#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBoards.h>        //includes necessaris per el modul d'ethernet i OSC

#include <RTPRelay.h>
//#include <RTPTeensyWatchDog.h>

#include "EthernetResetInitSeq.h"
#include "PinsAndConstants.h"

EthernetUDP Udp;                //objecte per a connexió udp
IPAddress selfIp(192, 168, 1, 31);  //192, 168, 1, 31 //ip de la teensy i port on escoltem
const unsigned int inPort  = 3312;
IPAddress outIp(192, 168, 1, 143); //192, 168, 1, 10//ip destí i port on enviarem
const unsigned int outPort = 3311;
byte mac[] = { 0x04, 0xE9, 0xE5, 0x03, 0x94, 0x1E }; //mac, patillera

RTPRelay relaysArray[N_RELAYS] = {
  RTPRelay(RELAY_PIN_1ST, true), RTPRelay(RELAY_PIN_2ND, true),
  RTPRelay(RELAY_PIN_3RD, true), RTPRelay(RELAY_PIN_4TH, true),
};

RTPRelay switchingLights(SWITCHING_LIGHTS_PIN, true);

//RTPTeensyWatchDog watchdog;

void setup() {
  Serial.begin(115200);     //velocitat de comunicació amb el port serie
  Serial.println("START SETUP");
  EthernetResetInitSeq();   //funció (definidad més abaix) de secuencia necesaria per inicialitzar modul ethernet!!
  Ethernet.begin(mac, selfIp);  //arranquem el modul d'ethernet
  Udp.begin(inPort);        //arranquem el port on escoltarem en Udp
  //watchdog.init();
  Serial.println("FINISH SETUP");
}

void loop() {
  //watchdog.kick();
  OSCMsgReceive();    //esperem a rebre missatges OSC
  //Serial.println("LOOP");
}


void OSCMsgReceive() {
  OSCBundle bundleIN;       //paquet generic de missatges OSC, pot contenir whatever
  int size = Udp.parsePacket(); //mirem si hi ha dades als paquets, si rebem algo
  if (size > 0) {   //si rebem algo...
    Serial.print("Message Received with size: ");
    Serial.println(size);
    while (size--) {
      bundleIN.fill(Udp.read()); //plenem el bundle amb el que llegim al port Udp
      //if (bundleIN.hasError())
      //Serial.println("ERROR ON PACKET");
    }
    bundleIN.route("/open", actOnOpenRelay);  //"Rutejem el rebut a l'etiqueta i la funció que cridem mes abaix.
    bundleIN.route("/close", actOnCloseRelay);
    bundleIN.route("/switchLights", actOnSwitchLights);
  }
}

void actOnOpenRelay(OSCMessage &msg, int addrOffset) {
  int relayIndex = msg.getInt(0);    //desempaquetem els Integers que ens venen
  Serial.print("Opening relay #");
  Serial.println(relayIndex + 1);
  switch (relayIndex) {
    case TUNNEL_IN_DOOR: relaysArray[relayIndex].setState(true); break;
    case TUNNEL_OUT_DOOR: relaysArray[relayIndex].setState(true); break;
    case RUBYK_DRAWER: relaysArray[relayIndex].setState(true); break;
    case ESCAPE_DOOR: relaysArray[relayIndex].setState(true); break;
  }
}

void actOnCloseRelay(OSCMessage &msg, int addrOffset) {
  int relayIndex = msg.getInt(0);    //desempaquetem els Integers que ens venen
  Serial.print("Closing relay #");
  Serial.println(relayIndex + 1);
  switch (relayIndex) {
    case TUNNEL_IN_DOOR: relaysArray[relayIndex].setState(false); break;
    case TUNNEL_OUT_DOOR: relaysArray[relayIndex].setState(false); break;
    case RUBYK_DRAWER: relaysArray[relayIndex].setState(false); break;
    case ESCAPE_DOOR: relaysArray[relayIndex].setState(false); break;
  }
}

void actOnSwitchLights(OSCMessage &msg, int addrOffset) {
  int state = msg.getInt(0);
  Serial.print("Switching Lights to ");
  Serial.print(state);
  switch (state) {
    case SWITCH_WHITE_BOARD: switchingLights.setState(false); break;
    case SWITCH_RED_LIGHTS: switchingLights.setState(true); break;
  }
}

