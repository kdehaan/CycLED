#include <Adafruit_NeoPixel.h>
#include "TimerOne.h"
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 9
#define HALL 3
#define ledPin 13
#define numLeds 60

volatile unsigned long now = 0;
volatile unsigned long lastTime = 0;
volatile unsigned long currentTime = 0;
volatile unsigned long goal = 600000;
unsigned long microDif = goal;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLeds, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(ledPin, OUTPUT);
  
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
  //attachInterrupt(digitalPinToInterrupt(HALL), magnet_detect, FALLING);
  Timer1.initialize(1000000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.stop();
  Timer1.restart();
  //Timer1.attachInterrupt(timeout);
}

void loop() {
  int value = 0;
  for (int i = 0; i<=11; i++){
    int lambda = 200*i;
    fastestRainbowCycle(1000000000, lambda);
    Serial.print("Lambda: ");
    Serial.println(lambda);
    
  }
  colorWipe(strip.Color(0, 0, 0), 0);
  exit(0);
  
  
//  Serial.print("STARTING LOOP with goal ");
//  Serial.println(goal/1000);
//  int lambda;
//  rainbowCycle(100000, 0);
//  
////  lambda = ((goal/1000) - 412)*3.89;
////  rainbowCycle(goal, lambda);
////  if(goal > 412000) {
////    lambda = ((goal/1000) - 412)*3.89;
////    rainbowCycle(goal, lambda);
////  } else if (goal > 206000) {
////    lambda = ((goal/1000) - 206)*3.89;
////    fastRainbowCycle(goal, lambda);
////  } else if (goal > 103000) {
////    lambda = ((goal/1000) - 103)*3.89;
////    fasterRainbowCycle(goal, lambda);
////  } else if (goal > 51) {
////    lambda = ((goal/1000) - 51)*3.89;
////    fasterRainbowCycle(goal, lambda);
////  }
//  Serial.print("And lambda: ");
//  Serial.println(lambda);
////  
//  
//  now = micros();


}


void magnet_detect() {
  
  
}

//void timeout() {
//  unsigned long actual = micros()-now;
//  Serial.print("took ");
//  Serial.println(actual/1000);
//  magnet_detect();
//}


// Fill the dots one actual the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delayMicroseconds(wait);
  }
}

void fastestRainbowCycle(uint32_t goalMicro, int lambda) { //for less than 412
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
        Serial.print("took: ");
        Serial.println(Timer1.read()/1000);
        return;
    }
    delayMicroseconds(lambda);
    j = j+7;
  }
  
  Serial.print("took: ");
  Serial.println(Timer1.read()/1000);
}

void fasterRainbowCycle(uint32_t goalMicro, int lambda) { //for less than 412
  Timer1.restart();
  uint16_t i, j;
  for(j=0; j<256; j++) { // 1 cycle of all colors on wheel
    
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      
    }
    strip.show();
    if(Timer1.read() > (goalMicro - 1000)){
        Serial.print("got to: ");
        Serial.println(j);
        Serial.print("took: ");
        Serial.println(Timer1.read());
        return;
    }
    delayMicroseconds(lambda);
    j = j+3;
  }
  
  Serial.print("took: ");
  Serial.println(Timer1.read()/1000);
}

void fastRainbowCycle(uint32_t goalMicro, int lambda) { //for less than 412
  Timer1.restart();
  uint16_t i, j;
  for(j=0; j<256; j++) { // 1 cycle of all colors on wheel
    j++;
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      
    }
    strip.show();
    if(Timer1.read() > (goalMicro - 1000)){
        Serial.print("got to: ");
        Serial.println(j);
        Serial.print("took: ");
        Serial.println(Timer1.read());
        return;
    }
    delayMicroseconds(lambda);
  }
  Serial.print("took: ");
  Serial.println(Timer1.read()/1000);
}

void rainbowCycle(uint32_t goalMicro, int lambda) {
  Timer1.restart();
  uint16_t i, j;
  for(j=0; j<256; j++) { // 1 cycle of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      
    }
    strip.show();
    if(Timer1.read() > (goalMicro - 1000)){
        Serial.print("got to: ");
        Serial.println(j);
        Serial.print("took: ");
        Serial.println(Timer1.read());
        return;
    }
    delayMicroseconds(lambda);
  }
  Serial.print("took: ");
  Serial.println(Timer1.read()/1000);
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
