#define FASTLED_INTERNAL
#include <FastLED.h>
#include <math.h>
#define LED_PIN 6
#define NUM_LEDS 12 
#define TIME 60 




CRGB leds[NUM_LEDS];
int counting = 0; 
//int ledStrip = {2,3,4,5,6,7,8,9,10,11,12,13};

void setup() {
  
  // put your setup code here, to run once:
  FastLED.addLeds <WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.clear();
  FastLED.show();
  

}

void loop() {
  // put your main code here, to run repeatedly:
  int delaytime = 5000; //milliseconds 

  //turn LED on 
  for (int i = NUM_LEDS; i > 0; i++){
    leds[i] = leds[i] = CRGB(255, 90, 0);
    show(leds[i]);
    delay(delaytime); 
    
  }

  //turn LED off
  for (int i = NUM_LEDS; i> 0; i--){
   leds[i] = leds[i] = CRGB(0, 0, 0);
    show(leds[i]);
    delay(delaytime); 
  }
  

}
