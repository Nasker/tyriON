#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBoards.h>        //includes necessaris per el modul d'ethernet i OSC

#include "MatrixControl.h"
#include "EthernetResetInitSeq.h"
#include "PinsAndConstants.h"

EthernetUDP Udp;                //objecte per a connexió udp
IPAddress selfIp(192, 168, 1, 34);  //172, 16, 17, 172 //ip de la teensy i port on escoltem
const unsigned int inPort  = 3342;
IPAddress outIp(192, 168, 1, 130); //ip destí i port on enviarem  192, 168, 1, 20
const unsigned int outPort = 3341;
byte mac[] = { 0x04, 0xE9, 0xE5, 0x03, 0x94, 0x4E }; //mac, patillera

MatrixControl mControl;

void setup() {
  Serial.begin(115200);     //velocitat de comunicació amb el port serie
  Serial.println("Start sketch");
  EthernetResetInitSeq();   //funció (definidad més abaix) de secuencia necesaria per inicialitzar modul ethernet!!
  Ethernet.begin(mac, selfIp);  //arranquem el modul d'ethernet
  Udp.begin(inPort);        //arranquem el port on escoltarem en Udp
}

void loop() {
  mControl.detectChangeCallback(actOnGPIOReadChanges);
  mControl.testInputMatrix();
}

void actOnGPIOReadChanges(String callbackString) {
  if (callbackString == "CHANGED") {
    bool matrixInputState[N_MATRIX_ELEMENTS];
    mControl.getMatrix(matrixInputState);
    mControl.setMatrix(matrixInputState);
    OSCMessage msg("/state");      //creem un missatge OSC amb l'etiqueta /response
    //for (int i = 0; i < N_MATRIX_ELEMENTS; i++) {
    msg.add(matrixInputState);
    //}
    Udp.beginPacket(outIp, outPort);      //Comenvem un paquet de transmissio Udp
    msg.send(Udp);                  //l'enviem
    Udp.endPacket();              //tanquem el paquet
    msg.empty();
  }
}
