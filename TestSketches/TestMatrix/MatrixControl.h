#include <Wire.h>
#include "Adafruit_MCP23017.h"

#define mcpRead0Adress 0
#define mcpRead1Adress 1
#define mcpWrite0Adress 2//Defining address of the chip
#define mcpWrite1Adress 3
#define N_PINS_CHIP 16
#define N_INPUT_CHIPS 2
#define N_OUTPUT_CHIPS 2


class MatrixControl {

    byte mcpReadsToMatrix[16][2] = { {21, 13}, {15, 16}, {9, 7}, {22, 3}, {5, 28}, {15, 24}, {0, 19}, {4, 11},
      {14, 17}, {30, 1}, {29, 10}, {12, 6}, {26, 2}, {25, 27}, {20, 23}, {8, 18}
    };

    byte matrixToMCPWrites[16][2] = { {21, 13}, {15, 16}, {9, 7}, {22, 3}, {5, 28}, {15, 24}, {0, 19}, {4, 11},
      {14, 17}, {30, 1}, {29, 10}, {12, 6}, {26, 2}, {25, 27}, {20, 23}, {8, 18}
    };

    bool matrixLedState[32];

    Adafruit_MCP23017 mcpRead0;
    Adafruit_MCP23017 mcpRead1;
    Adafruit_MCP23017 mcpWrite0;
    Adafruit_MCP23017 mcpWrite1;

    int mcpRead0Last;
    int mcpRead1Last;

  public:

    MatrixControl();
    void getMatrix(bool &ledState[]);
    //void setMatrix(bool[]);
    //void detectChangeCallback((*f)(String, bool[]));
    void testOutputMatrix();
};

MatrixControl::MatrixControl() {
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

void MatrixControl::getMatrix(bool &ledState[]) {
  for (int i = 0; i < N_PINS_CHIP ; i++) {
    int j = 0;
    matrixLedState[mcpReadsToMatrix[i][j]] = mcpRead0.digitalRead(i);
    j++;
    matrixLedState[mcpReadsToMatrix[i][j]] = mcpRead1.digitalRead(i);
  }
  memcpy(matrixLedState, ledState, sizeof(bool) * 32);
}

