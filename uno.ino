//Bibliothèque bandeau de LEDs
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 2
#define NUMPIXELS      15

//LED 13
#define LED13 13
int LED13val = 0;


//Serial communication
char inChar;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;

String mode;
int arg[4];




//ghost:100,100,100,10;






//Variables programme
int i,j,k;

//Bandeau de LEDs




// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  
  //Setup Serial communication
  Serial.begin(9600);
  inputString.reserve(200);
  
  //Setup LED13
  pinMode(LED13,OUTPUT);
  
  //BEGIN
  //Serial.println("--------BEGIN UNO--------");
  
}

void loop() {
  
  LED13val = 1 - LED13val;
  digitalWrite(LED13,LED13val);
  
  if (stringComplete == true) {
    
    //Serial.println(inputString); 
    
    
    
    i = 0;
    j = 0;
    
    mode = "";    
    arg[0] = 0;
    arg[1] = 0;
    arg[2] = 0;
    arg[3] = 0;
    
    while(inputString[i] != ':'){
    
      mode += inputString[i];
      i++;
      
    }
    
    i++;
    
    while(inputString[i] != ';'){
      
      if(inputString[i] == ','){
        
         j++; 
         
      } else {
      
         arg[j] = arg[j] * 10 + (inputString[i] - 48);
      
      }
      
      i++;
    }
    
    
    
//    Serial.print("mode : ");
//    Serial.print(mode); 
//    
//    Serial.print(", arg[0] : ");
//    Serial.print(arg[0]); 
//    
//    Serial.print(", arg[1] : ");
//    Serial.print(arg[1]); 
//    
//    Serial.print(", arg[2] : ");
//    Serial.print(arg[2]); 
//    
//    Serial.print(", arg[3] : ");
//    Serial.println(arg[3]); 
//    
    
        
    inputString = "";
    stringComplete = false;
    
  }
  
  
  if(mode == "ghost"){
    
     ghost(arg[0],arg[1],arg[2],arg[3]);
     
  }
  
  if(mode == "fixe"){
    
    for(k = 0;k<NUMPIXELS;k++){
      strip.setPixelColor(k, strip.Color(arg[0],arg[1],arg[2])); 
    }
    
    strip.show();
     
  }

  
}

//mode ghost
void ghost(int R, int G, int B, int beat){
  
  //beat 
  //R between 0 to 255
  
  float coef;
  int i2, j2;
 
  for(j2 = 0; j2 < 100; j2++){
 
    coef = (float) j2/100.0;
    
//    Serial.print("R : ");
//    Serial.print(coef*R);
//    Serial.print(" G : ");
//    Serial.print(coef*G);
//    Serial.print(" B : ");
//    Serial.println(coef*B);
    
    for(int i2 = 0;i2<NUMPIXELS;i2++){
      strip.setPixelColor(i2, strip.Color(coef*R,coef*G,coef*B));  
    }
    strip.show();
    
    delay(beat);
    
  }
  
  for(j2 = 100; j2 > 0; j2--){
 
    coef = j2/100.0;
    
//    Serial.print("R : ");
//    Serial.print(coef*R);
//    Serial.print(" G : ");
//    Serial.print(coef*G);
//    Serial.print(" B : ");
//    Serial.println(coef*B);
    
    for(i2 = 0;i2<NUMPIXELS;i2++){
      strip.setPixelColor(i2, strip.Color(coef*R,coef*G,coef*B)); 
    }
  
  strip.show();
  delay(beat);
  }
  
  
}


//Récupération Serial
void serialEvent() {
  
  
  while (Serial.available() && stringComplete == false ) {
    
    
    inChar =  (char)Serial.read(); 
    // get the new byte:
    inputString += inChar;
       
    if(inChar == ';'){
      stringComplete = true;
    }
    
    
  }
}
