#define N_RELAYS 5
#define RELAY_PIN_1ST 20
#define RELAY_PIN_2ND 22
#define RELAY_PIN_3RD 4
#define RELAY_PIN_4TH 6
#define SWITCHING_LIGHTS_PIN 8

#include <RTPRelay.h>

RTPRelay relaysArray[N_RELAYS] = {
  RTPRelay(RELAY_PIN_1ST), RTPRelay(RELAY_PIN_2ND),
  RTPRelay(RELAY_PIN_3RD), RTPRelay(RELAY_PIN_4TH),
  RTPRelay(SWITCHING_LIGHTS_PIN),
};

//RTPRelay switchingLights(SWITCHING_LIGHTS_PIN);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  //cycleRelays();
  allAtOnce();
}

void switchLights(){
//  switchingLights.switchState();
}

void cycleRelays() {
  for (int i = 0; i < N_RELAYS; i++) {
    Serial.print(i);
    Serial.println("# ON");
    relaysArray[i].setState(!true);
    delay(100);
    Serial.print(i);
    Serial.println("# OFF");
    relaysArray[i].setState(!false);
    //delay(100);
  }
}


void allAtOnce() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < N_RELAYS; i++) {
    Serial.println("ALL ON");
    relaysArray[i].setState(!true);
  }
  delay(1000);
  for (int i = 0; i < N_RELAYS; i++) {
    Serial.println("ALL OFF");
    relaysArray[i].setState(!false);
  }
  delay(1000);
}

