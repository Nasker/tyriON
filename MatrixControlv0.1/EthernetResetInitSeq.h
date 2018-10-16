#include "PinsAndConstants.h"

void EthernetResetInitSeq() {         //secuencia d'inici necesaria per al modul ethernet
  pinMode(ETHERNET_RESET_PIN, OUTPUT);
  digitalWrite(ETHERNET_RESET_PIN, LOW);    // begin reset the WIZ820io
  pinMode(ETHERNET_CS_PIN, OUTPUT);
  digitalWrite(ETHERNET_CS_PIN, HIGH);  // de-select WIZ820io
  digitalWrite(ETHERNET_RESET_PIN, HIGH);
}
