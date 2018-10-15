#include <RTPButton.h>

#define INFRARED_PIN 2

RTPButton hall(INFRARED_PIN, NORMAL);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(INFRARED_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(digitalRead(INFRARED_PIN));
  //delay(20);
  hall.callback(actOnCallback);
}

void actOnCallback(String callbackString){
  Serial.println(callbackString);
}

