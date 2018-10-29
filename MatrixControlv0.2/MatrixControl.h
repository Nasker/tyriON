#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include "PinsAndConstants.h"

#define mcpRead0Adress 0
#define mcpRead1Adress 1
#define mcpWrite0Adress 2//Defining address of the chip
#define mcpWrite1Adress 3

class MatrixControl {
    const byte mcpReadsToMatrix[N_PINS_CHIP][N_INPUT_CHIPS] = {
      {21, 21}, {15, 25}, {27, 8}, {22, 26}, {5, 12}, {10, 29}, {0, 14}, {4, 30},
      {14, 1}, {30, 5}, {28, 9}, {12, 0}, {26, 15}, {25, 16}, {13, 4}, {31, 20}
    }; //Content not correct, but method is

    const byte matrixToMCPWrites[N_MATRIX_ELEMENTS][2] = {
      {MCP_0, 6}, {MCP_1, 9}, {MCP_1, 12}, {MCP_1, 3}, {MCP_0, 7}, {MCP_0, 4}, {MCP_1, 11}, {MCP_1, 2},
      {MCP_0, 15}, {MCP_0, 2}, {MCP_1, 10}, {MCP_1, 7}, {MCP_0, 11}, {MCP_1, 0}, {MCP_0, 8}, {MCP_0, 5},
      {MCP_1, 1}, {MCP_1, 8}, {MCP_1, 15}, {MCP_1, 6}, {MCP_0, 14}, {MCP_0, 0}, {MCP_0, 3}, {MCP_1, 14},
      {MCP_1, 5}, {MCP_0, 13}, {MCP_0, 12}, {MCP_1, 13}, {MCP_1, 4}, {MCP_0, 10}, {MCP_0, 9}, {MCP_0, 1}
    };

    Adafruit_MCP23017 mcpRead0;
    Adafruit_MCP23017 mcpRead1;
    Adafruit_MCP23017 mcpWrite0;
    Adafruit_MCP23017 mcpWrite1;

    int mcpRead0Last;
    int mcpRead1Last;

  public:
    MatrixControl();
    void getMatrix(bool (&ledState)[N_MATRIX_ELEMENTS]);
    void setMatrix(bool (&ledState)[N_MATRIX_ELEMENTS]);
    void detectChangeCallback(void (*f)(String));
    void testOutputMatrix();
    void testInputMatrix();
};
