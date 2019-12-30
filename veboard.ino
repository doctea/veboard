#include <TVout.h>
#include <pollserial.h>
#include <fontALL.h>

#define W 128
#define H 96

TVout TV;
pollserial pserial;


int frame_count = 0;
int clear_after = 0;

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


void initInputProcessing() {
  // Analog Comparator setup
  ADCSRA &= ~_BV(ADEN); // disable ADC
  ADCSRB |= _BV(ACME); // enable ADC multiplexer
  ADMUX &= ~_BV(MUX0);  // select A2 for use as AIN1 (negative voltage of comparator)
  ADMUX |= _BV(MUX1);
  ADMUX &= ~_BV(MUX2);
  ACSR &= ~_BV(ACIE);  // disable analog comparator interrupts
  ACSR &= ~_BV(ACIC);  // disable analog comparator input capture
}

void setup() {
  TV.begin(PAL,W,H);
  TV.select_font(font6x8);
  initOverlay();
  initInputProcessing();
  TV.println("\VurFX// veBoard");
  TV.println("-- Version 0.1337 --");
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
#define ENABLE_GRAPHMESSAGE ((int)'M')
#define DISABLE_GRAPHMESSAGE ((int)'m')
#define ENABLE_CAPTURE ((int)'R')
#define DISABLE_CAPTURE ((int)'r')
#define ENABLE_INVERT ((int)'I')
#define DISABLE_INVERT ((int)'i')
#define ENABLE_VIEWPLANE ((int)'E')
#define DISABLE_VIEWPLANE ((int)'e')

#define ROTATE_X ((int)'X')
#define ROTATE_Y ((int)'Y')
#define ROTATE_Z ((int)'Z')

#define ENABLE_OVERLAY ((int)'O')
#define DISABLE_OVERLAY ((int)'o')

bool enable_clear = false;//true;
bool enable_cube = true;//false;
bool enable_debug = false;
bool enable_axes = false;
bool enable_graph = false;
bool enable_graphmessage = false;
bool enable_capture = false;
bool enable_invert = false;
bool enable_viewplane = true;

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
    else if (target==ROTATE_X)      rotate_x      = !rotate_x;
    else if (target==ROTATE_Y)      rotate_y      = !rotate_y;
    else if (target==ROTATE_Z)      rotate_z      = !rotate_z;
    else if (target==DISABLE_DEBUG) enable_debug  = false;
    else if (target==ENABLE_CLEAR)  enable_clear  = true;
    else if (target==DISABLE_CLEAR) enable_clear  = false;
    else if (target==ENABLE_AXES)   enable_axes   = true;
    else if (target==DISABLE_AXES)  enable_axes   = false;
    else if (target==ENABLE_GRAPH)  enable_graph  = true;
    else if (target==DISABLE_GRAPH) enable_graph  = false;
    else if (target==ENABLE_GRAPHMESSAGE) enable_graphmessage = true;
    else if (target==DISABLE_GRAPHMESSAGE) enable_graphmessage = false;
    else if (target==ENABLE_OVERLAY) startOverlay();
    else if (target==DISABLE_OVERLAY) stopOverlay();
    else if (target==ENABLE_CAPTURE) enable_capture = true;
    else if (target==DISABLE_CAPTURE) enable_capture = false;
    else if (target==ENABLE_INVERT) enable_invert = true;
    else if (target==DISABLE_INVERT) enable_invert = false;
    else if (target==ENABLE_VIEWPLANE) enable_viewplane = true;
    else if (target==DISABLE_VIEWPLANE) enable_viewplane = false;
    else if (target>=((int)'0') && target <= ((int)'9')) clear_after = target-((int)'0');
    //TV.clear_screen();
    //TV.print(0,0,clear_after*10);
  }
  
  if (enable_clear || (clear_after>0 && frame_count >= clear_after)) {
    TV.delay_frame(1);
    TV.clear_screen();
    frame_count = 0;
  }

  if (enable_capture) {
    TV.capture();
    if (enable_invert) {
      TV.fill(INVERT);
    }
    TV.resume();
  }

  if (enable_axes) {
    drawAxes();
  }

  if (enable_graph) {
    drawGraph();
  }

  if (enable_graphmessage) {
    printGraphMessage();
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
    if (enable_viewplane) viewplane(1);
    printcube();
  }

  if (enable_invert) {
    TV.fill(INVERT);
  }

  frame_count++;
}
