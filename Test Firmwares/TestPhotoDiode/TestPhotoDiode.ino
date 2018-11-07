#define PHOTODIODE_PIN_1ST 3
#define PHOTODIODE_PIN_2ND 4
#define PHOTODIODE_PIN_3RD 6
#define PHOTODIODE_PIN_4TH 8

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(analogRead(PHOTODIODE_PIN_1ST));
  Serial.print("\t");
  Serial.print(analogRead(PHOTODIODE_PIN_2ND));
  Serial.print("\t");
  Serial.print(analogRead(PHOTODIODE_PIN_3RD));
  Serial.print("\t");
  Serial.println(analogRead(PHOTODIODE_PIN_4TH));
  delay(20);
}
