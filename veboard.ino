#include <TVout.h>
#include <pollserial.h>
#include <fontALL.h>

#define W 128
#define H 96

TVout TV;
pollserial pserial;

bool capture_enabled = false;

void stopOverlay() {
  TV.end();
  EIMSK = 0;
  EICRA = 0;
  TV.begin(PAL, W, H);
}

void startOverlay() {
  TV.end();
  TV.begin(PAL, W, H);
  initOverlay();
}

void initOverlay() {
  TCCR1A = 0;
  // Enable timer1.  ICES0 is set to 0 for falling edge detection on input capture pin.
  TCCR1B = _BV(CS10);

  // Enable input capture interrupt
  TIMSK1 |= _BV(ICIE1);

  // Enable external interrupt INT0 on pin 2 with falling edge.
  EIMSK = _BV(INT0);
  EICRA = _BV(ISC01);
}

// Required to reset the scan line when the vertical sync occurs
ISR(INT0_vect) {
  display.scanLine = 0;
}

void setup() {
  TV.begin(PAL,W,H);
  TV.select_font(font6x8);
  initOverlay();
  TV.println("Serial Terminal");
  TV.println("-- Version 0.1 --");
  TV.set_hbi_hook(pserial.begin(9600));
}

int target = 10;
int position = 0;

#define ENABLE_CLEAR ((int)'V')
#define DISABLE_CLEAR ((int)'v')
#define ENABLE_DEBUG ((int)'D')
#define DISABLE_DEBUG ((int)'d')
#define ENABLE_CUBE  ((int)'C')
#define DISABLE_CUBE ((int)'c')
#define ENABLE_AXES  ((int)'A')
#define DISABLE_AXES ((int)'a')
#define ENABLE_GRAPH  ((int)'G')
#define DISABLE_GRAPH ((int)'g')

#define ROTATE_X ((int)'X')
#define ROTATE_Y ((int)'Y')
#define ROTATE_Z ((int)'Z')

bool enable_clear = false;//true;
bool enable_cube = true;//false;
bool enable_debug = true;
bool enable_axes = false;
bool enable_graph = false;

bool rotate_x = false;
bool rotate_y = false;
bool rotate_z = false;

void loop() {
  if (pserial.available()) {
    target = pserial.read();
    //TV.print((char)pserial.read());
    if (target==ENABLE_CUBE)        enable_cube   = true;
    else if (target==DISABLE_CUBE)  enable_cube   = false;
    else if (target==ENABLE_DEBUG)  enable_debug  = true;
    else if (target==DISABLE_DEBUG) enable_debug  = false;
    else if (target==ENABLE_CLEAR)  enable_clear  = true;
    else if (target==DISABLE_CLEAR) enable_clear  = false;
    else if (target==ENABLE_AXES)   enable_axes   = true;
    else if (target==DISABLE_AXES)  enable_axes   = false;
    else if (target==ENABLE_GRAPH)  enable_graph  = true;
    else if (target==DISABLE_GRAPH) enable_graph  = false;
    else if (target==ROTATE_X)      rotate_x      = !rotate_x;
    else if (target==ROTATE_Y)      rotate_y      = !rotate_y;
    else if (target==ROTATE_Z)      rotate_z      = !rotate_z;
  }
  
  if (enable_clear) {
    TV.delay_frame(1);
    TV.clear_screen();
  }

  if (enable_axes) {
    drawAxes();
  }

  if (enable_graph) {
    drawGraph();
  }
  
  if (enable_debug) {
    TV.print(0,90,"pos:");
    TV.print(position);
    TV.print("   :");  
    //delay(50);
  }
  position++;
  if (position>target) position = 0;

  if (enable_cube) {
    if (rotate_x) xrotate(PI/60);
    if (rotate_y) yrotate(PI/60);
    if (rotate_z) zrotate(PI/60);
    printcube();
  }
}
