//  ___    ___             ___  __      __  __  __ 
// | _ \  / __|    ___    | _ \ \ \    / / |  \/  |
// |   / | (__    |___|   |  _/  \ \/\/ /  | |\/| |
// |_|_\  \___|___   ___  |_|_   _\_/\_/   |_|  |_|
// \ \    / / / __| |_  ) ( _ ) / | |_  )          
//  \ \/\/ /  \__ \  / /  / _ \ | |  / /           
//   \_/\_/   |___/ /___| \___/ |_| /___|                                                         
//
// RC (PWM) controlled NeoPixel strip/ring. 
// Details in the RCGroups forum - https://www.rcgroups.com/forums/showpost.php?p=36543522&postcount=6652
// 
// Source code downloaded from PaulHam211 https://github.com/PaulHam211/JetAfterburnerEffect-PWM-WS2812
//
// Connect PWM signal (from flight controller) in line with ESC signal wire to
// Digital IO pin 0.
// 
// Connect Neopixel strip or singular LEDs (WS2812 RGB LED) to Digital IO pin 1
// Power Digispark & LED ring from ESC BEC 5v+
// 
// Define NUMPIXELS to amount of LEDs on strip/ring
// 
// Code adapted from http://www.mostfun.de/index.php/modellbau/flugzeuge-blogansicht/314-edf-afterburner2
// CC BY (free with attribution) and who wants to find a better solution or make improvements, can do this gladly.
// -------------------//

#define PIN            1
#define NUMPIXELS      16


#include <Adafruit_NeoPixel.h>
#include <Wire.h>

const uint16_t IRange = 850;
const uint16_t Range1 = 50;//255
const uint16_t Range2 = 300;//511
const float Range3 = 500;//767

#define CH1_PIN  0    // Digital IO pin connected to the PWM signal.
double RAWPWM_CH1;
double PWM_CH1;

uint32_t sensorValue = 0;
uint8_t red,green,blue;
uint32_t TSBegin;   //Timestamp Begin
uint32_t tPulse;    //Pulslänge

int waitTime = 4;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  
// Serial.begin(9600);  // Initialize Serial port

// #if defined(__AVR_ATmega32U4__)
// while(!Serial);
//#endif
//Serial.println("Setup started");
  pinMode(CH1_PIN, INPUT);
  pixels.begin();
  randomSeed(tPulse);
}


void loop() {
  // Get PWM data & Contrain values //
  RAWPWM_CH1 = pulseIn(CH1_PIN, HIGH);
  PWM_CH1 = map(RAWPWM_CH1,936, 1941, 0, 1200);

  sensorValue =PWM_CH1;
 
  if (sensorValue > 4200000000){
    fadeInAndOut(0, 255, 0, waitTime); // Green
    }
    else{
  if (sensorValue<Range1){
    red=uint8_t(10);
  for(int i=0;i<NUMPIXELS;i++){
         // pixels.setPixelColor(i, pixels.Color(red,random(10,50), 0, 0)); 
            pixels.setPixelColor(i, pixels.Color(red,random(0,red/8),random(0,red/8))); 
      } 
     }
    else{
      if (sensorValue<Range2){
        red=uint8_t(sensorValue-Range1)*(256/(Range2-Range1));
       for(int i=0;i<NUMPIXELS;i++){
          pixels.setPixelColor(i, pixels.Color(red,random(0,red/8),random(0,red/8))); 
        }
      }
      else{
        if (sensorValue<Range3){
          //red goes to orange
          green=uint8_t((sensorValue-Range2)*(70/(Range3-Range2)));
         
         for(int i=0;i<NUMPIXELS;i++){
            pixels.setPixelColor(i, pixels.Color(255,green,random(0,green/2))); 
          }
        }
        else{
          if (sensorValue<IRange){
            green=70+uint8_t((sensorValue-Range3+1)*(150/(IRange-Range3)));
            blue=uint8_t((sensorValue-Range3)*(256/(IRange-Range3)));

          for(int i=0;i<NUMPIXELS;i++){
              pixels.setPixelColor(i, pixels.Color(random(128,255),green,blue));
            }
          }
           else{
             //Range.limit

             for(int i=0;i<NUMPIXELS;i++){
               pixels.setPixelColor(i, 255,255,255);
             }
          } 
        }
      }
    }}
    pixels.show(); // This sends the updated pixel color to the hardware.
    
  // Wait 1 second before next reading
  delay(50);
}

void fadeInAndOut(uint8_t red, uint8_t green, uint8_t blue, uint8_t wait) {
  for(uint8_t b=0; b <255; b++) {
     for(uint8_t i=0; i < pixels.numPixels(); i++) {
        pixels.setPixelColor(i, red*b/255, green*b/255, blue*b/255);
     }
     pixels.show();
     delay(wait);
  }
  

   for(uint8_t b=255; b > 0; b--) {
     for(uint8_t i=0; i < pixels.numPixels(); i++) {
        pixels.setPixelColor(i, red*b/255, green*b/255, blue*b/255);
     }
     pixels.show();
     delay(wait);
  }}


