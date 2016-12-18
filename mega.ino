#include <Servo.h>
#define lightPin A2

const int btnStartPin = 52;
int btnStart;

const int dataIn = 2;
const int load = 3;
const int clock = 4;

const int trigPin = 6;
const int echoPin = 7;
long duration, distance;

const int vibreursPin = 49;
const int pokPin = 47;

const int aiguillePin = 11;
const int horlogePin = 12;

const int LED13 =  13;    //LED13
int LED13val = 0;

Servo servoAiguille;
Servo servoHorloge;
float real = 0;



//Serial communication
char inChar;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;

String mode;
int arg[5];

//88

    const int time = 1;
     
    int maxInUse = 4;    //change this variable to set how many MAX7219's you'll use
     
    int e = 0;           // just a variable
     
    // define max7219 registers
    byte max7219_reg_noop        = 0x00;
    byte max7219_reg_digit0      = 0x01;
    byte max7219_reg_digit1      = 0x02;
    byte max7219_reg_digit2      = 0x03;
    byte max7219_reg_digit3      = 0x04;
    byte max7219_reg_digit4      = 0x05;
    byte max7219_reg_digit5      = 0x06;
    byte max7219_reg_digit6      = 0x07;
    byte max7219_reg_digit7      = 0x08;
    byte max7219_reg_decodeMode  = 0x09;
    byte max7219_reg_intensity   = 0x0a;
    byte max7219_reg_scanLimit   = 0x0b;
    byte max7219_reg_shutdown    = 0x0c;
    byte max7219_reg_displayTest = 0x0f;
     
    void putByte(byte data) {
      byte i = 8;
      byte mask;
      while(i > 0) {
        mask = 0x01 << (i - 1);      // get bitmask
        digitalWrite( clock, LOW);   // tick
        if (data & mask){            // choose bit
          digitalWrite(dataIn, HIGH);// send 1
        }else{
          digitalWrite(dataIn, LOW); // send 0
        }
        digitalWrite(clock, HIGH);   // tock
        --i;                         // move to lesser bit
      }
    }
     
    void maxSingle( byte reg, byte col) {    
    //maxSingle is the "easy"  function to use for a single max7219
     
      digitalWrite(load, LOW);       // begin    
      putByte(reg);                  // specify register
      putByte(col);//((data & 0x01) * 256) + data >> 1); // put data  
      digitalWrite(load, LOW);       // and load da stuff
      digitalWrite(load,HIGH);
    }
     
    void maxAll (byte reg, byte col) {    // initialize  all  MAX7219's in the system
      int c = 0;
      digitalWrite(load, LOW);  // begin    
      for ( c =1; c<= maxInUse; c++) {
      putByte(reg);  // specify register
      putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
        }
      digitalWrite(load, LOW);
      digitalWrite(load,HIGH);
    }
     
    void maxOne(byte maxNr, byte reg, byte col) {    
    //maxOne is for addressing different MAX7219's,
    //while having a couple of them cascaded
     
      int c = 0;
      digitalWrite(load, LOW);  // begin    
     
      for ( c = maxInUse; c > maxNr; c--) {
        putByte(0);    // means no operation
        putByte(0);    // means no operation
      }
     
      putByte(reg);  // specify register
      putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
     
      for ( c =maxNr-1; c >= 1; c--) {
        putByte(0);    // means no operation
        putByte(0);    // means no operation
      }
     
      digitalWrite(load, LOW); // and load da stuff
      digitalWrite(load,HIGH);
    }


int mode88 = false;
int onOff88 = false;




//Variables programme
int i,j,p; //p : pok

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

boolean hover() {

  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
//  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
 
  if (distance >= 2 && distance <= 10){
    return true;
  }
  else {
    return false;
  }
}


void setup(){
  
    
  //Setup Serial communication
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial1.print("fixe:0,0,0;");
  
  inputString.reserve(200);
  
  //Setup LED13
  pinMode(LED13,OUTPUT);
  pinMode(btnStartPin,INPUT);
  
  pinMode(36,OUTPUT);
  pinMode(37,OUTPUT);
  pinMode(38,OUTPUT);
  pinMode(39,OUTPUT);
  
  pinMode(lightPin,INPUT);
  
  pinMode(vibreursPin,OUTPUT);
  pinMode(pokPin,OUTPUT);
  
  pinMode(dataIn, OUTPUT);
  pinMode(clock,  OUTPUT);
  pinMode(load,   OUTPUT);
  
  //initiation of the max 7219
      maxAll(max7219_reg_scanLimit, 0x07);      
      maxAll(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
      maxAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
      maxAll(max7219_reg_displayTest, 0x00); // no display test
       for (e=1; e<=8; e++) {    // empty registers, turn all LEDs off
        maxAll(e,0);
      }
      maxAll(max7219_reg_intensity, 0x0f & 0x0f);    // the first 0x0f is the value you can set
    
  
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);


  servoAiguille.attach(aiguillePin);
  servoHorloge.attach(horlogePin);
  
  
  //BEGIN
  //Serial.println("");
  //Serial.println("--------BEGIN--------");
  
  
}

