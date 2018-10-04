import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;

int X = 0;
int Y = 0;

void setup() {
  size(640, 480);
  oscP5 = new OscP5(this, 12000); 
  myRemoteLocation = new NetAddress("172.16.17.172", 12001);
  smooth();
  stroke(0);
}

void draw() {
}

void mouseDragged() {
  OscMessage myMessage = new OscMessage("/received");
  myMessage.add(mouseX);
  myMessage.add(mouseY);
  oscP5.send(myMessage, myRemoteLocation);
  //println("Sending Message!");

  ellipse(X, Y, 10, 10);
}

void mouseReleased() {
  background(255);
}

void oscEvent(OscMessage theOscMessage) {
  //print("### received an osc message.");
  //print(" addrpattern: "+theOscMessage.addrPattern());
  //println(" typetag: "+theOscMessage.typetag());

  if (theOscMessage.checkAddrPattern("/response")) {
    if (theOscMessage.checkTypetag("ii")) {
      X = theOscMessage.get(0).intValue();
      Y = theOscMessage.get(1).intValue();
      println("-X: "+X+"\t-Y: "+Y);
    }
  }
}
