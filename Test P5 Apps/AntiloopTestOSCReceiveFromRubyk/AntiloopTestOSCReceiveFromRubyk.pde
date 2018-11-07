import oscP5.*;
import peasy.PeasyCam;


PeasyCam cam;

//import netP5.*;

OscP5 oscP5;
//NetAddress myRemoteLocation;
int x, y, z;

void setup() {
  size(1280, 960, P3D);
  oscP5 = new OscP5(this, 12000); 
  cam = new PeasyCam(this, 400);
  // myRemoteLocation = new NetAddress("172.16.17.172", 12001);
  smooth();
  stroke(255);
  lights();
}

void draw() {
  background(0);
  //ellipse(x+width/2, y+height/2, 30, 30);
  translate(x+width/2, y+height/2, z);
  sphere(100);
}

/*void mouseDragged() {
 OscMessage myMessage = new OscMessage("/received");
 myMessage.add(mouseX);
 myMessage.add(mouseY);
 //oscP5.send(myMessage, myRemoteLocation);
 //println("Sending Message!");
 
 ellipse(X, Y, 10, 10);
 }*/
/*
void mouseReleased() {
 background(255);
 }*/

void oscEvent(OscMessage theOscMessage) {
  // print("### received an osc message.");
  // print(" addrpattern: "+theOscMessage.addrPattern());
  // println(" typetag: "+theOscMessage.typetag());

  if (theOscMessage.checkAddrPattern("/Rotary")) {
    if (theOscMessage.checkTypetag("isi")) {
      int rotary = theOscMessage.get(0).intValue();
      String words = theOscMessage.get(1).stringValue();
      int value = theOscMessage.get(2).intValue();
      println("-Rotary: "+rotary+" says :"+words+" and sends this value: "+value);
      switch (rotary) {
      case 1: 
        x = value*2;
        break;
      case 2: 
        y = value*2;
        break;
      case 3: 
        z = value;
        break;
      default:
      }
    }
  }
}
