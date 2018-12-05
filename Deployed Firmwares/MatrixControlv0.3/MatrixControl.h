//#include <Wire.h>
#include <i2c_t3.h>
#include "Adafruit_MCP23017.h"
#include "PinsAndConstants.h"

const byte mcpReadsToMatrix[N_PINS_CHIP][N_INPUT_CHIPS] = {
  {18, 21}, {22, 12}, {23, 30}, {2, 14}, {10, 26}, {27, 25}, {17, 29}, {6, 9},
  {11, 5}, {19, 1}, {7, 15}, {28, 0}, {24, 4}, {13, 8}, {3, 16}, {31, 20}
}; //Should work

const byte matrixToMCPWrites[N_MATRIX_ELEMENTS][2] = {
  {MCP_0, 4}, {MCP_0, 6}, {MCP_1, 10}, {MCP_0, 10}, {MCP_0, 7}, {MCP_1, 9}, {MCP_1, 15}, {MCP_1, 4},
  {MCP_0, 0}, {MCP_1, 12}, {MCP_1, 11}, {MCP_1, 2}, {MCP_0, 13}, {MCP_1, 7}, {MCP_0, 8}, {MCP_0, 3},
  {MCP_0, 2}, {MCP_1, 8}, {MCP_1, 0}, {MCP_1, 1}, {MCP_0, 15}, {MCP_0, 1}, {MCP_0, 5}, {MCP_1, 14},
  {MCP_1, 3}, {MCP_0, 14}, {MCP_0, 12}, {MCP_1, 5}, {MCP_1, 13}, {MCP_0, 11}, {MCP_0, 9}, {MCP_1, 6}
};



void getMatrix(bool (& matrixLedState) [N_MATRIX_ELEMENTS + 1]);
void printMatrixLedState(bool (& matrixLedState) [N_MATRIX_ELEMENTS + 1]);
void setMatrix(bool (& matrixLedState) [N_MATRIX_ELEMENTS + 1]);
void matrixInit(bool (& matrixLedState) [N_MATRIX_ELEMENTS + 1]);
void detectChangeCallback(void (*actOnChangeCustomFunction)(String)) ;
void testOutputMatrixLoop();
void testOutputMatrixBlink();
void randomBlinkMatrix(bool (& matrixLedState) [N_MATRIX_ELEMENTS + 1]);
void testInputMatrix() ;
