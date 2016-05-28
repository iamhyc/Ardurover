import processing.serial.*;
Serial serial;

String hmc_str;
float hmc_val;
final int width = 1200;
final int height = 360;

int index = 0;

void setup() {
  
  serial = new Serial(this, Serial.list()[0], 115200);
  serial.bufferUntil('\n');
  
  size(1200, 360);
  background(255);//white
  drawAxis();
  stroke(255, 0, 0);
  //fullScreen();
}

void draw() {
  if(hmc_str != null) {
    line((index-1), hmc_val, index, float(hmc_str));
    hmc_val = float(hmc_str);
  }
  if (index > 1200) {
    background(255);
    drawAxis();
    index = 0;
  }
}

void drawAxis() {
  for (int i = 0; i < width; i++) {
    stroke(200); // Grey
    line(i*10, 0, i*10, height);
    line(0, i*10, width, i*10);
  }

  stroke(0); // Black
  for (int i = 1; i <= 3; i++)
    line(0, height/4*i, width, height/4*i);
}

void serialEvent(Serial serial) {
  hmc_str = serial.readString();
  index++;
}