#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <Wire.h>

/*
  NeoPixel Strip Control
*/

/*
 * The design thought here was to have a dynamic array of strips with individual pixel lengths.
 * While some of the scaffolding for this is in the code (i.e. strips array), there are some
 * issues that need to be worked out for setting MIN/MAX, Delay, RGB Size and actual RGB/RGBW structure
 * Also, it would be nice to define cases for both NeoPixel's and DotStars if the code can
 * be made sufficiently small to handle both.
 */

/////////////////////////
// Defines
/////////////////////////
// Serial Enable
#define SERIALENABLE 1
// Troubleshoot/Debug Flag
#define TROUBLESHOOT 0
// Number of strips
#define STRIPS 1
// Default PIN for default strip
#define PIN 6
// Default pixel length for default strip
#define PIXELS 25
// Min Value for Brightness and R-G-B component values
#define MIN 0
// Max Value for Brightness and R-G-B component values
#define MAX 255
// Default Delay
#define DELAY 100
// RGB Pixel Buffer Size 3 = RGB, 4 = RGBW
#define RGB_SIZE 3
// Size of Palette Array
#define PALETTE_SIZE 32

// Colors
#define GOLD 0
#define ORANGE 1
#define DARKORANGE 2
#define RED 3
#define CRIMSON 4
#define YELLOW 5
#define OLIVE 6
#define OLIVEDRAB 7
#define DARKOLIVEGREEN 8
#define BLUEVIOLET 9
#define DARKVIOLET 10
#define DARKORCHID 11
#define DARKMAGENTA 12
#define PURPLE 13
#define INDIGO 14
#define LIGHTBLUE 15
#define LIGHTRED 16

// Command Registers
#define R_SHOW 0
#define R_INCDELAY 1
#define R_DECDELAY 2
#define R_STEP 3
#define R_SETSTRIP 4
#define R_SETPIXEL 5
#define R_COLOR 6
#define R_FADEDELAY 7
#define R_SETSTRIPS 8
#define R_SETPALREG 9
#define R_PALETTEINDEX 10
#define R_PALETTE_R 11
#define R_PALETTE_G 12
#define R_PALETTE_B 13
#define R_PALETTE_W 14
#define R_MAXPAL 15
#define R_STATUS 16

/////////////////////////
// Classes
/////////////////////////

// Strip Class
//class Strip
//{
//  Adafruit_NeoPixel strip;
//  int pixels;
//  int pixelColors[];
    
//};

/////////////////////////
// Global Variables
/////////////////////////

// Neopixel Strip Objects
Adafruit_NeoPixel strips[STRIPS];
// Strip Defaults
int stripDefaults[][2] = { { PIXELS, PIN } };

