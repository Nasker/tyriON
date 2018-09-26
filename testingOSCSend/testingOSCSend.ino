#include <ArduinoOSC.h>

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

int i = 0;
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
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
  //osc.parse();
  OSCMessage msg;
  //set destination ip address & port no
  msg.beginMessage(host, send_port);
  //set argument
  msg.setOSCAddress("/hola");
  msg.addArgInt32(i);
  msg.addArgFloat(i * (18 / 17));
  msg.addArgString("fuck you");
  //send osc message
  osc.send(msg);
  Serial.print("Sending message to: ");
  Serial.println(host);
  i++;

  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
