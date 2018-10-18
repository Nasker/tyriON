#include <RTPBatteryControl.h>

#define VCC_LEVEL_PIN A0

RTPBatteryControl bat(VCC_LEVEL_PIN, 4096);

void setup() {
  Serial.begin(115200);
}

void loop() {
  //bat.monitorBatteryCallbacks(actOnBatteryCallback);
  float volts = bat.getVoltageLevelFast();
  float voltageHue = constrain(map(round( volts * 100), 350, 420, 0, 120), 0, 120) / 360.0f;

  Serial.print("-VCC LEVEL: ");
  Serial.print(volts);
  Serial.print("\t -HUE: ");
  Serial.println(voltageHue);

  delay(100);
}

void actOnBatteryCallback(String callbackString, float vccLevel) {
  Serial.print(callbackString);
  Serial.print(" and level is ");
  Serial.print(vccLevel);
  Serial.println("V");
}

