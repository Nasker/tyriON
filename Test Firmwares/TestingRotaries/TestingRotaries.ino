#include <RTPRotary.h>

#define ROT_RIGHT_PIN1 GPIO_NUM_25
#define ROT_LEFT_PIN1 GPIO_NUM_26

#define ROT_RIGHT_PIN2 GPIO_NUM_27
#define ROT_LEFT_PIN2 GPIO_NUM_9

#define ROT_RIGHT_PIN3 GPIO_NUM_10
#define ROT_LEFT_PIN3 GPIO_NUM_13

#define BUTTON_PIN 0

RTPRotary rotaryClick1(1, ROT_RIGHT_PIN1, ROT_LEFT_PIN1);
RTPRotary rotaryClick2(2, ROT_RIGHT_PIN2, ROT_LEFT_PIN2);
RTPRotary rotaryClick3(3, ROT_RIGHT_PIN3, ROT_LEFT_PIN3);
       //arguments => (ID, rightRotaryPin, leftRotaryPin)

void setup() {
  Serial.begin(115200);
}

void loop() {
  rotaryClick1.callbackOnRotation(actOnRotCallback);
  rotaryClick2.callbackOnRotation(actOnRotCallback);
  rotaryClick3.callbackOnRotation(actOnRotCallback);
}

void actOnRotCallback(int ID, String callbackString, int newPosition) {
  Serial.print("Rotary with ID #");
  Serial.print(ID);
  Serial.print(" ");
  Serial.print(callbackString);
  Serial.print("\t-Position: ");
  Serial.println(newPosition);
}
