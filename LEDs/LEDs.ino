#define FASTLED_INTERNAL
#include <FastLED.h>
#define LED_PIN 6
#define NUM_LEDS 12 


CRGB leds[NUM_LEDS];
int counting = 0; 

void setup() {
  
  // put your setup code here, to run once:
  FastLED.addLeds <WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.clear();
  FastLED.show();
  

}


void loop() {
  switch (counting){
    //lights blue and flash 
    case0:
    
    for(int i=0; i<NUM_LEDS; i++){
    leds[i] = CRGB(255, 0, 255);
    FastLED.setBrightness(6*i);
    FastLED.show();
    delay(50);
    }
    
  for(int i=NUM_LEDS; i>0; i--){
    leds[i] = CRGB(0, 213, 255);
    FastLED.setBrightness(60-2*i);
    FastLED.show();
    delay(50);
    counting = 1; 
    break; 
  }

  case1:
//change the light colours 
    for(int i=0; i<NUM_LEDS; i++){
    leds[i] = CRGB(255, 90, 0);
    FastLED.setBrightness(6*i);
    FastLED.show();
    delay(100);


  }

   for(int i=NUM_LEDS; i>0; i--){
    leds[i] = CRGB(255, 40, 0);
    FastLED.setBrightness(60-2*i);
    FastLED.show();
    delay(100);
    counting = 2; 
    break; 
  } 

  default:

  while(1); //freeze



  }

 
//  //turn all the lights blue and flash 
//  for(int i=0; i<NUM_LEDS; i++){
//    leds[i] = CRGB(255, 0, 255);
//    FastLED.setBrightness(6*i);
//    FastLED.show();
//    delay(50);
//  }
//
//   for(int i=NUM_LEDS; i>0; i--){
//    leds[i] = CRGB(0, 213, 255);
//    FastLED.setBrightness(60-2*i);
//    FastLED.show();
//    delay(50);
//  }

//  //change the light colours
//  for(int i=0; i<NUM_LEDS; i++){
//    leds[i] = CRGB(255, 90, 0);
//    FastLED.setBrightness(6*i);
//    FastLED.show();
//    delay(100);
//  }
//
//   for(int i=NUM_LEDS; i>0; i--){
//    leds[i] = CRGB(255, 40, 0);
//    FastLED.setBrightness(60-2*i);
//    FastLED.show();
//    delay(100);
//  } 


}
