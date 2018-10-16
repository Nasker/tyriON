#include <Wire.h>
#include "Adafruit_MCP23017.h"

#define mcpRead0Adress 0
#define mcpRead1Adress 1
#define mcpWrite0Adress 2//Defining address of the chip
#define mcpWrite1Adress 3
#define N_PINS_CHIP 16

Adafruit_MCP23017 mcpRead0;
Adafruit_MCP23017 mcpRead1;
Adafruit_MCP23017 mcpWrite0;
Adafruit_MCP23017 mcpWrite1;

int mcpRead0Last;
int mcpRead1Last;

void setup() {
  Serial.begin(9600);
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
}

void loop() {
  mcpWrite0.writeGPIOAB(mcpRead0.readGPIOAB());
  mcpWrite1.writeGPIOAB(mcpRead1.readGPIOAB());
  if (mcpRead0.readGPIOAB() != mcpRead0Last || mcpRead1.readGPIOAB() != mcpRead1Last) {
    
  }
  delay(500);
}
