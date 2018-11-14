#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBoards.h>        //includes necessaris per el modul d'ethernet i OSC

#include <RTPPhotoDiodeTrigger.h>
#include <RTPRelay.h>
#include <RTPButton.h>
#include <RTPPeriodicBang.h>

#include "EthernetResetInitSeq.h"
#include "PinsAndConstants.h"

EthernetUDP Udp;                //objecte per a connexió udp
IPAddress selfIp(192, 168, 1, 37);  //172, 16, 17, 172 //ip de la teensy i port on escoltem
const unsigned int inPort  = 3372;
IPAddress outIp( 192, 168, 1, 10); //ip destí i port on enviarem  192, 168, 1, 143
const unsigned int outPort = 3371;
byte mac[] = { 0x04, 0xE9, 0xE5, 0x03, 0x94, 0x7E }; //mac, patillera

RTPPhotoDiodeTrigger photoDiodesArray[N_INPUTS] = {
  RTPPhotoDiodeTrigger(0, PHOTODIODE_PIN_1ST), RTPPhotoDiodeTrigger(1, PHOTODIODE_PIN_2ND),
  RTPPhotoDiodeTrigger(2, PHOTODIODE_PIN_3RD), RTPPhotoDiodeTrigger(3, PHOTODIODE_PIN_4TH)
};

RTPRelay relaysArray[N_LIGHT_RELAYS] = {
  RTPRelay(LIGHT_RELAY_PIN_1ST), RTPRelay(LIGHT_RELAY_PIN_2ND),
  RTPRelay(LIGHT_RELAY_PIN_3RD), RTPRelay(LIGHT_RELAY_PIN_4TH),
};

RTPRelay laserRelay(LASER_RELAY_PIN, true);
RTPRelay hazerRelay(HAZER_RELAY_PIN, true);

RTPButton plugSensor(0, PLUG_SENSOR_PIN, NORMAL);

RTPPeriodicBang hazerTimer(HAZER_PERIOD_MILLIS);

unsigned long int hazerCounter = 0;

void setup() {
  Serial.begin(115200);     //velocitat de comunicació amb el port serie
  EthernetResetInitSeq();   //funció (definidad més abaix) de secuencia necesaria per inicialitzar modul ethernet!!
  Ethernet.begin(mac, selfIp);  //arranquem el modul d'ethernet
  Udp.begin(inPort);        //arranquem el port on escoltarem en Udp
  for (int i = 0; i < N_INPUTS; i++)
    photoDiodesArray[i].setThreshold(100);
  for (int i = 0; i < N_LIGHT_RELAYS; i++)
    relaysArray[i].setState(false);
}

void loop() {
  OSCMsgReceive();    //esperem a rebre missatges OSC
  for (int i = 0; i < N_INPUTS; i++) {
    photoDiodesArray[i].readnShoot(actOnPhotoDiodeCallbacks);
    //photoDiodesArray[i].printReading();
  }
  //Serial.println();
  plugSensor.callback(actOnPlugSensorCallback);
  hazerTimer.callbackPeriodBang(actOnHazerTimerCallback);
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
    //bundleIN.route("/open", actOnOpenRelay);  //"Rutejem el rebut a l'etiqueta i la funció que cidrem mes abaix.
    //bundleIN.route("/close", actOnCloseRelay);
    bundleIN.route("/reset", actOnResetMessage);
  }
}

void actOnResetMessage(OSCMessage &msg, int addrOffset) {
  Serial.println("Reset message received!");
  for (int i = 0; i < N_LIGHT_RELAYS; i++)
    relaysArray[i].setState(false);
}
/*
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
  }*/

void actOnPhotoDiodeCallbacks(int ID, String callbackString) {
  Serial.println(callbackString);
  if (callbackString == "ON" && !relaysArray[ID].getState() && !plugSensor.pressed()) {
    OSCMessage msg("/state");      //creem un missatge OSC amb l'etiqueta /response
    relaysArray[ID].setState(true);
    Serial.print("ID activated #");
    Serial.println(ID);
    for (int i = 0; i < N_INPUTS; i++) {
      Serial.print(relaysArray[i].getState());
      msg.add(relaysArray[i].getState());
    }
    Serial.println();
    Udp.beginPacket(outIp, outPort);      //Comenvem un paquet de transmissio Udp
    msg.send(Udp);                  //l'enviem
    Udp.endPacket();              //tanquem el paquet
    msg.empty();
  }
}

void actOnPlugSensorCallback (int ID, String callbackString) {
  Serial.println(callbackString);
  if (callbackString == "DECLICK") {
    laserRelay.setState(true);
    OSCMessage msg("/active");      //creem un missatge OSC amb l'etiqueta /response
    msg.add(!plugSensor.pressed());
    Serial.println(!plugSensor.pressed());
    Udp.beginPacket(outIp, outPort);      //Comenvem un paquet de transmissio Udp
    msg.send(Udp);                  //l'enviem
    Udp.endPacket();              //tanquem el paquet
    msg.empty();
  }
  if (callbackString == "CLICK") {
    for (int i = 0; i < N_LIGHT_RELAYS; i++)
      relaysArray[i].setState(false);
    laserRelay.setState(false);
  }
}

void actOnHazerTimerCallback(String callbackString) {
  if (!plugSensor.pressed()) {
    if (hazerCounter < 3) {
      hazerRelay.setState(false);
      hazerCounter++;
    }
    else {
      hazerRelay.setState(true);
      hazerCounter = 0;
    }

  }
}

