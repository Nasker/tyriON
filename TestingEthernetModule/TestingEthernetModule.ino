#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBoards.h>

#define ETHERNET_RESET_PIN 9
#define ETHERNET_CS_PIN 10

EthernetUDP Udp;
IPAddress ip(192, 168, 1, 123);
IPAddress outIp(192, 168, 1, 131);
const unsigned int outPort = 12000;
const unsigned int inPort  = 12001;
byte mac[] = { 0x04, 0xE9, 0xE5, 0x03, 0x94, 0x5E }; 

void setup() {
  Serial.begin(115200);
  EthernetResetInitSeq();
  Ethernet.begin(mac, ip);
  Udp.begin(inPort);
}


void loop() {
  sendOSCMessage();
  OSCMsgReceive();
  delay(1000);
}

void sendOSCMessage() {
  OSCMessage msg("/FUCK");
  msg.add(analogRead(A0));
  msg.add("FUCK YOU!");
  Udp.beginPacket(outIp, outPort);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();
  Serial.println("MESSAGE SEND");
}

void OSCMsgReceive() {
  OSCBundle bundleIN;
  int size;
  if ( (size = Udp.parsePacket()) > 0) {
    Serial.print("Message Received with size: ");
    Serial.println(size);
    while (size--)
      bundleIN.fill(Udp.read());
      
    if (!bundleIN.hasError()) bundleIN.route("/input", inputfunction);
    else Serial.println("Message No GOOd!");
  }
}

void EthernetResetInitSeq() {
  pinMode(ETHERNET_RESET_PIN, OUTPUT);
  digitalWrite(ETHERNET_RESET_PIN, LOW);    // begin reset the WIZ820io
  pinMode(ETHERNET_CS_PIN, OUTPUT);
  digitalWrite(ETHERNET_CS_PIN, HIGH);  // de-select WIZ820io
  digitalWrite(ETHERNET_RESET_PIN, HIGH);
}

void inputfunction(OSCMessage &msg, int addrOffset){
  int value = msg.getInt(0);
  Serial.print("Value = : ");
  Serial.println(value);
}
