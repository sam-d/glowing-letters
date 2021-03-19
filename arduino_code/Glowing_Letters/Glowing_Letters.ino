#include <FastLED.h>

#define LED_PIN 5              // which pin are LEDS connected to?
#define NUM_LEDS 58			//TODO: Change this value
#define COLOR_ORDER GRB //RGB
#define LED_TYPE WS2811        // i'm using WS2811s, FastLED supports lots of different types.
#define BRIGHTNESS  200
//constants for the Fire Palette
#define FRAMES_PER_SECOND 120   // Speed of the frames. Higher number is more speed.
#define COOLING  55 // 20/100
#define SPARKING 15 // 50/200
bool gReverseDirection = false; //Fire2012withpalette fire direction

//for the PIR sensor
const int sensor = 8;
const int calibrationTime = 30;   
bool trigger = false;
bool reverse = false;

//Palette related definitions
static uint8_t startIndex = 0;

struct CRGB leds[NUM_LEDS];
CRGBPalette16 gPal;
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

/* Alternative: download palettes.h from GitHub
 *  e-g here: https://github.com/SteveRMann/xmas2020/blob/51701384715bbedda1d302af4797dc0f08d04b73/src/ColorWavesWithPalettes/palettes.h
 *  include this and grab Palette from the final array
 *  */
// Gradient palette "Yellow_Orange_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ggr/tn/Yellow_Orange.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( Yellow_Orange_gp ) {
    0, 255,199,  0,
   34, 255,121,  0,
  144, 255, 63,  0,
  241, 222, 51,  1,
  255, 194, 39,  1};

// Gradient palette "gr65_hult_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/hult/tn/gr65_hult.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.

DEFINE_GRADIENT_PALETTE( gr65_hult_gp ) {
  0, 247, 176, 247,
  48, 255, 136, 255,
  89, 220, 29, 226,
  160,   7, 82, 178,
  216,   1, 124, 109,
  255,   1, 124, 109
};

// Gradient palette "ib15_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ing/general/tn/ib15.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.

DEFINE_GRADIENT_PALETTE( ib15_gp ) {
  0, 113, 91, 147,
  72, 157, 88, 78,
  89, 208, 85, 33,
  107, 255, 29, 11,
  141, 137, 31, 39,
  255,  59, 33, 89
};

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

// Gradient palette "es_autumn_19_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/autumn/tn/es_autumn_19.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 52 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_autumn_19_gp ) {
  0,  26,  1,  1,
  51,  67,  4,  1,
  84, 118, 14,  1,
  104, 137, 152, 52,
  112, 113, 65,  1,
  122, 133, 149, 59,
  124, 137, 152, 52,
  135, 113, 65,  1,
  142, 139, 154, 46,
  163, 113, 13,  1,
  204,  55,  3,  1,
  249,  17,  1,  1,
  255,  17,  1,  1
};

void setup() {
  //Serial.begin(9600);
  //setup PIR pin
  pinMode(sensor, INPUT);
  digitalWrite(sensor,LOW);
  delay(3000); // in case we do something stupid. We dont want to get locked out.

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  gPal = CRGBPalette16( CRGB::Black, CRGB::OrangeRed, CRGB::Gold, CRGB::LightYellow);
  
    // Turn on all LEDs
  for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to white 
    leds[i] = CRGB::Red;
  }     
  // Show the leds
  FastLED.show();
  delay(3000);
  FastLED.clearData();
  FastLED.show();
    
  //give the sensor some time to calibrate
  //Serial.print("calibrating sensor ");
  for(int i = 0; i < calibrationTime; i++){
    //Serial.print(".");
    delay(1000);
  }
  //Serial.println(" done");
  //Serial.println("SENSOR ACTIVE");
  delay(50);
}



void loop()
{
    if(digitalRead(sensor)==HIGH){ 
      //Serial.println("Pin: HIGH");
      trigger = true;
    }

    while(trigger){
//      for(int i = 0; i < NUM_LEDS; i++) {
//    // Set the i'th led to white 
//      leds[i] = CRGB::White;
//      }    
      currentPalette = Sunset_Real_gp; //gr65_hult_gp; //Yellow_Orange_gp;
      currentBlending = LINEARBLEND;
      if(reverse){
        startIndex -= 1;
      }else{
        startIndex += 1; //00; //startIndex + 1; /* motion speed */
      }
      FillLEDsFromPaletteColors(startIndex);
      //Serial.println(startIndex);
      //fadeColor(5);
      //Fire2012WithPalette();
      FastLED.show();
      FastLED.delay(500);
      //if(startIndex == 255 | startIndex == 0){
      //  reverse = !reverse; //change direction
      //}
      if(digitalRead(sensor) == LOW){   
        //Serial.println("Pin: LOW");
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
        //colorIndex += 1; //(int) floor(255/NUM_LEDS) % 255; //sample uniformly accross palette
        colorIndex = colorIndex % 255;
     }
}
 
void fadeColor(uint8_t wait) {
  for( uint16_t j=BRIGHTNESS; j<255; j++) {
    FastLED.setBrightness(j);
    FastLED.show();
    FastLED.delay(wait);
  }
 
  for( uint16_t j=255; j>BRIGHTNESS; j--) {
    FastLED.setBrightness(j);
    FastLED.show();
    FastLED.delay(wait);
  }
  FastLED.delay(wait);
}

 

// Gradient palette "ib37_gp", originally from// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ing/general/tn/ib37.png.index.html// converted for FastLED with gammas (2.6, 2.2, 2.5)// Size: 0 bytes of program space.
//DEFINE_GRADIENT_PALETTE( ib37_gp ) {



void Fire2012WithPalette()
  { 

  random16_add_entropy( random(256));
  
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
      addGlitter(1);
     }
     }
     void addGlitter( fract8 chanceOfGlitter) 
    {
    if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::Orange;
  }
}
