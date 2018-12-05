#include "MatrixControl.h"

Adafruit_MCP23017 mcpRead0;
Adafruit_MCP23017 mcpRead1;
Adafruit_MCP23017 mcpWrite0;
Adafruit_MCP23017 mcpWrite1;

int mcpRead0Last;
int mcpRead1Last;

void matrixInit(bool (& matrixInputState) [N_MATRIX_ELEMENTS + 1]) {  //Inicialitza els MCPs de la matriu
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
  //bool matrixInputState[N_MATRIX_ELEMENTS + 1];
  getMatrix(matrixInputState);
  setMatrix(matrixInputState);
  Serial.println("Finish Constructing Matrix");
}

void getMatrix(bool (& matrixLedState) [N_MATRIX_ELEMENTS + 1]) {
  for (int i = 0; i < N_PINS_CHIP ; i++) {
    matrixLedState[mcpReadsToMatrix[i][MCP_0]] = mcpRead0.digitalRead(i);
    matrixLedState[mcpReadsToMatrix[i][MCP_1]] = mcpRead1.digitalRead(i);
  }
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
      case MCP_0: mcpWrite0.digitalWrite(matrixToMCPWrites[i][1], matrixLedState[i]); break;
      case MCP_1: mcpWrite1.digitalWrite(matrixToMCPWrites[i][1], matrixLedState[i]); break;
    }
  }
}

void detectChangeCallback(void (*actOnChangeCustomFunction)(String)) {
  if (mcpRead0.readGPIOAB() != mcpRead0Last || mcpRead1.readGPIOAB() != mcpRead1Last) {
    (*actOnChangeCustomFunction) ("CHANGED");
    mcpRead0Last = mcpRead0.readGPIOAB();
    mcpRead1Last = mcpRead1.readGPIOAB();
  }
}

void testOutputMatrixLoop() {
  for (int i = 0; i < N_MATRIX_ELEMENTS; i++) {
    switch (matrixToMCPWrites[i][0]) {
      case MCP_0:
        mcpWrite0.digitalWrite(matrixToMCPWrites[i][1], true);
        delay(500);
        mcpWrite0.digitalWrite(matrixToMCPWrites[i][1], false);
        break;
      case MCP_1:
        mcpWrite1.digitalWrite(matrixToMCPWrites[i][1], true);
        delay(500);
        mcpWrite1.digitalWrite(matrixToMCPWrites[i][1], false);
        break;
    }
  }
}

void randomBlinkMatrix(bool (& matrixLedState) [N_MATRIX_ELEMENTS + 1]) {
  int index = random(0, 31);
  if (!matrixLedState[index]) {
    switch (matrixToMCPWrites[index][0]) {
      case MCP_0:
        mcpWrite0.digitalWrite(matrixToMCPWrites[index][1], true);
        delay(20);
        mcpWrite0.digitalWrite(matrixToMCPWrites[index][1], false);
        break;
      case MCP_1:
        mcpWrite1.digitalWrite(matrixToMCPWrites[index][1], true);
        delay(20);
        mcpWrite1.digitalWrite(matrixToMCPWrites[index][1], false);
        break;
    }
  }
}

void testOutputMatrixBlink() {
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
