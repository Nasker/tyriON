#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBoards.h>        //includes necessaris per el modul d'ethernet i OSC

#include "MatrixControl.h"   // la declaració i implementació de la classe MatrixControl. (DEBUG no l'empra)
#include "EthernetResetInitSeq.h"

EthernetUDP Udp;                //objecte per a connexió udp
IPAddress selfIp(192, 168, 1, 34);  //172, 16, 17, 172 //ip de la teensy i port on escoltem
const unsigned int inPort  = 3342;
IPAddress outIp(192, 168, 1, 130); //ip destí i port on enviarem  192, 168, 1, 20
const unsigned int outPort = 3341;
byte mac[] = { 0x04, 0xE9, 0xE5, 0x03, 0x94, 0x4E }; //mac, patillera

void setup() {
  Serial.begin(115200);     //velocitat de comunicació amb el port serie
  Serial.println("Start setup");
  EthernetResetInitSeq();   //funció (definidad més abaix) de secuencia necesaria per inicialitzar modul ethernet!!
  Ethernet.begin(mac, selfIp);  //arranquem el modul d'ethernet
  Udp.begin(inPort);        //arranquem el port on escoltarem en Udp
  matrixInit();
  Serial.println("Finish setup!");
}

void loop() {
  bool matrixLedState[N_MATRIX_ELEMENTS + 1];
  getMatrix(matrixLedState);
  printMatrixLedState(matrixLedState);
  detectChangeCallback(actOnGPIOReadChanges); //detecta quan hi ha un canvi en el GPIO d'entrada i crida
                                              //a la funció que hi ha com a argument (callback)
  //les dos crides següents son per testejar entrades i sortides
  testInputMatrix();  //printa l'estat dels inputs
  testOutputMatrixLoop(); //loopeja al voltant de tots els outputs encenent-los i apagantlos secuencialment
}

void actOnGPIOReadChanges(String callbackString) {
  if (callbackString == "CHANGED") {
    Serial.println(callbackString);
    bool matrixInputState[N_MATRIX_ELEMENTS + 1];
    getMatrix(matrixInputState);
    setMatrix(matrixInputState);
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
