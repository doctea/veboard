/*#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 
*/

unsigned char x,y;
unsigned char originx = 5;
unsigned char originy = 80;
unsigned char plotx = originx;
unsigned char ploty = 40;
char s[32];
int index = 0;
char message[] = "...GOLDILOCKS...0x3040D932...ALPHA...0x4834...BETA...0b10101001...GAMMA...TBTT###########";
int messageLen = 32; //length(message);
//"...OVERLAY TEXT AND GRAPHICS ON A VIDEO SIGNAL...OVERLAY TEXT AND GRAPHICS ON A VIDEO SIGNAL";
char saveChar;
byte ledState = LOW;

void printGraphMessage() {
  // for some reason, uncommenting this function prevents the sketch from running, even if its never called!
/*  saveChar = message[index+22];
  message[index+22] = '\0';

  for(int x=6;x>=0;x--) {
    //if (x<6) {
    //  tv.delay_frame(1);
    //} 
    TV.print(x, 87, message+index);

    for(byte y=87;y<96;y++) {
      TV.draw_line(0, y, 5, y, 0);
      TV.draw_line(128, y, 134, y, 0);
    }
  }

  message[index+22] = saveChar;
  index++;
  if (index > 45) {
    index = 0;
  }

  sprintf(s, "%ums", millis());
  TV.print(0, 0, s);*/
}

void drawGraph() { 
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
}


void drawAxes() {
  TV.draw_line(originx, 15, originx, originy, 1);
  TV.draw_line(originx, originy, 120, originy, 1);
  for(byte y=originy;y>15;y -= 4) {
    TV.set_pixel(originx-1, y, 1);
    TV.set_pixel(originx-2, y, 1);
  }
  for(byte x=originx;x<120;x += 4) {
    TV.set_pixel(x, originy+1, 1);
    TV.set_pixel(x, originy+2, 1);
  }
}
