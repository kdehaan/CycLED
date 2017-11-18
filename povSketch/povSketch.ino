#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif


void setup() {
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
}

void loop() {
  //colorWipe(strip.Color(0, 0, 0), 0);
  rainbowCycle(2000000);
}


void rainbowCycle(uint32_t waitMicro) {
  uint16_t i, j;
  for(j=0; j<256; j++) { // 1 cycle of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delayMicroseconds(waitMicro);
  }
}
