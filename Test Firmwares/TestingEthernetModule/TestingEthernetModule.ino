#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBoards.h>        //includes necessaris per el modul d'ethernet i OSC

#define ETHERNET_RESET_PIN 9    //Pins de reset i chip select (els altres per defecte)
#define ETHERNET_CS_PIN 10

EthernetUDP Udp;                //objecte per a connexió udp
IPAddress ip(192, 168, 1, 31);  //172, 16, 17, 172 //ip de la teensy i port on escoltem
const unsigned int inPort  = 3312;
IPAddress outIp(192, 168, 1, 143); //ip destí i port on enviarem
const unsigned int outPort = 3311;
byte mac[] = { 0x04, 0xE9, 0xE5, 0x03, 0x94, 0x5E }; //mac, patillera

int coordinateX = 0;    //"jugarem a rebotar les coordenades que ens envia la aplicació al destí
int coordinateY = 0;

void setup() {
  Serial.begin(115200);     //veloctitat de comunicació amb el port serie
  delay(1000);
  EthernetResetInitSeq();   //funció (definidad més abaix) de secuencia necesaria per inicialitzar modul ethernet!!
  Ethernet.begin(mac, ip);  //arranquem el modul d'ethernet
  Udp.begin(inPort);        //arranquem el port on escoltarem en Udp
  Serial.println("FINISH INIT!");
}

void loop() {
  OSCMsgReceive();    //al loop només esperem a rebre missatges OSC
  //Serial.println(millis());
}

void OSCMsgReceive() {
  OSCBundle bundleIN;       //paquet generic de missatges OSC, pot contenir whatever
  int size = Udp.parsePacket(); //mirem si hi ha dades als paquets, si rebem algo
  if (size > 0) {   //si rebem algo...
    //Serial.print("Message Received with size: ");
    //Serial.println(size);
    while (size--) {
      bundleIN.fill(Udp.read()); //plenem el bundle amb el que llegim al port Udp
    }
    bundleIN.route("/received", receivedMessageFunction);  //"Rutejem el rebut a l'etiqueta i la funció que cridem mes abaix.
    bundleIN.route("/fuck",fuckFunction);
  }
}

void fuckFunction(OSCMessage &msg, int addrOffset) {
  OSCMessage answ("/fuck");      //creem un missatge OSC amb l'etiqueta /response
  answ.add("FUCK YOU");
  Udp.beginPacket(outIp, outPort);      //Comenvem un paquet de transmissio Udp
  answ.send(Udp);                  //l'enviem
  Udp.endPacket();              //tanquem el paquet
  answ.empty();  
}

void receivedMessageFunction(OSCMessage &msg, int addrOffset) {
  coordinateX = msg.getInt(0);    //desempaquetem els Integers que ens venen
  coordinateY = msg.getInt(1);
  Serial.print("-X: ");             //els imprimim pel port serie
  Serial.print(coordinateX);
  Serial.print("\t-Y: ");
  Serial.println(coordinateY);
  sendOSCMessage();         //els enviarem (son variables globals, per aixo no hi ha arguments)
}

void sendOSCMessage() {
  OSCMessage msg("/response");      //creem un missatge OSC amb l'etiqueta /response
  msg.add(coordinateX);             //Hi afegim les coordinades
  msg.add(coordinateY);
  Udp.beginPacket(outIp, outPort);      //Comenvem un paquet de transmissio Udp
  msg.send(Udp);                  //l'enviem
  Udp.endPacket();              //tanquem el paquet
  msg.empty();                   //buidem el missatge
  //Serial.println("MESSAGE SEND");
}

void EthernetResetInitSeq() {         //secuencia d'inici necesaria per al modul ethernet
  pinMode(ETHERNET_RESET_PIN, OUTPUT);
  digitalWrite(ETHERNET_RESET_PIN, LOW);    // begin reset the WIZ820io
  pinMode(ETHERNET_CS_PIN, OUTPUT);
  digitalWrite(ETHERNET_CS_PIN, HIGH);  // de-select WIZ820io
  digitalWrite(ETHERNET_RESET_PIN, HIGH);
}
