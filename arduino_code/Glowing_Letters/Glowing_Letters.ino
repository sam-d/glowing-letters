#include <FastLED.h>

#define LED_PIN 5              // which pin are LEDS connected to?
#define NUM_LEDS 58			//TODO: Change this value
#define COLOR_ORDER GRB //RGB
#define LED_TYPE WS2811        // i'm using WS2811s, FastLED supports lots of different types.
#define BRIGHTNESS  200

//for the PIR sensor
const int sensor = 8; //which PIN the sensor is connected to
const int calibrationTime = 30;   
bool trigger = false;

//Palette related definitions
static uint8_t startIndex = 0;

struct CRGB leds[NUM_LEDS];
CRGBPalette16 gPal;
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

// Gradient palette "Sunset_Real_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Sunset_Real.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( Sunset_Real_gp ) {
  0, 120,  0,  0,
  22, 179, 22,  0,
  51, 255, 104,  0,
  85, 167, 22, 18,
  135, 100,  0, 103,
  198,  16,  0, 130,
  255,   0,  0, 160
};

void setup() {
  //setup PIR pin
  pinMode(sensor, INPUT);
  digitalWrite(sensor,LOW);
  delay(3000); // in case we do something stupid. We dont want to get locked out.

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  gPal = CRGBPalette16( CRGB::Black, CRGB::OrangeRed, CRGB::Gold, CRGB::LightYellow);
  
   // Turn on all LEDs
  for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red to test all LEDs work 
    leds[i] = CRGB::Red;
  }     
  // Show the leds
  FastLED.show();
  delay(3000);
  FastLED.clearData();
  FastLED.show();
    
  //give the sensor some time to calibrate
  for(int i = 0; i < calibrationTime; i++){
    delay(1000);
  }
  delay(50);
}



void loop()
{
    if(digitalRead(sensor)==HIGH){ 
      trigger = true;
    }

    while(trigger){
      currentPalette = Sunset_Real_gp; 
      currentBlending = LINEARBLEND;
      if(reverse){
        startIndex -= 1;
      }else{
        startIndex += 1; // motion speed
      }
      FillLEDsFromPaletteColors(startIndex);
      FastLED.show();
      FastLED.delay(500); //speed of going through the palette

      if(digitalRead(sensor) == LOW){   
        trigger = false;
      }    
    }
    for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to black 
      leds[i] = CRGB::Black;
      }
      FastLED.show();     
}


void FillLEDsFromPaletteColors( uint8_t colorIndex)
{ 
     for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
        colorIndex = colorIndex % 255;
     }
}

