import oscP5.*;
import netP5.*;

OscP5 oscP5_relays;
NetAddress relaysHost;

void setup() {
  size(640, 480);
  oscP5_relays = new OscP5(this, 3311); 
  relaysHost = new NetAddress("172.16.17.144", 3312);
}

void draw() {
}

void keyPressed() {
  if (key == '1') {
    OscMessage myMessage = new OscMessage("/open");
    myMessage.add(0);
    oscP5_relays.send(myMessage, relaysHost);
    println("Opening 1st relay!");
  }
  if (key == '!') {
    OscMessage myMessage = new OscMessage("/close");
    myMessage.add(0);
    oscP5_relays.send(myMessage, relaysHost);
    println("Closing 1st relay!");
  }
}


void oscEvent(OscMessage theOscMessage) {
  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.typetag());
}
