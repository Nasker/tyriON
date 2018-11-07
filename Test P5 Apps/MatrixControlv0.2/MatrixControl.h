#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include "PinsAndConstants.h"

class MatrixControl {

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

    Adafruit_MCP23017 mcpRead0;
    Adafruit_MCP23017 mcpRead1;
    Adafruit_MCP23017 mcpWrite0;
    Adafruit_MCP23017 mcpWrite1;

    int mcpRead0Last;
    int mcpRead1Last;

  public:
    MatrixControl();
    void initMatrix();
    void getMatrix(bool (&ledState)[N_MATRIX_ELEMENTS + 1]);
    void setMatrix(bool (&ledState)[N_MATRIX_ELEMENTS + 1]);
    void detectChangeCallback(void (*f)(String));
    void testInputMatrix();
    void printMatrixLedState(bool (& matrixLedState) [N_MATRIX_ELEMENTS + 1]);
    void testOutputMatrixLoop();
    void testOutputMatrixBlink();
};
