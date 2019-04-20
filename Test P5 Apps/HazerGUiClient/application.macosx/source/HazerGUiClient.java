import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import oscP5.*; 
import netP5.*; 
import controlP5.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class HazerGUiClient extends PApplet {





ControlP5 cp5;
OscP5 oscP5;
NetAddress myRemoteLocation;
RadioButton r1, r2;

public void setup() {
  
  frameRate(25);
  oscP5 = new OscP5(this, 12345);
  myRemoteLocation = new NetAddress("192.168.1.83", 3383);

  cp5 = new ControlP5(this);
  r1 = cp5.addRadioButton("Fan_Speed")
    .setCaptionLabel("Fan Speed")
    .showLabels()
    .setPosition(width*.1f, height*.25f)
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
    .setPosition(width*.1f, height*.65f)
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

public void draw() {
  background(0);
}

public void sendOSCMessage(String address, int value) {
  OscMessage myMessage = new OscMessage(address);
  myMessage.add(value);
  oscP5.send(myMessage, myRemoteLocation);
  print("Sending Message: ");
  println(myMessage);
}

public void controlEvent(ControlEvent theEvent) {
  if (theEvent.isFrom(r1)) {
    print("got an event from "+theEvent.getName()+"\t");
    for (int i=0; i<theEvent.getGroup().getArrayValue().length; i++) {
      print(PApplet.parseInt(theEvent.getGroup().getArrayValue()[i]));
    }
    println("\t "+theEvent.getValue());
    if (theEvent.getValue() >= 0) sendOSCMessage("/haze/intensity", PApplet.parseInt(theEvent.getValue()));
  }
  if (theEvent.isFrom(r2)) {
    print("got an event from "+theEvent.getName()+"\t");
    for (int i=0; i<theEvent.getGroup().getArrayValue().length; i++) {
      print(PApplet.parseInt(theEvent.getGroup().getArrayValue()[i]));
    }
    println("\t "+theEvent.getValue());
    if (theEvent.getValue() >= 0) sendOSCMessage("/haze/speed", PApplet.parseInt(theEvent.getValue()));
  }
}

/* incoming osc message are forwarded to the oscEvent method. */
public void oscEvent(OscMessage theOscMessage) {
  /* print the address pattern and the typetag of the received OscMessage */
  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.typetag());
}
  public void settings() {  size(700, 200); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "HazerGUiClient" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
