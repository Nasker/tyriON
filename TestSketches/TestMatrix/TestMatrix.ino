//#include "MatrixControl.h"
#include <Wire.h>
#include "Adafruit_MCP23017.h"

#define mcpRead0Adress 0
#define mcpRead1Adress 1
#define mcpWrite0Adress 2//Defining address of the chip
#define mcpWrite1Adress 3
#define N_PINS_CHIP 16
#define N_INPUT_CHIPS 2
#define N_OUTPUT_CHIPS 2
//MatrixControl mControl;

Adafruit_MCP23017 mcpRead0;
Adafruit_MCP23017 mcpRead1;
Adafruit_MCP23017 mcpWrite0;
Adafruit_MCP23017 mcpWrite1;

int mcpRead0Last;
int mcpRead1Last;

void setup() {
  Serial.begin(115200);     //velocitat de comunicació amb el port serie
  mcpRead0.begin(mcpRead0Adress);
  mcpRead1.begin(mcpRead1Adress);
  mcpWrite0.begin(mcpWrite0Adress);
  mcpWrite1.begin(mcpWrite1Adress);
  for (int i = 0; i < N_PINS_CHIP; i++) {
    mcpWrite0.pinMode(i, OUTPUT);
    mcpWrite1.pinMode(i, OUTPUT);
  }
  mcpRead0Last = mcpRead0.readGPIOAB();
  mcpRead1Last = mcpRead1.readGPIOAB();
  for (int i = 0; i < N_PINS_CHIP; i++) {
    mcpWrite0.digitalWrite(i, LOW);
    mcpWrite1.digitalWrite(i, LOW);
  }
  Serial.println("Finished setting the matrix up");
}
void loop() {
  Serial.println("FUCK");
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
  delay(50);
  //delay(1000);
  /* mControl.testOutputMatrix();
    bool state[32];
    mControl.getMatrix(state);
    mControl.setMatrix(state);
    mControl.detectChangeCallback(actOnMatrixCallback);*/


}

void actOnMatrixCallback(String callbackString) {
  Serial.println(callbackString);
}
/*
  for (int i = 0; i < N_PINS_CHIP; i++) {
  mcpWrite0.digitalWrite(i, LOW);
  //mcpWrite1.digitalWrite(i, LOW);
  }
  delay(500);
  for (int i = 0; i < N_PINS_CHIP; i++) {
  Serial.print( mcpRead0.digitalRead(i) );
  Serial.print(" ");
  Serial.print( mcpRead1.digitalRead(i) );
  Serial.print(" ");
  }
  Serial.println();*/
/*
  mcpWrite0.writeGPIOAB(mcpRead0.readGPIOAB());
  mcpWrite1.writeGPIOAB(mcpRead1.readGPIOAB());
  actOnGPIOReadChanges();
  delay(500);
  }

  void actOnGPIOReadChanges() {
  if (mcpRead0.readGPIOAB() != mcpRead0Last || mcpRead1.readGPIOAB() != mcpRead1Last) {
   OSCMessage msg("/state");      //creem un missatge OSC amb l'etiqueta /response
   for (int i = 0; i < N_PINS_CHIP; i++) {
     msg.add(mcpRead0.digitalRead(i));
   }
   for (int i = 0; i < N_PINS_CHIP; i++) {
     msg.add(mcpRead1.digitalRead(i));
   }
   Udp.beginPacket(outIp, outPort);      //Comenvem un paquet de transmissio Udp
   msg.send(Udp);                  //l'enviem
   Udp.endPacket();              //tanquem el paquet
   msg.empty();
   mcpRead0Last = mcpRead0.readGPIOAB();
   mcpRead1Last = mcpRead1.readGPIOAB();
  }
  }*/