void loop(){
  
  LED13val = 1 - LED13val;
  digitalWrite(LED13,LED13val);
  
  //envoi et récupération des informations
  mode = "";    
  arg[0] = 0;
  arg[1] = 0;
  arg[2] = 0;
  arg[3] = 0;
  arg[4] = 0;
  
  Servo::refresh();



  //envoi de la valeur des entrée
  //Serial.println("inBtn1");
  
  
  //récupération des valeurs de sortie
  if (stringComplete == true) {
    
    //Serial.println(inputString); 
        
    i = 0;
    j = 0;
    

    
    while(inputString[i] != ':' && inputString[i] != ';'){
    
      mode += inputString[i];
      i++;
      
    }
    
    if(inputString[i] != ';'){ //arguments
      i++;
  
    
      while(inputString[i] != ';'){
        
        
        
        if(inputString[i] == ','){
          
           j++; 
           
        } else {
        
           arg[j] = arg[j] * 10 + (inputString[i] - 48);
        
        }
        
        i++;
        
      }
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
//    Serial.print(arg[3]); 
//    
//    Serial.print(", arg[4] : ");
//    Serial.println(arg[4]); 
    
    
        
    inputString = "";
    stringComplete = false;
    
  }
  
  
  //-----------------LED-------------------
  
  if(mode == "outLED"){
     
       digitalWrite(arg[0]+36, arg[1]); //+10 -> offset
 
  }
  
  
  //---------------RGB----------------> outRGB:1,0,0,200,20;
  
  
  //Serial1 on pins 19 (RX) and 18 (TX)
  
  if(mode == "outRGB"){
    
     if(arg[0] == 0){ //fixe
       
       Serial1.print("fixe:");
       Serial1.print(arg[1]);
       Serial1.print(",");
       Serial1.print(arg[2]);
       Serial1.print(",");
       Serial1.print(arg[3]);
       Serial1.print(";");
       
       
     }
     
     if(arg[0] == 1){ //ghost
       
       Serial1.print("ghost:");
       Serial1.print(arg[1]);
       Serial1.print(",");
       Serial1.print(arg[2]);
       Serial1.print(",");
       Serial1.print(arg[3]);
       Serial1.print(",");
       Serial1.print(arg[4]);
       Serial1.print(";");
       
     }
     
  }
  

  
  //-----------------88-------------------
  
  if(mode == "out88"){ //OU mode RANDOM !

      onOff88 = arg[0];
      mode88 = arg[1];

  }
  

  
  if(onOff88 == 0){
    
    
        maxSingle(1,0);
        maxSingle(2,0);
        maxSingle(3,0);
        maxSingle(4,0);
        maxSingle(5,0);
        maxSingle(6,0);
        maxSingle(7,0);
        maxSingle(8,0);

    
  }
  
  if(onOff88 == 1 && mode88 == 0){
    
        maxSingle(1,random(0,255));
        maxSingle(2,random(0,255));
        maxSingle(3,random(0,255));
        maxSingle(4,random(0,255));
        maxSingle(5,random(0,255));
        maxSingle(6,random(0,255));
        maxSingle(7,random(0,255));
        maxSingle(8,random(0,255));
        
        delay(50);
    
  }
  
  //--------------Distance -------------
    if(mode == "inDistance"){ //OU mode RANDOM !

      hover();

      Serial.print("inDistance:");
      Serial.print(distance);
      Serial.print(";");

    }
    
    if(mode == "inHover"){
      
      Serial.print("inHover:");
      Serial.print(hover());
      Serial.print(";");
      
    }
    
    //------------Servo --------------
    
    if(mode == "outAiguille"){
      
       real = floor(188*(100-arg[0])/100+12);
       if(real<12) real = 12;
       if(real>180) real = 180;
       servoAiguille.write(real);
       
  
    }
    
   if(mode == "outHorloge"){
     

      
       real = floor(188*(100-arg[0])/100+12);
       if(real<12) real = 12;
       if(real>180) real = 180;
       servoHorloge.write(real);

  
    }
    
    //------------VIBREURS--------------> outVibreurs:0;
    if(mode == "outVibreurs"){
      
     for(p = 0;p < arg[0]; p++){
      
        digitalWrite(vibreursPin,HIGH);
        delay(arg[1]);
        digitalWrite(vibreursPin,LOW);
        delay(arg[2]);

       
     }
      
      
    }
  
    //-------------POK-----------------> outPok:3,1000;
    if(mode == "outPok"){
      
     for(p = 0;p < arg[0]; p++){
      
        digitalWrite(pokPin,HIGH);
        delay(1000);
        digitalWrite(pokPin,LOW);
        
        delay(arg[1]);

       
     }
      
      
    }
    
    //---------------BTN--------------
    if(mode == "inStart"){
      
        Serial.print("inStart:");
        Serial.print(btnStart);
        Serial.print(";");
        
        btnStart = 0;
    }
    
    //-------------Light-------------
    
      
    
        
    if(mode == "inLight"){
      
      Serial.print("inLight:");
      Serial.print(1024 - analogRead(lightPin));
      Serial.print(";");
      
    }
    
    
    
    
    
    
    
    
    
    
    
    if(digitalRead(btnStartPin) == 1){
      
       btnStart = 1; 
      
    }
  
  
}






