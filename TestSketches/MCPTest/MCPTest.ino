
#include <Wire.h>
#include "Adafruit_MCP23017.h"

// Basic pin reading and pullup test for the MCP23017 I/O expander
// public domain!

// Connect pin #12 of the expander to Analog 5 (i2c clock)
// Connect pin #13 of the expander to Analog 4 (i2c data)
// Connect pins #15, 16 and 17 of the expander to ground (address selection)
// Connect pin #9 of the expander to 5V (power)
// Connect pin #10 of the expander to ground (common ground)
// Connect pin #18 through a ~10kohm resistor to 5V (reset pin, active low)

// Input #0 is on pin 21 so connect a button or switch from there to ground

Adafruit_MCP23017 mcpWrite0;
Adafruit_MCP23017 mcpWrite1;
Adafruit_MCP23017 mcpRead0;
Adafruit_MCP23017 mcpRead1;
#define mcpWrite0Adress 2//Defining adress of the chip
#define mcpWrite1Adress 3
#define mcpRead0Adress 0
#define mcpRead1Adress 1

void setup() {  
  Serial.begin(9600);
  mcpWrite0.begin(mcpWrite0Adress);      
  mcpWrite1.begin(mcpWrite1Adress);
  mcpRead0.begin(mcpRead0Adress);
  mcpRead1.begin(mcpRead1Adress);   
  mcpWrite0.pinMode(0,OUTPUT);  
  mcpWrite0.pinMode(1,OUTPUT);  
  mcpWrite0.pinMode(2,OUTPUT);  
  mcpWrite0.pinMode(3,OUTPUT);  
  mcpWrite0.pinMode(4,OUTPUT);  
  mcpWrite0.pinMode(5,OUTPUT);  
  mcpWrite0.pinMode(6,OUTPUT);  
  mcpWrite0.pinMode(7,OUTPUT);  
  mcpWrite0.pinMode(8,OUTPUT);  
  mcpWrite0.pinMode(9,OUTPUT);  
  mcpWrite0.pinMode(10,OUTPUT);  
  mcpWrite0.pinMode(11,OUTPUT);  
  mcpWrite0.pinMode(12,OUTPUT);  
  mcpWrite0.pinMode(13,OUTPUT);  
  mcpWrite0.pinMode(14,OUTPUT);  
  mcpWrite0.pinMode(15,OUTPUT);  
  mcpWrite1.pinMode(0,OUTPUT);  
  mcpWrite1.pinMode(1,OUTPUT);  
  mcpWrite1.pinMode(2,OUTPUT);  
  mcpWrite1.pinMode(3,OUTPUT);  
  mcpWrite1.pinMode(4,OUTPUT);  
  mcpWrite1.pinMode(5,OUTPUT);  
  mcpWrite1.pinMode(6,OUTPUT);  
  mcpWrite1.pinMode(7,OUTPUT);  
  mcpWrite1.pinMode(8,OUTPUT);  
  mcpWrite1.pinMode(9,OUTPUT);  
  mcpWrite1.pinMode(10,OUTPUT);  
  mcpWrite1.pinMode(11,OUTPUT);  
  mcpWrite1.pinMode(12,OUTPUT);  
  mcpWrite1.pinMode(13,OUTPUT);  
  mcpWrite1.pinMode(14,OUTPUT);  
  mcpWrite1.pinMode(15,OUTPUT);  
  pinMode(13, OUTPUT);  // use the p13 LED as debugging

}



void loop() {
  int inValue;
  //uint16_t outValue;
  int ioutValue;
  digitalWrite(13, digitalRead(2));//No fa res
  Serial.println("Adios...");
  inValue = 0;
  inValue += (mcpRead0.readGPIO(0)<<8);
  inValue += mcpRead0.readGPIO(1);
  inValue += (mcpRead1.readGPIO(0)<<24);
  inValue += (mcpRead1.readGPIO(1)<<16);
  Serial.println(inValue);
  delay(500);
  //outValue = inValue;
  ioutValue = (inValue<<8)&0xFF00FF00;
  ioutValue += (inValue>>8)&0xFF00FF;
  Serial.println(ioutValue);
  delay(500);
  mcpWrite0.writeGPIOAB(~ioutValue);
  mcpWrite1.writeGPIOAB(~(ioutValue>>16));
  delay(10);
}
