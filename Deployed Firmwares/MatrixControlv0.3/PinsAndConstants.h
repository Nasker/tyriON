#define ETHERNET_RESET_PIN 9    //Pins de reset i chip select (els altres per defecte)
#define ETHERNET_CS_PIN 10

#define N_MATRIX_ELEMENTS 32
#define N_PINS_CHIP 16
#define N_INPUT_CHIPS 2
#define N_OUTPUT_CHIPS 2
#define MCP_0 0
#define MCP_1 1

#define mcpRead0Adress 0
#define mcpRead1Adress 1
#define mcpWrite0Adress 2//Defining address of the chip
#define mcpWrite1Adress 3

#define TIME_TO_RESET 300000
#define I2C_FREQ 50000

#define HARD_RESET_PIN 17
