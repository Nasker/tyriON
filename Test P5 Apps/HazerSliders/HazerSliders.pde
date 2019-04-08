import oscP5.*;
import netP5.*;
import controlP5.*;

ControlP5 cp5;
OscP5 oscP5;
NetAddress myRemoteLocation;
RadioButton r1, r2;

void setup() {
  size(800, 400);
  frameRate(25);
  oscP5 = new OscP5(this, 12345);
  myRemoteLocation = new NetAddress("192.168.1.2", 9000);

  cp5 = new ControlP5(this);
  r1 = cp5.addRadioButton("Fan_Speed")
    .setPosition(width*.2, height/2)
    .setSize(40, 20)
    .setColorForeground(color(120))
    .setColorActive(color(255))
    .setColorLabel(color(255))
    .setItemsPerRow(5)
    .setSpacingColumn(50)
    .addItem("0%", 0)
    .addItem("25%", 25)
    .addItem("50%", 50)
    .addItem("75%", 75)
    .addItem("100%", 100);

  for (Toggle t : r1.getItems()) {
    t.getCaptionLabel().setColorBackground(color(255, 80));
    t.getCaptionLabel().getStyle().moveMargin(-7, 0, 0, -3);
    t.getCaptionLabel().getStyle().movePadding(7, 0, 0, 3);
    t.getCaptionLabel().getStyle().backgroundWidth = 45;
    t.getCaptionLabel().getStyle().backgroundHeight = 13;
  }
}

void draw() {
  background(0);
}

void sendOSCMessage(int value) {
  OscMessage myMessage = new OscMessage("/haze/set");
  myMessage.add(value);
  myMessage.add(value);
  oscP5.send(myMessage, myRemoteLocation);
  print("Sending Message: ");
  println(myMessage);
}

void controlEvent(ControlEvent theEvent) {
  if (theEvent.isFrom(r1)) {
    print("got an event from "+theEvent.getName()+"\t");
    for (int i=0; i<theEvent.getGroup().getArrayValue().length; i++) {
      print(int(theEvent.getGroup().getArrayValue()[i]));
    }
    println("\t "+theEvent.getValue());
    sendOSCMessage(int(theEvent.getValue()));
  }
}

/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage theOscMessage) {
  /* print the address pattern and the typetag of the received OscMessage */
  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.typetag());
}
