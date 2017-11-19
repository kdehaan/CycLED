#include <Adafruit_NeoPixel.h>
#include "TimerOne.h"
#include "PinChangeInt.h"
#ifdef __AVR__
  #include <avr/power.h>
#endif


#define PIN 9
#define HALL 3
#define ledPin 13
#define numLeds 60
#define button 12

volatile unsigned long now = 0;
volatile unsigned long lastTime = 0;
volatile unsigned long currentTime = 0;
volatile unsigned long goal = 0;
float slope = 3.89;//3.89;
bool on;

float dt = 0.5;
float xk_1 = 0, vk_1 = 0, a = 0.5, b = 0.1;
float xk, vk, rk;

int error_tracking = 0;
int loop_tracking = 0;



// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLeds, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(button, INPUT);
  PCintPort::attachInterrupt(button, changeState, FALLING);
  PCintPort::attachInterrupt(HALL, magnet_detect, FALLING);
  digitalWrite(button, HIGH);
  on = false;
  
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
  colorWipe(strip.Color(0, 0, 0), 0);
  Serial.begin(9600);
  Serial.println("serial port initialized");
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  Timer1.initialize(1000000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.stop();
  Timer1.restart();
  //Timer1.attachInterrupt(timeout);
}

void changeState() {
  on = !on;
  if (on) {
    goal = 0;
  }
}

void loop() {
  if (on) {
    cycle();
    //digitalWrite(ledPin, HIGH);
  } else {
    //digitalWrite(ledPin, LOW);
    colorWipe(strip.Color(0, 0, 0), 0);
  }
  //cycle();

    


}

void timeout() {
  goal = 0;
}

void cycle() {
  loop_tracking++;
  if (loop_tracking > 5) {
    goal = 0;
    loop_tracking = 0;
  }
  Serial.print("STARTING LOOP with goal ");
  Serial.println(goal/1000);
  int lambda;

  if(goal > 802000) {
    lambda = ((goal/1000) - 802)*slope;
    lambda = (numLeds/60)*lambda;
    rainbowCycle(goal, lambda, 0);
  } else if (goal > 401000) {
    lambda = ((goal/1000) - 401)*slope*2;
    lambda = (numLeds/60)*lambda;
    rainbowCycle(goal, lambda, 1);
  } else if (goal > 200000) {
    lambda = ((goal/1000) - 200)*slope*4;
    lambda = (numLeds/60)*lambda;
    rainbowCycle(goal, lambda, 3);
  } else if (goal > 100000) {
    lambda = ((goal/1000) - 100)*slope*8;
    lambda = (numLeds/60)*lambda;
    rainbowCycle(goal, lambda, 7);
  } else if (goal > 50000) {
    lambda = ((goal/1000) - 50)*slope*16;
    lambda = (numLeds/60)*lambda;
    rainbowCycle(goal, lambda, 15);
  } else if (goal > 25000) { //slow down speed racer
    lambda = ((goal/1000) - 25)*slope*32;
    lambda = (numLeds/60)*lambda;
    rainbowCycle(goal, lambda, 31);
  } else {
    colorWipe(strip.Color(255, 255, 255), 0);
  }
}

//unsigned int catchSkips(unsigned int in) {
//  
//}

unsigned long alphaBeta(unsigned long in) {
//  Serial.print("in: ");
//  Serial.println(in);
//  Serial.print("xk: ");
//  Serial.println(xk);

  if ((in > xk_1*1.7) & (xk_1 != 0)) {
    error_tracking++;
    Serial.println("erroneous input");
    if (error_tracking >=3) {
      Serial.println("error reset");
      error_tracking = 0;
      xk_1 = in;
      return in;
    } else {
      return xk_1;
    }
  }

  xk = xk_1 + (vk_1 * dt);
  vk = vk_1;

  rk = in - xk;

  xk += a * rk;
  vk += ( b * rk ) / dt;

  xk_1 = xk;
  vk_1 = vk;


  
  return xk;
}


void magnet_detect() {
  loop_tracking = 0;
  digitalWrite(ledPin, HIGH);
  delay(1);
  digitalWrite(ledPin, LOW);
  currentTime = micros();
  
  
  unsigned long took = currentTime-lastTime;
  goal = alphaBeta(took);
//  Serial.print("goal: ");
//  Serial.println(goal);
//  Serial.print("took: ");
//  Serial.println(took);
//  Serial.println("---------");
  lastTime = currentTime;
}



// Fill the dots one actual the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delayMicroseconds(wait);
  }
}

void rainbowCycle(uint32_t goalMicro, int lambda, int multiplier) {
  Timer1.restart();
  uint16_t i, j;
  for(j=0; j<256; j++) { // 1 cycle of all colors on wheel
    
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      
    }
    strip.show();
    if(Timer1.read() > (goalMicro - 1000)){
//        Serial.print("got to: ");
//        Serial.println(j);
        //Serial.print("took: ");
        //Serial.println(Timer1.read()/1000);
        return;
    }
    delayMicroseconds(lambda);
    j = j+ multiplier;
  }
  
 // Serial.print("took: ");
 // Serial.println(Timer1.read()/1000);
}


uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
