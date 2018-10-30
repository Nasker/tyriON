#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBoards.h>        //includes necessaris per el modul d'ethernet i OSC

//#include "MatrixControl.h"   // la declaració i implementació de la classe MatrixControl. (DEBUG no l'empra)

#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include "EthernetResetInitSeq.h"
#include "PinsAndConstants.h"

#define mcpRead0Adress 0
#define mcpRead1Adress 1
#define mcpWrite0Adress 2//Defining address of the chip
#define mcpWrite1Adress 3

const byte mcpReadsToMatrix[N_PINS_CHIP][N_INPUT_CHIPS] = {
  {18, 8}, {23, 21}, {27, 26}, {2, 25}, {6, 12}, {10, 29}, {17, 14}, {22, 30},
  {7, 1}, {3, 5}, {19, 9}, {24, 15}, {28, 0}, {11, 16}, {13, 20}, {31, 4}
}; //Should work

const byte matrixToMCPWrites[N_MATRIX_ELEMENTS][2] = {
  {MCP_0, 4}, {MCP_0, 6}, {MCP_1, 10}, {MCP_0, 10}, {MCP_0, 7}, {MCP_1, 9}, {MCP_1, 15}, {MCP_1, 4},
  {MCP_0, 0}, {MCP_1, 12}, {MCP_1, 11}, {MCP_1, 2}, {MCP_0, 13}, {MCP_1, 7}, {MCP_0, 8}, {MCP_0, 3},
  {MCP_0, 2}, {MCP_1, 8}, {MCP_1, 0}, {MCP_1, 1}, {MCP_0, 15}, {MCP_0, 1}, {MCP_0, 5}, {MCP_1, 14},
  {MCP_1, 3}, {MCP_0, 14}, {MCP_0, 12}, {MCP_1, 5}, {MCP_1, 13}, {MCP_0, 11}, {MCP_0, 9}, {MCP_1, 6}
};

EthernetUDP Udp;                //objecte per a connexió udp
IPAddress selfIp(192, 168, 1, 34);  //172, 16, 17, 172 //ip de la teensy i port on escoltem
const unsigned int inPort  = 3342;
IPAddress outIp(192, 168, 1, 130); //ip destí i port on enviarem  192, 168, 1, 20
const unsigned int outPort = 3341;
byte mac[] = { 0x04, 0xE9, 0xE5, 0x03, 0x94, 0x4E }; //mac, patillera

//MatrixControl mControl;

Adafruit_MCP23017 mcpRead0;
Adafruit_MCP23017 mcpRead1;
Adafruit_MCP23017 mcpWrite0;
Adafruit_MCP23017 mcpWrite1;

int mcpRead0Last;
int mcpRead1Last;

void getMatrix(bool (& matrixLedState) [N_MATRIX_ELEMENTS + 1]) {
  for (int i = 0; i < N_PINS_CHIP ; i++) {
    int j = 0;
    matrixLedState[mcpReadsToMatrix[i][j]] = mcpRead0.digitalRead(i);
    j++;
    matrixLedState[mcpReadsToMatrix[i][j]] = mcpRead1.digitalRead(i);
  }
  Serial.println();
  //ledState[i] = matrixLedState
  //memcpy(ledState, matrixLedState, sizeof(matrixLedState) + 1);
}

