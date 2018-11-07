#include <Wire.h>
#include <LIDARLite.h>
#include <RTPSmooth.h>

LIDARLite myLidarLite;
RTPSmooth smoothie;

void setup() {
  Serial.begin(115200); // Initialize serial connection to display distance readings
  Serial.println("STARTING SETUP");
  myLidarLite.begin(4, true); // Set configuration to default and I2C to 400 kHz
  //myLidarLite.configure(4); // Change this number to try out alternate configurations
  Serial.println("ENDING SETUP");
}

void loop() {
  // Take a measurement with receiver bias correction and print to serial terminal
  Serial.println(myLidarLite.distance()); //smoothie.smooth(
  // Take 99 measurements without receiver bias correction and print to serial terminal
  delay(1);
}
