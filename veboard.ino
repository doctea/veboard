//#define HardwareSerial_h

#include <TVout.h>
#include <pollserial.h>
#include <fontALL.h>
//#include <SoftwareSerial.h>

#define W 128
#define H 96

TVout TV;
pollserial pserial;
//SoftwareSerial pserial(0,1);

int frame_count = 0;
int clear_after = 10;

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
  setup_serial();
  TV.println("-- Version 0.1337 --");
}

void setup_serial() {
  TV.set_hbi_hook(pserial.begin(38400));
  //pserial.begin(38400);
}

int target = 10;
int position = 0;

#define ENABLE_CLEAR  ((int)'V')
#define DISABLE_CLEAR ((int)'v')
#define ENABLE_DEBUG  ((int)'D')
#define DISABLE_DEBUG ((int)'d')
#define ENABLE_CUBE   ((int)'C')
#define DISABLE_CUBE  ((int)'c')
#define ENABLE_AXES   ((int)'A')
#define DISABLE_AXES  ((int)'a')
#define ENABLE_GRAPH  ((int)'G')
#define DISABLE_GRAPH ((int)'g')
#define ENABLE_GRAPHMESSAGE   ((int)'M')
#define DISABLE_GRAPHMESSAGE  ((int)'m')
#define ENABLE_CAPTURE        ((int)'R')
#define DISABLE_CAPTURE       ((int)'r')
#define ENABLE_INVERT         ((int)'I')
#define DISABLE_INVERT        ((int)'i')
#define ENABLE_VIEWPLANE      ((int)'E')
#define DISABLE_VIEWPLANE     ((int)'e')

#define ENABLE_ROTATE_X ((int)'X')
#define ENABLE_ROTATE_Y ((int)'Y')
#define ENABLE_ROTATE_Z ((int)'Z')
#define DISABLE_ROTATE_X ((int)'x')
#define DISABLE_ROTATE_Y ((int)'y')
#define DISABLE_ROTATE_Z ((int)'z')

#define ENABLE_OVERLAY  ((int)'O')
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
bool enable_overlay = true;

bool rotate_x = false;
bool rotate_y = false;
bool rotate_z = false;

void report_status() {
  pserial.write(enable_cube ? ENABLE_CUBE : DISABLE_CUBE);
  pserial.write(enable_clear ? ENABLE_CLEAR : DISABLE_CLEAR);
  pserial.write(enable_axes ? ENABLE_AXES : DISABLE_AXES);
  pserial.write(enable_graph ? ENABLE_GRAPH : DISABLE_GRAPH);
  //pserial.write(enable_graphmessage ? ENABLE_GRAPHMESS
  pserial.write(enable_invert ? ENABLE_INVERT : DISABLE_INVERT);
  pserial.write(enable_viewplane ? ENABLE_VIEWPLANE : DISABLE_VIEWPLANE);
  pserial.write(enable_capture ? ENABLE_CAPTURE : DISABLE_CAPTURE);
  pserial.write(rotate_x ? ENABLE_ROTATE_X : DISABLE_ROTATE_X);
  pserial.write(rotate_y ? ENABLE_ROTATE_Y : DISABLE_ROTATE_Y);
  pserial.write(rotate_z ? ENABLE_ROTATE_Z : DISABLE_ROTATE_Z);
  pserial.write(clear_after + ((int)'0'));
}

void loop() {
  if (pserial.available()) {
    target = pserial.read();
    //TV.print((char)pserial.read());
    int p = target;
    if      (target==ENABLE_CUBE)   enable_cube   = true;
    else if (target==DISABLE_CUBE)  enable_cube   = false;
    else if (target==ENABLE_DEBUG)  enable_debug  = true;
    else if (target==DISABLE_DEBUG) enable_debug  = false;
    else if (target==ENABLE_ROTATE_X)      rotate_x      = true;
    else if (target==DISABLE_ROTATE_X)     rotate_x      = false;
    else if (target==ENABLE_ROTATE_Y)      rotate_y      = true;
    else if (target==DISABLE_ROTATE_Y)     rotate_y      = false;
    else if (target==ENABLE_ROTATE_Z)      rotate_z      = true;
    else if (target==DISABLE_ROTATE_Z)     rotate_z      = false;
    else if (target==ENABLE_CLEAR)  enable_clear  = true;
    else if (target==DISABLE_CLEAR) enable_clear  = false;
    else if (target==ENABLE_AXES)   enable_axes   = true;
    else if (target==DISABLE_AXES)  enable_axes   = false;
    else if (target==ENABLE_GRAPH)  enable_graph  = true;
    else if (target==DISABLE_GRAPH) enable_graph  = false;
    else if (target==ENABLE_GRAPHMESSAGE) enable_graphmessage = true;
    else if (target==DISABLE_GRAPHMESSAGE) enable_graphmessage = false;
    else if (target==ENABLE_OVERLAY)  { if (!enable_overlay) { enable_overlay = true;   startOverlay(); } }
    else if (target==DISABLE_OVERLAY) { if (enable_overlay)   { enable_overlay = false; stopOverlay(); } }
    else if (target==ENABLE_CAPTURE) enable_capture = true;
    else if (target==DISABLE_CAPTURE) enable_capture = false;
    else if (target==ENABLE_INVERT) enable_invert = true;
    else if (target==DISABLE_INVERT) enable_invert = false;
    else if (target==ENABLE_VIEWPLANE) enable_viewplane = true;
    else if (target==DISABLE_VIEWPLANE) enable_viewplane = false;
    else if (target>=((int)'0') && target<=((int)'9')) {
      clear_after = pow(2,(1+(target-((int)'0'))));
      //TV.println(clear_after);
    } else if (target==((int)'S')) report_status();
    else target = p;
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
    TV.print(target);
    TV.print("   :");  
    TV.print(clear_after);
    //delay(50);
  }
  //position++;
  //if (position>target) position = 0;

  if (enable_cube) {
    if (rotate_x) xrotate(PI/60);
    if (rotate_y) yrotate(PI/60);
    if (rotate_z) zrotate(PI/60);
    if (enable_viewplane) viewplane(1);
    printcube();
    // funky double-cube effect
    viewplane(50);
    printcube();
    viewplane(-50);
  }

  if (enable_invert/*&& !enable_capture*/) {
    TV.fill(INVERT);
  }

  frame_count++;
}
