import oscP5.*;
import netP5.*;

OscP5 oscP5_printer;
NetAddress printerHost;

void setup() {
  size(640, 480);
  oscP5_printer = new OscP5(this, 3331); 
  printerHost = new NetAddress("192.168.1.33", 3332);
}

void draw() {
}

void keyPressed() {
  if (key == '1') {
    OscMessage myMessage = new OscMessage("/reset");
    myMessage.add(0);
    oscP5_printer.send(myMessage, printerHost);
    println("Sending Reset");
  }
}

void oscEvent(OscMessage theOscMessage) {
  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.typetag());
}
