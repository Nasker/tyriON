#include "MatrixControl.h"

MatrixControl mControl;

void setup() {
  Serial.begin(115200);     //velocitat de comunicació amb el port serie
}
void loop() {
  mControl.testOutputMatrix();
  bool state[32];
  mControl.getMatrix(state);
  mControl.setMatrix(state);
  mControl.detectChangeCallback(actOnMatrixCallback);
}

void actOnMatrixCallback(String callbackString){
  Serial.println(callbackString);
}
/*
  for (int i = 0; i < N_PINS_CHIP; i++) {
  mcpWrite0.digitalWrite(i, LOW);
  //mcpWrite1.digitalWrite(i, LOW);
  }
  delay(500);
  for (int i = 0; i < N_PINS_CHIP; i++) {
  Serial.print( mcpRead0.digitalRead(i) );
  Serial.print(" ");
  Serial.print( mcpRead1.digitalRead(i) );
  Serial.print(" ");
  }
  Serial.println();*/
/*
  mcpWrite0.writeGPIOAB(mcpRead0.readGPIOAB());
  mcpWrite1.writeGPIOAB(mcpRead1.readGPIOAB());
  actOnGPIOReadChanges();
  delay(500);
  }

  void actOnGPIOReadChanges() {
  if (mcpRead0.readGPIOAB() != mcpRead0Last || mcpRead1.readGPIOAB() != mcpRead1Last) {
   OSCMessage msg("/state");      //creem un missatge OSC amb l'etiqueta /response
   for (int i = 0; i < N_PINS_CHIP; i++) {
     msg.add(mcpRead0.digitalRead(i));
   }
   for (int i = 0; i < N_PINS_CHIP; i++) {
     msg.add(mcpRead1.digitalRead(i));
   }
   Udp.beginPacket(outIp, outPort);      //Comenvem un paquet de transmissio Udp
   msg.send(Udp);                  //l'enviem
   Udp.endPacket();              //tanquem el paquet
   msg.empty();
   mcpRead0Last = mcpRead0.readGPIOAB();
   mcpRead1Last = mcpRead1.readGPIOAB();
  }
  }*/
