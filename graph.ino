#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 


unsigned char x,y;
unsigned char originx = 5;
unsigned char originy = 80;
unsigned char plotx = originx;
unsigned char ploty = 40;
char s[32];
int index = 0;
//int messageLen = 32;
char message[100] = "...GOLDILOCKS...0x3040D932...ALPHA...0x4834...BETA...0b10101001...GAMMA...TBTT###########\0";
int messageLen = 32; //length(message);
//"...OVERLAY TEXT AND GRAPHICS ON A VIDEO SIGNAL...OVERLAY TEXT AND GRAPHICS ON A VIDEO SIGNAL";
char saveChar;
byte ledState = LOW;
/*
void setup()  {
  tv.begin(PAL, W, H);
  initOverlay();
  tv.select_font(font6x8);
  tv.fill(0);
  drawGraph();
  randomSeed(analogRead(0));
}*/

/*void resetMessage() {
  sprintf(message, "...GOLDILOCKS...0x%04X...ALPHA...0x%04X...BETA..."BYTE_TO_BINARY_PATTERN"...GAMMA...TBTT#\0",
    random(65536), random(4096), BYTE_TO_BINARY(random(255))
  );
}*/

void drawGraph() {
  /*if (capture_enabled) {
    capture();
  }*/
  //resetMessage();
  //saveChar = message[index+28];
  //message[index+28] = '\0';

  /*for(int x=6;x>=0;x--) {
    if (x<6) {
      tv.delay_frame(1);
    } 
    tv.print(x, 87, message+index);

    for(byte y=87;y<96;y++) {
      tv.draw_line(0, y, 5, y, 0);
      tv.draw_line(128, y, 134, y, 0);
    }
  }

  message[index+28] = saveChar;
  index++;
  if (index > 45) {
    index = 0;
  }*/

  //if (!capture_enabled) {
    //sprintf(s, "%ums", millis());
    //TV.print(0, 0, s);
  
    if (plotx++ > 120) {
      //if (other_enabled && !capture_enabled) tv.fill(0);
      //drawAxes();
      plotx = originx + 1;
      //return;
    }
    byte newploty = ploty + random(0, 7) - 3;
    newploty = constrain(newploty, 15, originy);
    TV.draw_line(plotx-1, ploty, plotx, newploty, 1);
    ploty = newploty;
  //}
}


void drawAxes() {
  TV.draw_line(originx, 15, originx, originy, 1);
  TV.draw_line(originx, originy, 120, originy, 1);
  //if (capture_enabled) 
    //return;
  for(byte y=originy;y>15;y -= 4) {
    TV.set_pixel(originx-1, y, 1);
    TV.set_pixel(originx-2, y, 1);
  }
  for(byte x=originx;x<120;x += 4) {
    TV.set_pixel(x, originy+1, 1);
    TV.set_pixel(x, originy+2, 1);
  }
}
