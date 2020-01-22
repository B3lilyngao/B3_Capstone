import controlP5.*;
import processing.serial.*;

Serial port;

ControlP5 cp5;

PImage bg;

void setup(){
  
  //Setting Up the GUI Background
  size(1000, 800);
  bg = loadImage("GUIBackground1.png");
  
  //Connecting to the Arduino Serial Port
  port = new Serial(this, "COM4", 9600);
  
  cp5 = new ControlP5(this);
  
  // Forward Button
  cp5.addButton("Forward")
    .setPosition(600, 200)
    .setSize(100, 100)
    ;
  
  // Backward Button
  cp5.addButton("Backward")
    .setPosition(600, 400)
    .setSize(100, 100)
    ;
  
  // Left Button
  cp5.addButton("Left")
    .setPosition(500, 300)
    .setSize(100, 100)
    ;
  
  // Right Button
  cp5.addButton("Right")
    .setPosition(700, 300)
    .setSize(100, 100)
    ;
  
  // Emergency Button
  cp5.addButton("Emergency")
    .setPosition(800, 600)
    .setSize(100, 100)
    ;
    
}

// Drawing Background, Circle
void draw(){
  
  //background(150, 150, 150);
  background(bg);
  
  // Drawing Sensor Circles -- hopefully color will change with distance, or something like that
  fill(226, 131, 131);
  ellipse(50,100,20, 20);
  
  fill(226, 214, 131);
  ellipse(200,100,20, 20);
  
  fill(146, 226, 131);
  ellipse(350,100,20, 20);

}

// Button functions -- each of these functions writes to the serial port, arduino code can handle the function
// (i.e. sending the signal to the antenna to promptmoving)
void Forward(){
  
  port.write('f');
  
}

void Backward(){
  
  port.write('b');
  
}

void Left(){
  
  port.write('l');
  
}

void Right(){
  
  port.write('r');
  
}

void Emergency(){
  
  port.write('e');
  
}
