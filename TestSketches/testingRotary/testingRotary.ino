#include <RTPRotaryClick.h>
#include <ArduinoOSC.h>

#define ROT_RIGHT_PIN 0
#define ROT_LEFT_PIN 1
#define BUTTON_PIN 2





boolean ledState = true;

RTPRotaryClick rotaryClick(ROT_RIGHT_PIN, ROT_LEFT_PIN, BUTTON_PIN, LOW, false);


//arguments=> (rightRotaryPin,leftRotaryPin,pushButtonPin,activeLOWorHIGH,INTERNAL_PULLUP?)

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(ROT_RIGHT_PIN, HIGH);
  digitalWrite(ROT_LEFT_PIN, HIGH);
  Serial.begin(115200);
  WiFi.disconnect(true);
  //WiFi.onEvent(onWiFiEvent);
  WiFi.begin(ssid, pwd);
  WiFi.config(ip, gateway, subnet);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  osc.begin(udp, recv_port);
}

void loop() {
  // put your main code here, to run repeatedly:
  rotaryClick.callbackOnRotation(actOnRotCallback);
}

void actOnRotCallback(String callbackString, int newPosition) {
  digitalWrite(LED_BUILTIN, ledState);
  Serial.println("CALLBACK ROTARY");
  ledState = !ledState;

  OSCMessage msg;
  //set destination ip address & port no
  msg.beginMessage(host, send_port);
  //set argument
  msg.setOSCAddress("/rotary");
  msg.addArgString("rotating!");
  osc.send(msg);
  Serial.print("Sending message to: ");
  Serial.println(host);
}

