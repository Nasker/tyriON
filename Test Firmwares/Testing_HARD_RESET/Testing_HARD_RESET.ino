#define HARD_RESET_PIN 17


void setup() {
    // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(HARD_RESET_PIN, OUTPUT);  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("HIGH -> 0VCC FOR DEVICE");
  digitalWrite(HARD_RESET_PIN, HIGH);
  delay(5000);
  Serial.println("LOW -> 5VCC FOR DEVICE");
  digitalWrite(HARD_RESET_PIN, LOW);
  delay(5000);
}
