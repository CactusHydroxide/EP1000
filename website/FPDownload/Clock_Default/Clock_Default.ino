#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <TM1637Display.h>


// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3
#define NPX 4
#define EDT 5
#define HPLUS 6
#define MPLUS 7

//Number of NeoPixels
#define PXNUM 12

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PXNUM, NPX, NEO_GRB + NEO_KHZ800);

//Colour On
int pxR = 1;
int pxG = 1;
int pxB = 1;
int rgbCombi = 1;

//Current RGB Brightness 
int cR;
int cG;
int cB;
int stripColour;
int maxbrightpx; 

//Tm brightness
int tm = 7;

//brightness value
double bright = 1;

TM1637Display display(CLK, DIO);
//int values
int H = 0;
int M = 0;
int S = 0;

//pinvalues
int edit;
int hPlus;
int mPlus;
int editFlag = false;

//display text
const uint8_t disEdit[] = {
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,            // E
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,            // d
  SEG_E | SEG_F,                                    // i
  SEG_D | SEG_E | SEG_F | SEG_G                     // t
};

const uint8_t disDone[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
};

const uint8_t tmRGB[] = {
  SEG_E | SEG_G, //R
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G, //G
  SEG_C | SEG_D | SEG_E | SEG_F | SEG_G, //B
  SEG_G //-
};

const uint8_t brig[] ={
  SEG_C | SEG_D | SEG_E | SEG_F | SEG_G, //B
  SEG_E | SEG_G, //R
  SEG_E | SEG_F,                                    // i
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G //G
};


void setup()
{
 
  //NeoPixel 
  pixels.begin();
  
  //TM1637
  display.setBrightness(7);
  display.showNumberDecEx(8888, 0b01000000, true, 4, 0);
  delay(500);
  display.clear();

  //PushButton
  pinMode(EDT, INPUT_PULLUP);
  pinMode(HPLUS, INPUT_PULLUP);
  pinMode(MPLUS, INPUT_PULLUP);

  //Serial for testing
    Serial.begin(9600);
}

void loop()
{
  //Till next second
  int nextSec = 1000;

  
  //Change Time
  edit = digitalRead(EDT);
  if(edit == 0){
    Serial.print("in");
    editFlag = true;
    display.setSegments(disEdit,4,0);
    delay(500);
  }

  while(editFlag == true and edit == 1){
    display.clear();
    delay(5);
    showTime();
    delay(145);
    
    hPlus = digitalRead(HPLUS);
    mPlus = digitalRead(MPLUS);

    //h++
    if (hPlus == 0){
      if (H == 23){
        H = 0;
      } else {
        H++;
      }
      showTime();
    }

    //m++
    if (mPlus == 0){
      if (M == 59){
        M = 0;
      } else {
        M++;
      }
      showTime();
    }
    
    edit = digitalRead(EDT);
    if (edit == 0){
      editFlag = false;
      display.setSegments(disDone,4,0);
      delay(500);
    }
  }


  //Run Clock Display and Pixels
  //Set NeoPixel
  showPx();
  
  //Set TM1637
  showTime();

  //adjust brightness (HPLUS)
  hPlus = digitalRead(HPLUS);
    if (hPlus == 0){  
    display.setSegments(brig,4,0);
    delay(150);
    nextSec -=150;
      if(bright == 1){
        bright = 0.5;
        showPx();
      } else {
        bright = 1;
      }
    }

   //adjst Colour
   mPlus = digitalRead(MPLUS);
   if (mPlus == 0){
    if (rgbCombi == 4){
      rgbCombi = 1;
    } else {
      rgbCombi++;
    }
    display.setSegments(tmRGB,4,0);
    delay(150);
    nextSec -=150;
    switch(rgbCombi){
      case 1: 
        pxR = 1;
        pxG = 1;
        pxB = 1;
        break;
      case 2: 
        pxR = 1;
        pxG = 0;
        pxB = 0;
        break;
      case 3: 
        pxR = 0;
        pxG = 1;
        pxB = 0;
        break;
      case 4: 
        pxR = 0;
        pxG = 0;
        pxB = 1;
        break;
    }
   }
  

  //Time++
  delay(nextSec);
  S++;

  if (S == 60){
    S = 0;
    M ++;
    if (M == 60) {
      M = 0;
      H ++;
      if (H == 24){
        H = 0;
      }
    }
  } 
}

//Function to display itme on TM1637
void showTime() {
  int displayTime = H*100 + M;
  display.showNumberDecEx(displayTime, 0b01000000, true, 4, 0);
}

//Fnc to display pixels
void showPx() {
  
  maxbrightpx = floor(S/5);
  if(maxbrightpx != 0){
  pixels.fill(pixels.Color(255*pxR*bright,255*pxG*bright,255*pxB*bright), 0, maxbrightpx);
  } else {
    pixels.clear();
  }
  
  int brightlvl = (S - maxbrightpx*5)+1;
  cR = (pow(3, brightlvl) - 1 )*pxR*bright;
  cG = (pow(3, brightlvl) - 1 )*pxG*bright;
  cB = (pow(3, brightlvl) - 1 )*pxB*bright;
  
  pixels.setPixelColor(maxbrightpx, cR, cG, cB); 
  pixels.show();
}
