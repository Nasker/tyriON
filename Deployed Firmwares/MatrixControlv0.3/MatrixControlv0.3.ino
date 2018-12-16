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

//bool enableSendOSC = false;

void setup() {
  Serial.begin(115200); //velocitat de comunicació amb el port serie
  Serial.println("Start setup");
  hardVccReset();
  EthernetResetInitSeq();   //funció (definida més abaix) de secuencia necesaria per inicialitzar modul ethernet!!
  Ethernet.begin(mac, selfIp);  //arranquem el modul d'ethernet
  Udp.begin(inPort);        //arranquem el port on escoltarem en Udp
  matrixInit(matrixInputState);
  Serial.println("Finish setup!");
  Wire.setClock(I2C_FREQ);
  Wire.onError(actOnBusError);
  initWatchdog();
}

void loop() {
  //if (millis() > TIME_TO_RESET) resetTeensy();
  kickWatchdog();
  OSCMsgReceive();    //esperem a rebre missatges OSC
  getMatrix(matrixInputState);
  setMatrix(matrixInputState);
  printMatrixLedState(matrixInputState);
  randomBlinkMatrix(matrixInputState);
  detectChangeCallback(actOnGPIOReadChanges); //detecta quan hi ha un canvi en el GPIO d'entrada i crida
  delay(10);
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
    case I2C_TIMEOUT:  Serial.print("I2C timeout\n"); Wire.resetBus(); resetTeensy(); break;
    case I2C_ADDR_NAK: Serial.print("Slave addr not acknowledged\n"); Wire.resetBus(); resetTeensy(); break;
    case I2C_DATA_NAK: Serial.print("Slave data not acknowledged\n"); Wire.resetBus(); resetTeensy(); break;
    case I2C_ARB_LOST: Serial.print("Arbitration Lost, possible pullup problem\n"); resetTeensy(); break;
    case I2C_BUF_OVF:  Serial.print("I2C buffer overflow\n"); resetTeensy(); break;
    case I2C_NOT_ACQ:  Serial.print("Cannot acquire bus, possible stuck SDA/SCL\n"); resetTeensy(); break;
    case I2C_DMA_ERR:  Serial.print("DMA Error\n"); resetTeensy(); break;
    default:           Serial.print("Default Error!\n"); resetTeensy(); break;
  }
}

/*
  void actOnActivateSendOSC(OSCMessage &msg, int addrOffset) {
  Serial.println("Activate Send OSC Message Received!");
  enableSendOSC = msg.getInt(0) == 1;
  }*/

void actOnResetMessage(OSCMessage &msg, int addrOffset) {
  Serial.println("Reset Message Received!");
  resetTeensy();
}

void resetTeensy() {
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

void initWatchdog() {
  noInterrupts();                                         // don't allow interrupts while setting up WDOG
  WDOG_UNLOCK = WDOG_UNLOCK_SEQ1;                         // unlock access to WDOG registers
  WDOG_UNLOCK = WDOG_UNLOCK_SEQ2;
  delayMicroseconds(1);                                   // Need to wait a bit..

  // for this demo, we will use 1 second WDT timeout (e.g. you must reset it in < 1 sec or a boot occurs)
  WDOG_TOVALH = 0x006d;
  WDOG_TOVALL = 0xdd00;

  // This sets prescale clock so that the watchdog timer ticks at 7.2MHz
  WDOG_PRESC  = 0x400;

  // Set options to enable WDT. You must always do this as a SINGLE write to WDOG_CTRLH
  WDOG_STCTRLH |= WDOG_STCTRLH_ALLOWUPDATE |
                  WDOG_STCTRLH_WDOGEN | WDOG_STCTRLH_WAITEN |
                  WDOG_STCTRLH_STOPEN | WDOG_STCTRLH_CLKSRC;
  interrupts();
}

void kickWatchdog() {
  noInterrupts();
  WDOG_REFRESH = 0xA602;
  WDOG_REFRESH = 0xB480;
  interrupts();
}

void hardVccReset() {
  pinMode(HARD_RESET_PIN, OUTPUT);
  Serial.println("HIGH -> 0VCC FOR DEVICE");
  digitalWrite(HARD_RESET_PIN, HIGH);
  delay(500);
  Serial.println("LOW -> 5VCC FOR DEVICE");
  digitalWrite(HARD_RESET_PIN, LOW);
  delay(500);
}

