#include <RTPRotary.h>
#include <ArduinoOSC.h>

#define ROT_RIGHT_PIN1 GPIO_NUM_25
#define ROT_LEFT_PIN1 GPIO_NUM_26
#define ROT_RIGHT_PIN2 GPIO_NUM_27
#define ROT_LEFT_PIN2 GPIO_NUM_9
#define ROT_RIGHT_PIN3 GPIO_NUM_10
#define ROT_LEFT_PIN3 GPIO_NUM_13

WiFiUDP udp;
ArduinoOSCWiFi osc;
const char* ssid = "NaisDelClot";
const char* pwd = "xmp13051985";
const IPAddress ip(192, 168, 1, 201);
const IPAddress gateway(192, 168, 1, 1);
const IPAddress subnet(255, 255, 255, 0);
const char* host = "192.168.1.131";
const int recv_port = 10000;
const int send_port = 12000;

RTPRotary rotaryClick1(1, ROT_RIGHT_PIN1, ROT_LEFT_PIN1);
RTPRotary rotaryClick2(2, ROT_RIGHT_PIN2, ROT_LEFT_PIN2);
RTPRotary rotaryClick3(3, ROT_RIGHT_PIN3, ROT_LEFT_PIN3);
//arguments => (ID, rightRotaryPin, leftRotaryPin)

void setup() {
  Serial.begin(115200);
  WiFi.disconnect(true);
  WiFi.begin(ssid, pwd);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  osc.begin(udp, recv_port);
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

  Serial.println("SENDING OSC MESSAGE!");
  OSCMessage msg;
  msg.beginMessage(host, send_port);
  switch (ID) {
    case 1:
      msg.setOSCAddress("/#1Rotary");
      break;
    case 2:
      msg.setOSCAddress("/#2rotary");
      break;
    case 3:
      msg.setOSCAddress("/#3rotary");
      break;
    default:
      msg.setOSCAddress("/?rotary");
      break;

  }
  msg.addArgString(callbackString);
  msg.addArgInt32(newPosition);
  osc.send(msg);
}
