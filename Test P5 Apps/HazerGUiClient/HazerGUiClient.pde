import oscP5.*;
import netP5.*;
import controlP5.*;

ControlP5 cp5;
OscP5 oscP5;
NetAddress myRemoteLocation;
RadioButton r1, r2;

void setup() {
  size(700, 200);
  frameRate(25);
  oscP5 = new OscP5(this, 12345);
  myRemoteLocation = new NetAddress("192.168.1.2", 9000);

  cp5 = new ControlP5(this);
  r1 = cp5.addRadioButton("Fan_Speed")
    .setCaptionLabel("Fan Speed")
    .showLabels()
    .setPosition(width*.1, height*.25)
    .setSize(60, 20)
    .setColorForeground(color(120))
    .setColorActive(color(255))
    .setColorLabel(color(255))
    .setItemsPerRow(5)
    .setSpacingColumn(50)
    .addItem("0% haze", 0)
    .addItem("25% haze", 25)
    .addItem("50% haze", 50)
    .addItem("75% haze", 75)
    .addItem("100% haze", 100);



  for (Toggle t : r1.getItems()) {
    t.getCaptionLabel().setColorBackground(color(255, 80));
    t.getCaptionLabel().getStyle().moveMargin(-7, 0, 0, -3);
    t.getCaptionLabel().getStyle().movePadding(7, 0, 0, 3);
    t.getCaptionLabel().getStyle().backgroundWidth = 60;
    t.getCaptionLabel().getStyle().backgroundHeight = 13;
  }

  r2 = cp5.addRadioButton("Hazer_Intens")
    .setCaptionLabel("Hazer Intens") 
    .showLabels()
    .setPosition(width*.1, height*.65)
    .setSize(60, 20)
    .setColorForeground(color(120))
    .setColorActive(color(255))
    .setColorLabel(color(255))
    .setItemsPerRow(5)
    .setSpacingColumn(50)
    .addItem("0% speed", 0)
    .addItem("25% speed", 25)
    .addItem("50% speed", 50)
    .addItem("75% speed", 75)
    .addItem("100% speed", 100);

  for (Toggle t : r2.getItems()) {
    t.getCaptionLabel().setColorBackground(color(255, 80));
    t.getCaptionLabel().getStyle().moveMargin(-7, 0, 0, -3);
    t.getCaptionLabel().getStyle().movePadding(7, 0, 0, 3);
    t.getCaptionLabel().getStyle().backgroundWidth = 60;
    t.getCaptionLabel().getStyle().backgroundHeight = 13;
  }
}

void draw() {
  background(0);
}

void sendOSCMessage(String address, int value) {
  OscMessage myMessage = new OscMessage(address);
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
    if (theEvent.getValue() >= 0) sendOSCMessage("/haze/intensity", int(theEvent.getValue()));
  }
  if (theEvent.isFrom(r2)) {
    print("got an event from "+theEvent.getName()+"\t");
    for (int i=0; i<theEvent.getGroup().getArrayValue().length; i++) {
      print(int(theEvent.getGroup().getArrayValue()[i]));
    }
    println("\t "+theEvent.getValue());
    if (theEvent.getValue() >= 0) sendOSCMessage("/haze/speed", int(theEvent.getValue()));
  }
}

/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage theOscMessage) {
  /* print the address pattern and the typetag of the received OscMessage */
  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.typetag());
}