// gold, orange, darkorange, red, crimson, yellow, olive, olivedrab, darkolivegreen, lightred, white
int colors[PALETTE_SIZE][RGB_SIZE] = { {255,215,0}, {255,165,0}, {255,140,0}, {255,0,0}, {220,20,60}, {255,255,0}, {128,128,0}, {107,142,35}, {85,107,47}, { 0, 0, 128 }, { 128, 0, 0 }, { 255, 255, 255 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
int totalColors = 11;

// Pixel Pattern Buffer
int pixelPatternBuffer[STRIPS][PIXELS][RGB_SIZE] = { { { colors[RED] }, { colors[RED] }, { colors[RED] }, { colors[RED] }, { colors[RED] } } };

// Scratch Pixel & Default Pixels
int rgbPixel[RGB_SIZE] = { colors[RED] };
// Off Pixel
int offPixel[RGB_SIZE] = { 0, 0, 0 };
// Palette RGB
int paletteRGB[RGB_SIZE] = { 0, 0, 0 };
// Palette Index
int paletteIndex = 0;

// Brightness Max
int maxBrightness = MAX;

// MaxColor Time (in ms)
long maxColorTime = 300000;
// Color Timer
long timeInColor = 0;

// Fade Step
int fadeStep = -1;
// Fade Delay
int fadeDelay = 100;

// Global Mix down Color
uint32_t mixedColor;
// Current Mixed Color
int currentColor = RED;
// Current Strip
int currentStrip = 0;
// Current Pixel
int currentPixel = 0;
// Percent Full Color
int percentColor = 100;
// If One Color > -1, then only display that color
int oneColor = -1;
// If effects > 0, then enable effects
int effects = 0;

// LED Toggle Flag
int ledToggle = 1;

// Display Flag (0 = LED's off, 1 = LED's on)
int showLeds = 1;

// Delay Flag
int Delay = DELAY;

/////////////////////////
// Main Loop and Supporting Functions
/////////////////////////

// the setup function runs once when you press reset or power the board
void setup() {
  // Init Serial
  Serial.begin(19200);
  while (!Serial) ;

  // Initialize I2C bus
  Wire.begin(8);
  Wire.onReceive(i2cReceive);
  
  // Setup default LED in case we want debug output to show the controller has not crashed
  pinMode(LED_BUILTIN, OUTPUT);

  // Seed Randomizer
  randomSeed(analogRead(0));

  // Init Neopixel strips
  for (int index=0; index < STRIPS; ++index)
  {
    strips[index] = Adafruit_NeoPixel(stripDefaults[index][0],stripDefaults[index][1], NEO_GRB + NEO_KHZ800);
    strips[index].begin();
    strips[index].setBrightness(maxBrightness);

    strips[index].show();
  }

  oneColor = RED;
  currentColor = RED;
  showLeds = 1;

  for (int index=0; index < STRIPS; ++index)
    SetPixels(index,currentColor,0);
}

// *** I2C Recieve Loop
void i2cReceive(int howManyBytes)
{
  while (Wire.available() > 0) {
    char r = Wire.read();
    char v = Wire.read();
        
    switch (r) {
      case R_SHOW: {
        if (v == 0) {
          showLeds = 0;
        
          TurnOffStrips();
        } else if (v == 1) {
          TurnOnStrips(currentColor);

          showLeds = 1;          
        }
      }
      break;
      case R_INCDELAY: {
        Delay += v;
      }
      break;
      case R_DECDELAY: {
        Delay -= v;
      }
      break;
      case R_STEP: {
        fadeStep = v;
      }
      break;
      case R_SETSTRIP: {
        if (v > -1 && v <= STRIPS)
          currentStrip = v;
      }
      break;
      case R_SETPIXEL: {
        if (v > -1 && v <= strips[currentStrip].numPixels())
          currentPixel = v;
      }
      break;
      case R_COLOR: {
        if (v <= totalColors && v > -1) {
          currentColor = v;
          SetPixel(currentStrip,currentPixel,currentColor);
          Show(currentStrip);
        }
      }
      break;
      case R_FADEDELAY: {
        fadeDelay = abs(v);
      }
      break;
      case R_SETSTRIPS: {
        if (v <= totalColors && v > -1)
        {
          currentColor = v;

          for (int index=0; index < STRIPS; ++index)
            SetPixels(index,currentColor,0);
        }
      }
      break;
      case R_SETPALREG: {
        CopyPixel(colors[paletteIndex],paletteRGB);
      }
      break;
      case R_PALETTEINDEX: {
        if (v <= PALETTE_SIZE && v >= 0)
          paletteIndex = v;
      }
      break;
      case R_PALETTE_R:
      {
        paletteRGB[0] = v;
      }
      break;
      case R_PALETTE_G: {
        paletteRGB[1] = v;
      }
      break;
      case R_PALETTE_B: {
        paletteRGB[2] = v;
      }
      break;
      case R_PALETTE_W: {
        if (RGB_SIZE == 4)
        {
          paletteRGB[3] = v;
        }
      }
      break;
      case R_MAXPAL:
      {
        if (v <= PALETTE_SIZE)
        {
          totalColors = v;
        }
      }
      break;
      case R_STATUS: {
        Serial.println("****************");
        Serial.print("Strips         : ");
        Serial.println(STRIPS,DEC);
        Serial.print("Max Brightness : ");
        Serial.println(maxBrightness,DEC);
        Serial.print("Current Color  : ");
        Serial.println(currentColor,DEC);
        Serial.print("Current Pixel  : ");
        Serial.println(currentPixel,DEC);
        Serial.print("Current Strip  : ");
        Serial.println(currentStrip,DEC);
        Serial.print("Show LEDS      : ");
        Serial.println(showLeds,DEC);
        Serial.print("Time In Color  : ");
        Serial.println(timeInColor,DEC);
        Serial.print("Delay          : ");
        Serial.println(Delay,DEC);
        Serial.print("RGB Size       : ");
        Serial.println(RGB_SIZE,DEC);
        Serial.print("Fade Step      : ");
        Serial.println(fadeStep,DEC);
        Serial.print("Fade Delay     : ");
        Serial.println(fadeDelay,DEC);
        Serial.print("LED Toggle     : ");
        Serial.println(ledToggle,DEC);
        Serial.print("Color Count    : ");
        Serial.println(totalColors,DEC);
        Serial.print("Troubleshoot   : ");
        Serial.println(TROUBLESHOOT,DEC);
        Serial.print("Serial Enable  : ");
        Serial.println(SERIALENABLE,DEC);
        
        for (int index=0; index < STRIPS; ++index)
        {
          Serial.print("Strip ");
          Serial.print(index,DEC);
          Serial.print("        : LED Count = ");
          Serial.println(strips[index].numPixels(),DEC);
        }
        // Serial.print("");
        // Serial.println(,);
      }
      break;
    }
  }
}

// Serial Msg Helper
void SendMsg(String msg)
{
  Serial.println(msg);
}

// Debug Message Helper
void DebugMsg(String msg)
{
  if (TROUBLESHOOT > 0 && SERIALENABLE > 0)
  {
    SendMsg(msg);
  }
}

// *** Pixel Helper Functions

// Turn Off Strip Pixels
void TurnOffStrip(int stripIndex)
{
  SetRGBPixels(stripIndex,offPixel,0); 
}

// Turn Off All Strips
void TurnOffStrips()
{
  for (int index=0; index < STRIPS; ++index)
    TurnOffStrip(index);
}

// Turn On Strip Pixels With Given Color
void TurnOnStrip(int stripIndex, int color)
{
  SetPixels(stripIndex,color,0);
}

// Turn On All Strips
void TurnOnStrips(int color)
{
  for (int index=0; index < STRIPS; ++index)
    TurnOnStrip(index,color);
}

// *** Set Pixel Functions ***

// Show New Settings On Strip
void Show(int stripIndex)
{
  strips[stripIndex].show();
}

// Set the Color of a Specific Pixel (w/No Adjust)
void SetPixel(int stripIndex, int pixelIndex, int color)
{
  strips[stripIndex].setPixelColor(pixelIndex,GetColor(colors[color]));
}

// Set Pixels on A Strip To An RGB/W Value (w/0 Percent Adjust)
void SetRGBPixels(int strip, int rgbColor[], int wait)
{
  uint32_t c = GetColor(rgbColor);
  
  for (uint16_t index=0; index < strips[strip].numPixels(); ++index)
  {
    strips[strip].setPixelColor(index,c);
  }

  strips[strip].show();
  
  if (wait > 0)
    delay(wait);
}

// Set Pixels on A Strip (w/Percent Adjust)
void SetPixels(int strip, int color, int wait)
{
  uint32_t c = AdjustColor(color,percentColor);
  
  for (uint16_t index=0; index < strips[strip].numPixels(); ++index)
  {
    strips[strip].setPixelColor(index,c);
  }

  strips[strip].show();
  
  if (wait > 0)
    delay(wait);
}

// Mixdown RGB/W Values
uint32_t GetColor(int pixel[RGB_SIZE])
{
  uint32_t color = 0;
  
  if (RGB_SIZE == 3)
  {
    color = strips[0].Color(pixel[0],pixel[1],pixel[2]);
  }
  else if (RGB_SIZE == 4)
  {
    color = strips[0].Color(pixel[0],pixel[1],pixel[2],pixel[3]);
  }

  return (color);
}

// Copy Pixel RGB/W Values From Source pixel to dest pixel
void CopyPixel(int dst[RGB_SIZE],int src[RGB_SIZE])
{
  for(int index=0; index < RGB_SIZE; ++index)
    dst[index] = src[index];
}

// *** End Set Pixel Functions

// *** Step/Fade Helpers

// Toggle Step
int ToggleStep(int stepValue)
{
  return (stepValue < 0 ? abs(stepValue) : (stepValue * -1));
}

// Adjust Color By Percent (0 thru 100, 0 = no color, 100 = current color)
uint32_t AdjustColor(int color, int percent)
{
  float p = (float(percent) / 100.0);
  int pixelColor[RGB_SIZE];
  int tmpColor[RGB_SIZE];
  int white[4] = { 255, 255, 255, 255 };

  CopyPixel(pixelColor,colors[color]);

  for (int index=0; index < RGB_SIZE; ++index)
  {
    pixelColor[index] = int(float(colors[color][index]) * p);
  }

  return (GetColor(pixelColor));
}

// Get New Color
void NewColor()
{
  if (oneColor < 0)
    currentColor = random(0,totalColors - 1);
  else
    currentColor = oneColor;
}

// Fade Down, then Up
void Fade()
{
  // Flip fade up/down the color
  if (percentColor <= 0) {
    fadeStep = ToggleStep(fadeStep);
    percentColor = 0;
  }
  else if (percentColor >= 100) {
    fadeStep = ToggleStep(fadeStep);
    percentColor = 100;
  }
    
  percentColor += fadeStep;

  for (int index=0; index < STRIPS; ++index)
    SetPixels(index,currentColor,0);

  delay(fadeDelay);

  if (percentColor <= 0) {
    NewColor();
  }
    
  if (percentColor >= 100) {
    percentColor = 100;    
    timeInColor = 0;
  }
}

/// *** End Change/Fade Helpers

void ToggleLed()
{
  if (TROUBLESHOOT) {
    digitalWrite(LED_BUILTIN,ledToggle);
    ledToggle = !ledToggle;

    if (showLeds == 0)
      delay(Delay);
  }
}

/////////////////////////////
// Main Loop
/////////////////////////////
// the loop function runs over and over again forever
void loop() {
  if (showLeds) {
    ToggleLed();

    if (timeInColor > maxColorTime) {
      Fade();
    } else {
      delay(Delay);

      timeInColor += Delay;
    }
  }
}
