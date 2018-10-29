#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include "PinsAndConstants.h"

#define mcpRead0Adress 0
#define mcpRead1Adress 1
#define mcpWrite0Adress 2//Defining address of the chip
#define mcpWrite1Adress 3

class MatrixControl {
    /*const byte mcpReadsToMatrix[N_PINS_CHIP][N_INPUT_CHIPS] = {
      {21, 13}, {15, 16}, {9, 7}, {22, 3}, {5, 28}, {15, 24}, {0, 19}, {4, 11},
      {14, 17}, {30, 1}, {29, 10}, {12, 6}, {26, 2}, {25, 27}, {20, 23}, {8, 18}
    }; //Content not correct, but method is

    const byte matrixToMCPWrites[N_MATRIX_ELEMENTS][2] = {
      {0, 6}, {1, 9}, {1, 12}, {1, 3}, {0, 7}, {0, 4}, {1, 11}, {1, 2},
      {0, 15}, {0, 2}, {1, 10}, {1, 7}, {0, 11}, {1, 0}, {0, 8}, {0, 5},
      {1, 1}, {1, 8}, {1, 15}, {1, 6}, {0, 14}, {0, 0}, {0, 3}, {1, 14},
      {1, 5}, {0, 13}, {0, 12}, {1, 13}, {1, 4}, {0, 10}, {0, 9}, {0, 1}
    };
*/
    Adafruit_MCP23017 mcpRead0;
    Adafruit_MCP23017 mcpRead1;
    Adafruit_MCP23017 mcpWrite0;
    Adafruit_MCP23017 mcpWrite1;

    int mcpRead0Last;
    int mcpRead1Last;

  public:
    MatrixControl();
   /* void getMatrix(bool (&ledState)[N_MATRIX_ELEMENTS]);
    void setMatrix(bool (&ledState)[N_MATRIX_ELEMENTS]);
    void detectChangeCallback(void (*f)(String));
    void testOutputMatrix();
    void testInputMatrix();*/
};

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
/*
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
}*/
