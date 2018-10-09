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
IPAddress ip(172, 16, 17, 172);   //ip de la teensy i port on escoltem
const unsigned int inPort  = 12001;
IPAddress outIp(172, 16, 17, 173);    //ip destí i port on enviarem
const unsigned int outPort = 12000;
byte mac[] = { 0x04, 0xE9, 0xE5, 0x03, 0x94, 0x5E }; //mac, patillera

RTPButton buttonsArray[N_BUTTONS] = {
  RTPButton(0, PULLUP), RTPButton(1, PULLUP),
  RTPButton(2, PULLUP), RTPButton(3, PULLUP),
  RTPButton(4, PULLUP), RTPButton(5, PULLUP),
  RTPButton(6, PULLUP), RTPButton(7, PULLUP),
  RTPButton(8, PULLUP), RTPButton(9, PULLUP),
};

void setup() {
  Serial.begin(115200);     //veloctitat de comunicació amb el port serie
  EthernetResetInitSeq();   //funció (definidad més abaix) de secuencia necesaria per inicialitzar modul ethernet!!
  Ethernet.begin(mac, ip);  //arranquem el modul d'ethernet
  Udp.begin(inPort);        //arranquem el port on escoltarem en Udp
}

void loop() {
  OSCMsgReceive();    //esperem a rebre missatges OSC
  for (int i = 0; i < N_BUTTONS; i++)
    buttonsArray[i].callback(actOnCallbacks);
}

void actOnCallbacks(String callbackString) {
  int number = 1;
  sendOSCMessage(number);
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
    bundleIN.route("/received", receivedMessageFunction);  //"Rutejem el rebut a l'etiqueta i la funció que cidrem mes abaix.
  }
}

void receivedMessageFunction(OSCMessage &msg, int addrOffset) {
  int argument0 = msg.getInt(0);    //desempaquetem els Integers que ens venen
  Serial.print("-1st Argument: ");             //els imprimim pel port serie
  Serial.print(argument0);
  sendOSCMessage(argument0);         //els enviarem (son variables globals, per aixo no hi ha arguments)
}

void sendOSCMessage(int argument0) {
  OSCMessage msg("/response");      //creem un missatge OSC amb l'etiqueta /response
  msg.add(argument0);             //Hi afegim les coordinades
  Udp.beginPacket(outIp, outPort);      //Comenvem un paquet de transmissio Udp
  msg.send(Udp);                  //l'enviem
  Udp.endPacket();              //tanquem el paquet
  msg.empty();                   //buidem el missatge
  //Serial.println("MESSAGE SEND");
}
