import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;

int X = 0;
int Y = 0;
int pX = X;
int pY = Y;

void setup() {
  size(640, 480);
  oscP5 = new OscP5(this, 3311); 
  myRemoteLocation = new NetAddress("192.168.1.31", 3312);
  smooth();
  stroke(0);
}

void draw() {
}

void keyPressed() {
  if (key == 'f') {
    OscMessage myMessage = new OscMessage("/fuck");
    myMessage.add("FUCK!");
    oscP5.send(myMessage, myRemoteLocation);
    println("SENDING FUCKING MESSAGE!");
  }
}

void mouseDragged() {
  OscMessage myMessage = new OscMessage("/received");
  myMessage.add(mouseX);
  myMessage.add(mouseY);
  oscP5.send(myMessage, myRemoteLocation);
  //println("Sending Message!");
  strokeWeight(constrain(map(dist(X, Y, pX, pY), 10, 1, 1, 10), 1, 10));
  line(X, Y, pX, pY);
  pX = X;
  pY = Y;
  //ellipse(X, Y, 10, 10);
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
  
  if (theOscMessage.checkAddrPattern("/fuck")) {
    if (theOscMessage.checkTypetag("s")) {
      println(theOscMessage.get(0).stringValue());
    }
  }
}
