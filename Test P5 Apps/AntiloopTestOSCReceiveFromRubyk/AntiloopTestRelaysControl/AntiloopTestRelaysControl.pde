import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;


void setup() {
  size(640, 480);
  oscP5 = new OscP5(this, 3311); 
  myRemoteLocation = new NetAddress("172.16.17.172", 3312);//192.168.1.31
}

void draw() {
}

void keyPressed() {
  if (key == '1') {
    OscMessage myMessage = new OscMessage("/open");
    myMessage.add(0);
    oscP5.send(myMessage, myRemoteLocation);
    println("Opening 1st relay!");
  }
  if (key == '2') {
    OscMessage myMessage = new OscMessage("/open");
    myMessage.add(1);
    oscP5.send(myMessage, myRemoteLocation);
    println("Opening 2nd relay!");
  }
  if (key == '3') {
    OscMessage myMessage = new OscMessage("/open");
    myMessage.add(2);
    oscP5.send(myMessage, myRemoteLocation);
    println("Opening 3rd relay!");
  }
  if (key == '4') {
    OscMessage myMessage = new OscMessage("/open");
    myMessage.add(3);
    oscP5.send(myMessage, myRemoteLocation);
    println("Opening 4th relay!");
  }
  if (key == '5') {
    OscMessage myMessage = new OscMessage("/open");
    myMessage.add(4);
    oscP5.send(myMessage, myRemoteLocation);
    println("Opening 5th relay!");
  }
  if (key == '6') {
    OscMessage myMessage = new OscMessage("/open");
    myMessage.add(5);
    oscP5.send(myMessage, myRemoteLocation);
    println("Opening 6th relay!");
  }
  if (key == '!') {
    OscMessage myMessage = new OscMessage("/close");
    myMessage.add(0);
    oscP5.send(myMessage, myRemoteLocation);
    println("Closing 1st relay!");
  }
  if (key == '"') {
    OscMessage myMessage = new OscMessage("/close");
    myMessage.add(1);
    oscP5.send(myMessage, myRemoteLocation);
    println("Closing 2nd relay!");
  }
  if (key == '·') {
    OscMessage myMessage = new OscMessage("/close");
    myMessage.add(2);
    oscP5.send(myMessage, myRemoteLocation);
    println("Closing 3rd relay!");
  }
  if (key == '$') {
    OscMessage myMessage = new OscMessage("/close");
    myMessage.add(3);
    oscP5.send(myMessage, myRemoteLocation);
    println("Closing 4th relay!");
  }
  if (key == '%') {
    OscMessage myMessage = new OscMessage("/close");
    myMessage.add(4);
    oscP5.send(myMessage, myRemoteLocation);
    println("Closing 5th relay!");
  }
  if (key == '&') {
    OscMessage myMessage = new OscMessage("/close");
    myMessage.add(5);
    oscP5.send(myMessage, myRemoteLocation);
    println("Closing 6th relay!");
  }
  if (key == 's') {
    OscMessage myMessage = new OscMessage("/switchLights");
    myMessage.add(0);
    oscP5.send(myMessage, myRemoteLocation);
    println("Switching Lights!");
  }
}


void oscEvent(OscMessage theOscMessage) {
  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.typetag());
}
