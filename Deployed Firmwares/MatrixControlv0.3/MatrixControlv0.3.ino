#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBoards.h>        //includes necessaris per el modul d'ethernet i OSC

#include "MatrixControl.h"   // la declaració i implementació de la classe MatrixControl
#include "EthernetResetInitSeq.h"

EthernetUDP Udp;                //objecte per a connexió udp
IPAddress selfIp(192, 168, 1, 34);  //172, 16, 17, 172 //ip de la teensy i port on escoltem
const unsigned int inPort  = 3342;
IPAddress outIp(192, 168, 1, 120); //ip destí i port on enviarem  192, 168, 1, 138
const unsigned int outPort = 3341;
byte mac[] = { 0x04, 0xE9, 0xE5, 0x03, 0x94, 0x4E }; //mac, patillera

bool matrixInputState[N_MATRIX_ELEMENTS + 1];

bool enableSendOSC = false;

void setup() {
  Serial.begin(115200);     //velocitat de comunicació amb el port serie
  Serial.println("Start setup");
  EthernetResetInitSeq();   //funció (definida més abaix) de secuencia necesaria per inicialitzar modul ethernet!!
  Ethernet.begin(mac, selfIp);  //arranquem el modul d'ethernet
  Udp.begin(inPort);        //arranquem el port on escoltarem en Udp
  matrixInit(matrixInputState);
  Serial.println("Finish setup!");
  Wire.onError(actOnBusError);
}

void loop() {
  if (millis() > TIME_TO_RESET) SCB_AIRCR = 0x05FA0004;
  OSCMsgReceive();    //esperem a rebre missatges OSC
  getMatrix(matrixInputState);
  setMatrix(matrixInputState);
  randomBlinkMatrix(matrixInputState);
  detectChangeCallback(actOnGPIOReadChanges); //detecta quan hi ha un canvi en el GPIO d'entrada i crida
  delay(20);
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
    //bundleIN.route("/activateSendOSC", actOnActivateSendOS);
    bundleIN.route("/reset", actOnResetMessage);
  }
}

void actOnBusError(void) {
  Serial.print("FAIL - ");
  switch (Wire.status()) {
    case I2C_TIMEOUT:  Serial.print("I2C timeout\n"); Wire.resetBus(); break;
    case I2C_ADDR_NAK: Serial.print("Slave addr not acknowledged\n"); break;
    case I2C_DATA_NAK: Serial.print("Slave data not acknowledged\n"); break;
    case I2C_ARB_LOST: Serial.print("Arbitration Lost, possible pullup problem\n"); Wire.resetBus(); break;
    case I2C_BUF_OVF:  Serial.print("I2C buffer overflow\n"); break;
    case I2C_NOT_ACQ:  Serial.print("Cannot acquire bus, possible stuck SDA/SCL\n"); Wire.resetBus(); break;
    case I2C_DMA_ERR:  Serial.print("DMA Error\n"); break;
    default:           break;
  }
}

/*
  void actOnActivateSendOSC(OSCMessage &msg, int addrOffset) {
  Serial.println("Activate Send OSC Message Received!");
  enableSendOSC = msg.getInt(0) == 1;
  }*/

void actOnResetMessage(OSCMessage &msg, int addrOffset) {
  Serial.println("Reset Message Received!");
  SCB_AIRCR = 0x05FA0004;
}


void actOnGPIOReadChanges(String callbackString) {
  if (callbackString == "CHANGED") {
    Serial.println(callbackString);
    bool matrixInputStateSend[N_MATRIX_ELEMENTS + 1];
    getMatrix(matrixInputStateSend);
    //setMatrix(matrixInputState);
    OSCMessage msg("/state");      //creem un missatge OSC amb l'etiqueta /response
    for (int i = 0; i < N_MATRIX_ELEMENTS; i++) {
      //printMatrixLedState(matrixInputState);
      msg.add(matrixInputStateSend[i]);
    }
    Udp.beginPacket(outIp, outPort);      //Comencem un paquet de transmissio Udp
    msg.send(Udp);                  //l'enviem
    Udp.endPacket();              //tanquem el paquet
    msg.empty();
  }
}
