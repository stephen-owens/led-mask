#include "FastLED.h"

// Prototype controller for a matrix of neopixel LEDs
// Bluetooth LE controllable via accompanying app (iOS and Anxdroid)
// 
// Features 5 modes:
// 
//      Visualisations  -  LED animations based on examples from Mark Kriegsman, 2014
//      Sound React     -  LED visualiser that modulates in reacts to beat of music
//      Hush React      -  LEDs represent the power of a hush plug
//      Scroll Text     -  LEDs display user define scrolling text 
//      Custom Display  -  LEDs are defined by the user
//
//    -Stephen Owens & Matt Jones, 2017

#define DATA_PIN            2       //control pin for LEDs
#define LED_TYPE            WS2811 
#define COLOR_ORDER         GRB
#define NUM_LEDS            64      //8x8 matrix of LEDs
#define BRIGHTNESS          16      //keep low to prevent LEDs blinding mask wearer
#define FRAMES_PER_SECOND   120

CRGB leds[NUM_LEDS];

void setup() 
{
  delay(3000);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

}

typedef void (*SimpleVisualisationList[])();
SimpleVisualisationList patterns  = { modeRainbow, modeGlitterRainbow, modeConfetti, modeSinelon, modeBPM, modeJuggle}; //array of available visualisations

uint8_t currentVisualisation = 0; //index of current visulisation
uint8_t gHue = 0; // rotating base-color used by the stock visualisations

void loop() 
{
  patterns[currentVisualisation];
  
  FastLED.show();  
  FastLED.delay(1000/FRAMES_PER_SECOND); //delay to keep the framerate 

  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the base-color through the rainbow

  visualisations(0); //placeholder to cycle through the visualisations, this will be replaced by response from bluetooth module

}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void visualisations(int mode) 
{

  //Visualisations mode, displays animations selected by the user
  //0 cycle through all visualisations
  //1 rainbow effect    2 rainbow with glitter    3 confetti effect
  //4 sinelon effect    5 BPM effect              6 juggle effect

  switch(mode) {
    case 0: modeCycle(); break;
    case 1: modeRainbow(); break;
    case 2: modeGlitterRainbow(); break;
    case 3: modeConfetti(); break;
    case 4: modeSinelon(); break;
    case 5: modeBPM(); break;
    case 6: modeJuggle(); break;
    default: modeCycle(); break;
    break;
  }
}

void soundReact() 
{
  //sound equaliser that responds to music
  
}

void hushReact() 
{
  //matrix displays strength of hush plug
  
}

void scrollText(char scrollMessage[], int scrollDirection) 
{
  //scrolling text message - scrollMessage = message to display. scrollDirection = left, right, up or down
  
}

void customDisplay() 
{
  //each LED is indivudally user programmable
  
}

 void modeCycle() 
{
  //cycles through each of the visualisations
  patterns[currentVisualisation]();
  EVERY_N_SECONDS( 10 ) { currentVisualisation = (currentVisualisation + 1) % ARRAY_SIZE(patterns); } 
}

 void modeRainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

 void modeGlitterRainbow() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  modeRainbow();
  
  if( random8() < 80) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

 void modeConfetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

 void modeSinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

 void modeBPM()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

 void modeJuggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}