void printMatrixLedState(bool (& matrixLedState) [N_MATRIX_ELEMENTS + 1]) {
  for (int i = 0; i < N_MATRIX_ELEMENTS; i++) {
    Serial.print(i);
    Serial.print("-");
    Serial.print(matrixLedState[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void setMatrix(bool (& matrixLedState) [N_MATRIX_ELEMENTS + 1]) {
  for (int i = 0; i < N_MATRIX_ELEMENTS; i++) {
    switch (matrixToMCPWrites[i][0]) {
      case MCP_0: mcpWrite0.digitalWrite(i, matrixLedState[matrixToMCPWrites[i][1]]); break;
      case MCP_1: mcpWrite1.digitalWrite(i, matrixLedState[matrixToMCPWrites[i][1]]); break;
    }
  }
}

void setup() {
  Serial.begin(115200);     //velocitat de comunicació amb el port serie
  Serial.println("Start sketch");
  EthernetResetInitSeq();   //funció (definidad més abaix) de secuencia necesaria per inicialitzar modul ethernet!!
  Ethernet.begin(mac, selfIp);  //arranquem el modul d'ethernet
  Udp.begin(inPort);        //arranquem el port on escoltarem en Udp
  matrixInit();
}

void loop() {
  //Serial.println("LOOP!?");
  bool matrixLedState[N_MATRIX_ELEMENTS + 1];
  getMatrix(matrixLedState);
  printMatrixLedState(matrixLedState);
  // detectChangeCallback(actOnGPIOReadChanges); //detecta quan hi ha un canvi en el GPIO d'entrada i crida
  //a la funció que hi ha com a argument (callback)

  //les dos crides següents son per testejar entrades i sortides
  //testInputMatrix();  //printa l'estat dels inputs
  testOutputMatrix(); //loopeja al voltant de tots els outputs encenent-los i apagantlos secuencialment
  // testOutputMatrix2();

  //delay(1000);
}

void matrixInit() {  //Inicialitza els cmcps de la matriu
  Serial.println("Init Constructing Matrix");
  mcpRead0.begin(mcpRead0Adress);
  mcpRead1.begin(mcpRead1Adress);
  mcpWrite0.begin(mcpWrite0Adress);
  mcpWrite1.begin(mcpWrite1Adress);
  for (int i = 0; i < N_PINS_CHIP; i++) {
    mcpWrite0.pinMode(i, OUTPUT);
    mcpWrite1.pinMode(i, OUTPUT);
  }
  for (int i = 0; i < N_PINS_CHIP; i++) {
    mcpWrite0.digitalWrite(i, LOW);
    mcpWrite1.digitalWrite(i, LOW);
  }
  mcpRead0Last = mcpRead0.readGPIOAB();
  mcpRead1Last = mcpRead1.readGPIOAB();
  bool matrixInputState[N_MATRIX_ELEMENTS + 1];
  //getMatrix(matrixInputState);
  //setMatrix(matrixInputState);
  Serial.println("Finish Constructing Matrix");
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

void detectChangeCallback(void (*f)(String)) {
  if (mcpRead0.readGPIOAB() != mcpRead0Last || mcpRead1.readGPIOAB() != mcpRead1Last) {
    (*f) ("CHANGED");
    mcpRead0Last = mcpRead0.readGPIOAB();
    mcpRead1Last = mcpRead1.readGPIOAB();
  }
}

void testOutputMatrixSerial() {
  if (Serial.available() > 0) {
    // read the incoming byte:
    byte incomingByte = Serial.read();
    mcpWrite0.digitalWrite(incomingByte, HIGH);
    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  }
}

void testOutputMatrix() {
  for (int i = 0; i < N_PINS_CHIP; i++) {
    Serial.print("A: ");
    Serial.println(i);
    mcpWrite0.digitalWrite(i, HIGH);
    delay(50);
    mcpWrite0.digitalWrite(i, LOW);
    Serial.print("B: ");
    Serial.println(i);
    mcpWrite1.digitalWrite(i, HIGH);
    delay(50);
    mcpWrite1.digitalWrite(i, LOW);
  }
}

void testOutputMatrix2() {
  for (int i = 0; i < N_PINS_CHIP; i++) {
    mcpWrite0.digitalWrite(i, HIGH);
    mcpWrite1.digitalWrite(i, HIGH);
  }
  delay(100);
  for (int i = 0; i < N_PINS_CHIP; i++) {
    mcpWrite0.digitalWrite(i, LOW);
    mcpWrite1.digitalWrite(i, LOW);
  }
  delay(100);
}

void testInputMatrix() {
  Serial.print("1ST MCP ->");
  for (int i = 0; i < N_PINS_CHIP; i++) {
    Serial.print("  -");
    Serial.print(i);
    Serial.print(":");
    Serial.print(mcpRead0.digitalRead(i));
  }
  Serial.print("\n2ND MCP ->");
  for (int i = 0; i < N_PINS_CHIP; i++) {
    Serial.print("  -");
    Serial.print(i);
    Serial.print(":");
    Serial.print(mcpRead1.digitalRead(i));
  }
  Serial.println("\n\n");
  delay(1000);
}
