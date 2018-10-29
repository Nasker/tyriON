#include "MatrixControl.h"

MatrixControl::MatrixControl() {
  Serial.print("Constructing Matrix");
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
}

void MatrixControl::getMatrix(bool (& matrixLedState) [N_MATRIX_ELEMENTS]) {
  for (int i = 0; i < N_PINS_CHIP ; i++) {
    int j = 0;
    matrixLedState[mcpReadsToMatrix[i][j]] = mcpRead0.digitalRead(i);
    j++;
    matrixLedState[mcpReadsToMatrix[i][j]] = mcpRead1.digitalRead(i);
  }
  //for(int i = 0; i< 32; i++) ledState[i] = matrixLedState
  //memcpy(ledState, matrixLedState, sizeof(matrixLedState) + 1);
}


void MatrixControl::setMatrix(bool (& matrixLedState) [N_MATRIX_ELEMENTS]) {
  for (int i = 0; i < N_MATRIX_ELEMENTS; i++) {
    switch (matrixToMCPWrites[i][0]) {
      case MCP_0: mcpWrite0.digitalWrite(i, matrixLedState[matrixToMCPWrites[i][1]]); break;
      case MCP_1: mcpWrite1.digitalWrite(i, matrixLedState[matrixToMCPWrites[i][1]]); break;
    }
  }
}

void MatrixControl::detectChangeCallback(void (*f)(String)) {
  if (mcpRead0.readGPIOAB() != mcpRead0Last || mcpRead1.readGPIOAB() != mcpRead1Last) {
    (*f) ("CHANGED");
    mcpRead0Last = mcpRead0.readGPIOAB();
    mcpRead1Last = mcpRead1.readGPIOAB();
  }
}

void MatrixControl::testOutputMatrix() {
  for (int i = 0; i < N_PINS_CHIP; i++) {
    Serial.print("A: ");
    Serial.println(i);
    mcpWrite0.digitalWrite(i, HIGH);
    delay(5000);
    mcpWrite0.digitalWrite(i, LOW);
    Serial.print("B: ");
    Serial.println(i);
    mcpWrite1.digitalWrite(i, HIGH);
    delay(5000);
    mcpWrite1.digitalWrite(i, LOW);
  }
}

void MatrixControl::testInputMatrix() {
  Serial.print("1ST MCP ->");
  for (int i = 0; i < N_PINS_CHIP; i++) {
    Serial.print("  -");
    Serial.print(i);
    Serial.print(":");
    mcpRead0.digitalRead(i);
  }
  Serial.print("\n2ND MCP ->");
  for (int i = 0; i < N_PINS_CHIP; i++) {
    Serial.print("  -");
    Serial.print(i);
    Serial.print(":");
    mcpRead1.digitalRead(i);
  }
  Serial.println("\n\n");
  delay(1000);
}
