#include "MatrixControl.h"

MatrixControl::MatrixControl() {
  Serial.println("Init Constructing Matrix");
  mcpRead0.begin(mcpRead0Adress);
  mcpRead1.begin(mcpRead1Adress);
  mcpWrite0.begin(mcpWrite0Adress);
  mcpWrite1.begin(mcpWrite1Adress);
}

void MatrixControl::initMatrix() {
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
  getMatrix(matrixInputState);
  setMatrix(matrixInputState);
  Serial.println("Finish Constructing Matrix");
}

void MatrixControl::getMatrix(bool (& matrixLedState) [N_MATRIX_ELEMENTS + 1]) {
  for (int i = 0; i < N_PINS_CHIP ; i++) {
    matrixLedState[mcpReadsToMatrix[i][MCP_0]] = mcpRead0.digitalRead(i);
    matrixLedState[mcpReadsToMatrix[i][MCP_1]] = mcpRead1.digitalRead(i);
  }
}


void MatrixControl::setMatrix(bool (& matrixLedState) [N_MATRIX_ELEMENTS + 1]) {
  for (int i = 0; i < N_MATRIX_ELEMENTS; i++) {
    switch (matrixToMCPWrites[i][0]) {
      case MCP_0: mcpWrite0.digitalWrite(matrixToMCPWrites[i][1], matrixLedState[i]); break;
      case MCP_1: mcpWrite1.digitalWrite(matrixToMCPWrites[i][1], matrixLedState[i]); break;
        //case MCP_0: mcpWrite0.digitalWrite(i, matrixLedState[matrixToMCPWrites[i][1]]); break;
        //case MCP_1: mcpWrite1.digitalWrite(i, matrixLedState[matrixToMCPWrites[i][1]]); break;
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

void MatrixControl::testInputMatrix() {
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

void MatrixControl::printMatrixLedState(bool (& matrixLedState) [N_MATRIX_ELEMENTS + 1]) {
  for (int i = 0; i < N_MATRIX_ELEMENTS; i++) {
    Serial.print(i);
    Serial.print("-");
    Serial.print(matrixLedState[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void MatrixControl::testOutputMatrixLoop() {
  for (int i = 0; i < N_MATRIX_ELEMENTS; i++) {
    switch (matrixToMCPWrites[i][0]) {
      case MCP_0:
        mcpWrite0.digitalWrite(matrixToMCPWrites[i][1], true);
        delay(50);
        mcpWrite0.digitalWrite(matrixToMCPWrites[i][1], false);
        break;
      case MCP_1:
        mcpWrite1.digitalWrite(matrixToMCPWrites[i][1], true);
        delay(50);
        mcpWrite1.digitalWrite(matrixToMCPWrites[i][1], false);
        break;
    }
  }
}


void MatrixControl::testOutputMatrixBlink() {
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
