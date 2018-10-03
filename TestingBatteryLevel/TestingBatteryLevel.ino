#include <RTPBatteryControl.h>

#define VCC_LEVEL_PIN GPIO_NUM_15

RTPBatteryControl bat(VCC_LEVEL_PIN, 4096);

void setup() {
  Serial.begin(115200);
}

void loop() {
  bat.monitorBatteryCallbacks(actOnBatteryCallback);
}

void actOnBatteryCallback(String callbackString, float vccLevel){
  Serial.print(callbackString);
  Serial.print(" and level is ");
  Serial.print(vccLevel);
  Serial.println("V");
}

