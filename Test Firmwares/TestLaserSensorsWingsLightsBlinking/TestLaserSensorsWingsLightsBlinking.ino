/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  This example code is in the public domain.
*/

// Pin 13 has an LED connected on most Arduino boards.
// Pin 11 has the LED on Teensy 2.0
// Pin 6  has the LED on Teensy++ 2.0
// Pin 13 has the LED on Teensy 3.0
// give it a name:
int led = 13;
#define LIGHT_RELAY_PIN_1ST 2
#define LIGHT_RELAY_PIN_2ND 4

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(LIGHT_RELAY_PIN_1ST, OUTPUT);
  pinMode(LIGHT_RELAY_PIN_2ND, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(LIGHT_RELAY_PIN_1ST, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LIGHT_RELAY_PIN_2ND, HIGH);
  delay(1000);               // wait for a second
  digitalWrite(LIGHT_RELAY_PIN_1ST, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(LIGHT_RELAY_PIN_2ND, LOW);
  delay(1000);               // wait for a second
}
