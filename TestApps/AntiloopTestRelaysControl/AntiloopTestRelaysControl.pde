import oscP5.*;
import netP5.*;

OscP5 oscP5_relays;
NetAddress relaysHost;
OscP5 oscP5_steps;
NetAddress stepsHost;
OscP5 oscP5_matrix;
NetAddress matrixHost;
OscP5 oscP5_lidar;
NetAddress lidarHost;
OscP5 oscP5_buttons;
NetAddress buttonsHost;
OscP5 oscP5_laser;
NetAddress laserHost;
OscP5 oscP5_rubyk;
NetAddress rubykHost;


void setup() {
  size(1280, 800);
  oscP5_relays = new OscP5(this, 3311); 
  relaysHost = new NetAddress("192.168.1.31", 3312);

  oscP5_steps = new OscP5(this, 3321); 
  stepsHost = new NetAddress("192.168.1.32", 3322);

  oscP5_matrix = new OscP5(this, 3341); 
  matrixHost = new NetAddress("192.168.1.34", 3342);

  oscP5_lidar = new OscP5(this, 3351); 
  lidarHost = new NetAddress("192.168.1.35", 3352);

  oscP5_buttons = new OscP5(this, 3361); 
  buttonsHost = new NetAddress("192.168.1.36", 3362);

  oscP5_laser = new OscP5(this, 3371); 
  laserHost = new NetAddress("192.168.1.37", 3372);

  oscP5_rubyk = new OscP5(this, 8888); 
  rubykHost = new NetAddress("192.168.1.38", 8889);
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
  if (key == '2') {
    OscMessage myMessage = new OscMessage("/open");
    myMessage.add(1);
    oscP5_relays.send(myMessage, relaysHost);
    println("Opening 2nd relay!");
  }
  if (key == '3') {
    OscMessage myMessage = new OscMessage("/open");
    myMessage.add(2);
    oscP5_relays.send(myMessage, relaysHost);
    println("Opening 3rd relay!");
  }
  if (key == '4') {
    OscMessage myMessage = new OscMessage("/open");
    myMessage.add(3);
    oscP5_relays.send(myMessage, relaysHost);
    println("Opening 4th relay!");
  }
  if (key == '5') {
    OscMessage myMessage = new OscMessage("/open");
    myMessage.add(4);
    oscP5_relays.send(myMessage, relaysHost);
    println("Opening 5th relay!");
  }
  if (key == '6') {
    OscMessage myMessage = new OscMessage("/open");
    myMessage.add(5);
    oscP5_relays.send(myMessage, relaysHost);
    println("Opening 6th relay!");
  }
  if (key == '!') {
    OscMessage myMessage = new OscMessage("/close");
    myMessage.add(0);
    oscP5_relays.send(myMessage, relaysHost);
    println("Closing 1st relay!");
  }
  if (key == '"') {
    OscMessage myMessage = new OscMessage("/close");
    myMessage.add(1);
    oscP5_relays.send(myMessage, relaysHost);
    println("Closing 2nd relay!");
  }
  if (key == '·') {
    OscMessage myMessage = new OscMessage("/close");
    myMessage.add(2);
    oscP5_relays.send(myMessage, relaysHost);
    println("Closing 3rd relay!");
  }
  if (key == '$') {
    OscMessage myMessage = new OscMessage("/close");
    myMessage.add(3);
    oscP5_relays.send(myMessage, relaysHost);
    println("Closing 4th relay!");
  }
  if (key == '%') {
    OscMessage myMessage = new OscMessage("/close");
    myMessage.add(4);
    oscP5_relays.send(myMessage, relaysHost);
    println("Closing 5th relay!");
  }
  if (key == '&') {
    OscMessage myMessage = new OscMessage("/close");
    myMessage.add(5);
    oscP5_relays.send(myMessage, relaysHost);
    println("Closing 6th relay!");
  }
  if (key == '8') {
    OscMessage myMessage = new OscMessage("/switchLights");
    myMessage.add(0);
    oscP5_relays.send(myMessage, relaysHost);
    println("Switching Lights!");
  }
  if (key == 'a') {
    OscMessage myMessage = new OscMessage("/open");
    myMessage.add(0);
    oscP5_laser.send(myMessage, laserHost);
    println("Opening Laser 1st relay");
  }
  if (key == 's') {
    OscMessage myMessage = new OscMessage("/open");
    myMessage.add(1);
    oscP5_laser.send(myMessage, laserHost);
    println("Opening Laser 2nd relay");
  }
  if (key == 'd') {
    OscMessage myMessage = new OscMessage("/open");
    myMessage.add(2);
    oscP5_laser.send(myMessage, laserHost);
    println("Opening Laser 3rd relay");
  }
  if (key == 'f') {
    OscMessage myMessage = new OscMessage("/open");
    myMessage.add(3);
    oscP5_laser.send(myMessage, laserHost);
    println("Opening Laser 4th relay");
  }
  if (key == 'g') {
    OscMessage myMessage = new OscMessage("/open");
    myMessage.add(4);
    oscP5_laser.send(myMessage, laserHost);
    println("Opening Laser 5th relay");
  }
  if (key == 'A') {
    OscMessage myMessage = new OscMessage("/close");
    myMessage.add(0);
    oscP5_laser.send(myMessage, laserHost);
    println("Closing Laser 1st relay");
  }
  if (key == 'S') {
    OscMessage myMessage = new OscMessage("/close");
    myMessage.add(1);
    oscP5_laser.send(myMessage, laserHost);
    println("Closing Laser 2nd relay");
  }
  if (key == 'D') {
    OscMessage myMessage = new OscMessage("/close");
    myMessage.add(2);
    oscP5_laser.send(myMessage, laserHost);
    println("Closing Laser 3rd relay");
  }
  if (key == 'F') {
    OscMessage myMessage = new OscMessage("/close");
    myMessage.add(3);
    oscP5_laser.send(myMessage, laserHost);
    println("Closing Laser 4th relay");
  }
  if (key == 'G') {
    OscMessage myMessage = new OscMessage("/close");
    myMessage.add(4);
    oscP5_laser.send(myMessage, laserHost);
    println("Closing Laser 5th relay");
  }
  if (key == 'z') {
    OscMessage myMessage = new OscMessage("/open");
    myMessage.add(0);
    oscP5_steps.send(myMessage, stepsHost);
    println("Opening Steps 1st relay");
  }
    if (key == 'x') {
    OscMessage myMessage = new OscMessage("/open");
    myMessage.add(1);
    oscP5_steps.send(myMessage, stepsHost);
    println("Opening Steps 2nd relay");
  }
    if (key == 'Z') {
    OscMessage myMessage = new OscMessage("/close");
    myMessage.add(0);
    oscP5_steps.send(myMessage, stepsHost);
    println("Closing Steps 1st relay");
  }
    if (key == 'X') {
    OscMessage myMessage = new OscMessage("/close");
    myMessage.add(1);
    oscP5_steps.send(myMessage, stepsHost);
    println("Closing Steps 2nd relay");
  }
}


void oscEvent(OscMessage theOscMessage) {
  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.typetag());
}
