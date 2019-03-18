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
// Defines
/////////////////////////
// Serial Enable
#define SERIALENABLE 1
// Troubleshoot/Debug Flag
#define TROUBLESHOOT 0
// Debug Delay
#define DEBUGDELAY 500
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

// Initial Effects State On Or Off (0 = off > 0 = On)
#define EFFECTS_INITIAL_STATE 1
// Initial Effect
#define INITIAL_EFFECT 0
// Fade Effect
#define EFFECT_FADE 0
// Defined Pattern State (all pixels and all strips defined)
#define EFFECT_DEFINED_PATTERN 1

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
#define R_DEBUG 17
#define R_EFFECTSTATE 18
#define R_COLORTIME 19
#define R_COLORTIMEMULT 20
#define R_PERCENTCOLOR 21
#define R_EFFECT 22

// Palette Color Definitions

#if RGB_SIZE > 3
  // RGBW Color Definitions
  #define OFF_RGB   { 0, 0, 0, 0 }

  // index = 3 is white or brightness??? Before using 4 color pixels, check this (and modify "brightness" functions to "SetWhite..." etc...
  
  // Pinks
  #define PINK_RGB            { 255, 192, 203, 128 }
  #define LIGHTPINK_RGB       { 255, 182, 193, 128 }
  #define HOTPINK_RGB         { 255, 105, 180, 128 }
  #define DEEPPINK_RGB        { 255, 20, 147, 128 }
  #define PALEVIOLETRED_RGB   { 219, 112, 147, 128 }
  #define MEDIUMVIOLETRED_RGB { 199, 21, 133, 128 }

  // Reds
  #define LIGHTSALMON_RGB     { 255, 160, 122, 128 }
  #define SALMON_RGB          { 250, 128, 114, 128 }
  #define DARKSALMON_RGB      { 233, 150, 122, 128 }
  #define LIGHTCORAL_RGB      { 240, 128, 128, 128 }
  #define INDIANRED_RGB       { 205, 92, 92, 128 }
  #define CRIMSON_RGB         { 220, 20, 60, 128 }
  #define FIREBRICK_RGB       { 178, 34, 34, 128 }
  #define DARKRED_RGB         { 139, 0, 0, 128 }
  #define RED_RGB             { 255, 0, 0, 128 }

  // Orange
  #define ORANGERED_RGB       { 255, 69, 0, 128 }
  #define TOMATO_RGB          { 255, 99, 71, 128 }
  #define CORAL_RGB           { 255, 127, 80, 128 }
  #define DARKORANGE_RGB      { 255, 140, 0, 128 }
  #define ORANGE_RGB          { 255, 165, 0, 128 }

  // Yellows
  #define YELLOW_RGB          { 255, 255, 0, 128 }
  #define LIGHTYELLOW_RGB     { 255, 255, 224, 128 }
  #define LEMONCHIFFON_RGB    { 255, 250, 205, 128 }
  #define LIGHTGOLDENRODYELLOW_RGB       { 250, 250, 210, 128 }
  #define PAPAYAWHIP_RGB      { 255, 239, 213, 128 }
  #define MOCCASIN_RGB        { 255, 228, 181, 128 }
  #define PEACHPUFF_RGB       { 255, 218, 185, 128 }
  #define PALEGOLDENROD_RGB   { 238, 232, 170, 128 }
  #define KHAKI_RGB           { 240, 230, 140, 128 }
  #define DARKKHAKI_RGB       { 189, 183, 107, 128 }
  #define GOLD_RGB            { 255, 215, 0, 128 }

  // Browns
  #define CORNSILK_RGB        { 255, 248, 220, 128 }
  #define BLANCHEDALMOND_RGB  { 255, 235, 205, 128 }
  #define BISQUE_RGB          { 255, 228, 196, 128 }
  #define NAVAJOWHITE_RGB     { 255, 222, 173, 128 }
  #define WHEAT_RGB           { 245, 222, 179, 128 }
  #define BURLYWOOD_RGB       { 222, 184, 135, 128 }
  #define TAN_RGB             { 210, 180, 140, 128 }
  #define ROSYBROWN_RGB       { 188, 143, 143, 128 }
  #define SANDYBROWN_RGB      { 244, 164, 96, 128 }
  #define GOLDENROD_RGB       { 218, 165, 32, 128 }
  #define DARKGOLDENROD_RGB   { 184, 134, 11, 128 }
  #define PERU_RGB            { 205, 133, 63, 128 }
  #define CHOCOLATE_RGB       { 210, 105, 30, 128 }
  #define SADDLEBROWN_RGB     { 139, 69, 19, 128 }
  #define SIENNA_RGB          { 160, 82, 45, 128 }
  #define BROWN_RGB           { 165, 42, 42, 128 }
  #define MAROON_RGB          { 128, 0, 0, 128 }

  // Greens
  #define DARKOLIVEGREEN_RGB  { 85, 107, 47, 128 }
  #define OLIVE_RGB           { 128, 128, 0, 128 }
  #define OLIVEDRAB_RGB       { 107, 142, 35, 128 }
  #define YELLOWGREEN_RGB     { 154, 205, 50, 128 }
  #define LIMEGREEN_RGB       { 50, 205, 50, 128 }
  #define LIME_RGB            { 0, 255, 0, 128 }
  #define LAWNGREEN_RGB       { 124, 252, 0, 128 }
  #define CHARTREUSE_RGB      { 127, 255, 0, 128 }
  #define GREENYELLOW_RGB     { 173, 255, 47, 128 }
  #define SPRINGGREEN_RGB     { 0, 255, 127, 128 }
  #define MEDIUMSPRINGGREEN_RGB { 0, 250, 154, 128 }
  #define LIGHTGREEN_RGB      { 144, 238, 144, 128 }
  #define PALEGREEN_RGB       { 152, 251, 152, 128 }
  #define DARKSEAGREEN_RGB    { 143, 188, 143, 128 }
  #define MEDIUMAQUAMARINE_RGB  { 102, 205, 170, 128 }
  #define MEDIUMSEAGREEN_RGB  { 60, 179, 113, 128 }
  #define SEAGREEN_RGB        { 46, 139, 87, 128 }
  #define FORESTGREEN_RGB     { 34, 139, 34, 128 }
  #define GREEN_RGB           { 0, 128, 0, 128 }
  #define DARKGREEN_RGB       { 0, 100, 0, 128 }

  // Cyans
  #define AQUA_RGB            { 0, 255, 255, 128 }
  #define CYAN_RGB            { 0, 255, 255, 128 }
  #define LIGHTCYAN_RGB       { 224, 255, 255, 128 }
  #define PALETURQUOISE_RGB   { 175, 238, 238, 128 }
  #define AQUAMARINE_RGB      { 127, 255, 212, 128 }
  #define TURQUOISE_RGB       { 64, 224, 208, 128 }
  #define MEDIUMTURQUOISE_RGB { 72, 209, 204, 128 }
  #define DARKTURQUOISE_RGB   { 0, 206, 209, 128 }
  #define LIGHTSEAGREEN_RGB   { 32, 178, 170, 128 }
  #define CADETBLUE_RGB       { 95, 158, 160, 128 }
  #define DARKCYAN_RGB        { 0, 139, 139, 128 }
  #define TEAL_RGB            { 0, 128, 128, 128 }

  // Blues
  #define LIGHTSTEELBLUE_RGB  { 176, 196, 222, 128 }
  #define POWDERBLUE_RGB      { 176, 224, 230, 128 }
  #define LIGHTBLUE_RGB       { 173, 216, 230, 128 }
  #define SKYBLUE_RGB         { 135, 206, 235, 128 }
  #define LIGHTSKYBLUE_RGB    { 135, 206, 250, 128 }
  #define DEEPSKYBLUE_RGB     { 0, 191, 255, 128 }
  #define DODGERBLUE_RGB      { 30, 144, 255, 128 }
  #define CORNFLOWERBLUE_RGB  { 100, 149, 237, 128 }
  #define STEELBLUE_RGB       { 70, 130, 180, 128 }
  #define ROYALBLUE_RGB       { 65, 105, 225, 128 }
  #define BLUE_RGB            { 0, 0, 255, 128 }
  #define MEDIUMBLUE_RGB      { 0, 0, 205, 128 }
  #define DARKBLUE_RGB        { 0, 0, 139, 128 }
  #define NAVY_RGB            { 0, 0, 128, 128 }
  #define MIDNIGHTBLUE_RGB    { 25, 25, 112, 128 }

  // Purples
  #define LAVENDER_RGB        { 230, 230, 250, 128 }
  #define THISTLE_RGB         { 216, 191, 216, 128 }
  #define PLUM_RGB            { 221, 160, 221, 128 }
  #define VIOLET_RGB          { 238, 130, 238, 128 }
  #define ORCHID_RGB          { 218, 112, 214, 128 }
  #define FUCHSIA_RGB         { 255, 0, 255, 128 }
  #define MAGENTA_RGB         { 255, 0, 255, 128 }
  #define MEDIUMORCHID_RGB    { 186, 85, 211, 128 }
  #define MEDIUMPURPLE_RGB    { 147, 112, 219, 128 }
  #define BLUEVIOLET_RGB      { 138, 43, 226, 128 }
  #define DARKVIOLET_RGB      { 148, 0, 211, 128 }
  #define DARKORCHID_RGB      { 153, 50, 204, 128 }
  #define DARKMAGENTA_RGB     { 139, 0, 139, 128 }
  #define PURPLE_RGB          { 128, 0, 128, 128 }
  #define INDIGO_RGB          { 75, 0, 130, 128 }
  #define DARKSLATEBLUE_RGB   { 72, 61, 139, 128 }
  #define SLATEBLUE_RGB       { 106, 90, 205, 128 }
  #define MEDIUMSLATEBLUE_RGB { 123, 104, 238, 128 }

  // Whites
  #define WHITE_RGB           { 255, 255, 255, 128 }
  #define SNOW_RGB            { 255, 250, 250, 128 }
  #define HONEYDEW_RGB        { 240, 255, 240, 128 }
  #define MINTCREAM_RGB       { 245, 255, 250, 128 }
  #define AZURE_RGB           { 240, 255, 255, 128 }
  #define ALICEBLUE_RGB       { 240, 248, 255, 128 }
  #define GHOSTWHITE_RGB      { 248, 248, 255, 128 }
  #define WHITESMOKE_RGB      { 245, 245, 245, 128 }
  #define SEASHELL_RGB        { 255, 245, 238, 128 }
  #define BEIGE_RGB           { 245, 245, 220, 128 }
  #define OLDLACE_RGB         { 253, 245, 230, 128 }
  #define FLORALWHITE_RGB     { 255, 250, 240, 128 }
  #define IVORY_RGB           { 255, 255, 240, 128 }
  #define ANTIQUEWHITE_RGB    { 250, 235, 215, 128 }
  #define LINEN_RGB           { 250, 240, 230, 128 }
  #define LAVENDERBLUSH_RGB   { 255, 240, 245, 128 }
  #define MISTYROSE_RGB       { 255, 228, 225, 128 }

  // Grays
  #define GAINSBORO_RGB       { 220, 220, 220, 128 }
  #define LIGHTGRAY_RGB       { 211, 211, 211, 128 }
  #define SILVER_RGB          { 192, 192, 192, 128 }
  #define DARKGRAY_RGB        { 169, 169, 169, 128 }
  #define GRAY_RGB            { 128, 128, 128, 128 }
  #define DIMGRAY_RGB         { 105, 105, 105, 128 }
  #define LIGHTSLATEGRAY_RGB  { 119, 136, 153, 128 }
  #define SLATEGRAY_RGB       { 112, 128, 144, 128 }
  #define DARKSLATEGRAY_RGB   { 47, 79, 79, 128 }
  #define BLACK_RGB           { 0, 0, 0, 0 }

#else
  // RGB COlor Definitions
  #define OFF_RGB             { 0, 0, 0 }

  // Pinks
  #define PINK_RGB            { 255, 192, 203 }
  #define LIGHTPINK_RGB       { 255, 182, 193 }
  #define HOTPINK_RGB         { 255, 105, 180 }
  #define DEEPPINK_RGB        { 255, 20, 147 }
  #define PALEVIOLETRED_RGB   { 219, 112, 147 }
  #define MEDIUMVIOLETRED_RGB { 199, 21, 133 }

  // Reds
  #define LIGHTSALMON_RGB     { 255, 160, 122 }
  #define SALMON_RGB          { 250, 128, 114 }
  #define DARKSALMON_RGB      { 233, 150, 122 }
  #define LIGHTCORAL_RGB      { 240, 128, 128 }
  #define INDIANRED_RGB       { 205, 92, 92 }
  #define CRIMSON_RGB         { 220, 20, 60 }
  #define FIREBRICK_RGB       { 178, 34, 34 }
  #define DARKRED_RGB         { 139, 0, 0 }
  #define RED_RGB             { 255, 0, 0 }

  // Orange
  #define ORANGERED_RGB       { 255, 69, 0 }
  #define TOMATO_RGB          { 255, 99, 71 }
  #define CORAL_RGB           { 255, 127, 80 }
  #define DARKORANGE_RGB      { 255, 140, 0 }
  #define ORANGE_RGB          { 255, 165, 0 }

  // Yellows
  #define YELLOW_RGB          { 255, 255, 0 }
  #define LIGHTYELLOW_RGB     { 255, 255, 224 }
  #define LEMONCHIFFON_RGB    { 255, 250, 205 }
  #define LIGHTGOLDENRODYELLOW_RGB       { 250, 250, 210 }
  #define PAPAYAWHIP_RGB      { 255, 239, 213 }
  #define MOCCASIN_RGB        { 255, 228, 181 }
  #define PEACHPUFF_RGB       { 255, 218, 185 }
  #define PALEGOLDENROD_RGB   { 238, 232, 170 }
  #define KHAKI_RGB           { 240, 230, 140 }
  #define DARKKHAKI_RGB       { 189, 183, 107 }
  #define GOLD_RGB            { 255, 215, 0 }

  // Browns
  #define CORNSILK_RGB        { 255, 248, 220 }
  #define BLANCHEDALMOND_RGB  { 255, 235, 205 }
  #define BISQUE_RGB          { 255, 228, 196 }
  #define NAVAJOWHITE_RGB     { 255, 222, 173 }
  #define WHEAT_RGB           { 245, 222, 179 }
  #define BURLYWOOD_RGB       { 222, 184, 135 }
  #define TAN_RGB             { 210, 180, 140 }
  #define ROSYBROWN_RGB       { 188, 143, 143 }
  #define SANDYBROWN_RGB      { 244, 164, 96 }
  #define GOLDENROD_RGB       { 218, 165, 32 }
  #define DARKGOLDENROD_RGB   { 184, 134, 11 }
  #define PERU_RGB            { 205, 133, 63 }
  #define CHOCOLATE_RGB       { 210, 105, 30 }
  #define SADDLEBROWN_RGB     { 139, 69, 19 }
  #define SIENNA_RGB          { 160, 82, 45 }
  #define BROWN_RGB           { 165, 42, 42 }
  #define MAROON_RGB          { 128, 0, 0 }

  // Greens
  #define DARKOLIVEGREEN_RGB  { 85, 107, 47 }
  #define OLIVE_RGB           { 128, 128, 0 }
  #define OLIVEDRAB_RGB       { 107, 142, 35 }
  #define YELLOWGREEN_RGB     { 154, 205, 50 }
  #define LIMEGREEN_RGB       { 50, 205, 50 }
  #define LIME_RGB            { 0, 255, 0 }
  #define LAWNGREEN_RGB       { 124, 252, 0 }
  #define CHARTREUSE_RGB      { 127, 255, 0 }
  #define GREENYELLOW_RGB     { 173, 255, 47 }
  #define SPRINGGREEN_RGB     { 0, 255, 127 }
  #define MEDIUMSPRINGGREEN_RGB { 0, 250, 154 }
  #define LIGHTGREEN_RGB      { 144, 238, 144 }
  #define PALEGREEN_RGB       { 152, 251, 152 }
  #define DARKSEAGREEN_RGB    { 143, 188, 143 }
  #define MEDIUMAQUAMARINE_RGB  { 102, 205, 170 }
  #define MEDIUMSEAGREEN_RGB  { 60, 179, 113 }
  #define SEAGREEN_RGB        { 46, 139, 87 }
  #define FORESTGREEN_RGB     { 34, 139, 34 }
  #define GREEN_RGB           { 0, 128, 0 }
  #define DARKGREEN_RGB       { 0, 100, 0 }

  // Cyans
  #define AQUA_RGB            { 0, 255, 255 }
  #define CYAN_RGB            { 0, 255, 255 }
  #define LIGHTCYAN_RGB       { 224, 255, 255 }
  #define PALETURQUOISE_RGB   { 175, 238, 238 }
  #define AQUAMARINE_RGB      { 127, 255, 212 }
  #define TURQUOISE_RGB       { 64, 224, 208 }
  #define MEDIUMTURQUOISE_RGB { 72, 209, 204 }
  #define DARKTURQUOISE_RGB   { 0, 206, 209 }
  #define LIGHTSEAGREEN_RGB   { 32, 178, 170 }
  #define CADETBLUE_RGB       { 95, 158, 160 }
  #define DARKCYAN_RGB        { 0, 139, 139 }
  #define TEAL_RGB            { 0, 128, 128 }

  // Blues
  #define LIGHTSTEELBLUE_RGB  { 176, 196, 222 }
  #define POWDERBLUE_RGB      { 176, 224, 230 }
  #define LIGHTBLUE_RGB       { 173, 216, 230 }
  #define SKYBLUE_RGB         { 135, 206, 235 }
  #define LIGHTSKYBLUE_RGB    { 135, 206, 250 }
  #define DEEPSKYBLUE_RGB     { 0, 191, 255 }
  #define DODGERBLUE_RGB      { 30, 144, 255 }
  #define CORNFLOWERBLUE_RGB  { 100, 149, 237 }
  #define STEELBLUE_RGB       { 70, 130, 180 }
  #define ROYALBLUE_RGB       { 65, 105, 225 }
  #define BLUE_RGB            { 0, 0, 255 }
  #define MEDIUMBLUE_RGB      { 0, 0, 205 }
  #define DARKBLUE_RGB        { 0, 0, 139 }
  #define NAVY_RGB            { 0, 0, 128 }
  #define MIDNIGHTBLUE_RGB    { 25, 25, 112 }

  // Purples
  #define LAVENDER_RGB        { 230, 230, 250 }
  #define THISTLE_RGB         { 216, 191, 216 }
  #define PLUM_RGB            { 221, 160, 221 }
  #define VIOLET_RGB          { 238, 130, 238 }
  #define ORCHID_RGB          { 218, 112, 214 }
  #define FUCHSIA_RGB         { 255, 0, 255 }
  #define MAGENTA_RGB         { 255, 0, 255 }
  #define MEDIUMORCHID_RGB    { 186, 85, 211 }
  #define MEDIUMPURPLE_RGB    { 147, 112, 219 }
  #define BLUEVIOLET_RGB      { 138, 43, 226 }
  #define DARKVIOLET_RGB      { 148, 0, 211 }
  #define DARKORCHID_RGB      { 153, 50, 204 }
  #define DARKMAGENTA_RGB     { 139, 0, 139 }
  #define PURPLE_RGB          { 128, 0, 128 }
  #define INDIGO_RGB          { 75, 0, 130 }
  #define DARKSLATEBLUE_RGB   { 72, 61, 139 }
  #define SLATEBLUE_RGB       { 106, 90, 205 }
  #define MEDIUMSLATEBLUE_RGB { 123, 104, 238 }

  // Whites
  #define WHITE_RGB           { 255, 255, 255 }
  #define SNOW_RGB            { 255, 250, 250 }
  #define HONEYDEW_RGB        { 240, 255, 240 }
  #define MINTCREAM_RGB       { 245, 255, 250 }
  #define AZURE_RGB           { 240, 255, 255 }
  #define ALICEBLUE_RGB       { 240, 248, 255 }
  #define GHOSTWHITE_RGB      { 248, 248, 255 }
  #define WHITESMOKE_RGB      { 245, 245, 245 }
  #define SEASHELL_RGB        { 255, 245, 238 }
  #define BEIGE_RGB           { 245, 245, 220 }
  #define OLDLACE_RGB         { 253, 245, 230 }
  #define FLORALWHITE_RGB     { 255, 250, 240 }
  #define IVORY_RGB           { 255, 255, 240 }
  #define ANTIQUEWHITE_RGB    { 250, 235, 215 }
  #define LINEN_RGB           { 250, 240, 230 }
  #define LAVENDERBLUSH_RGB   { 255, 240, 245 }
  #define MISTYROSE_RGB       { 255, 228, 225 }

  // Grays
  #define GAINSBORO_RGB       { 220, 220, 220 }
  #define LIGHTGRAY_RGB       { 211, 211, 211 }
  #define SILVER_RGB          { 192, 192, 192 }
  #define DARKGRAY_RGB        { 169, 169, 169 }
  #define GRAY_RGB            { 128, 128, 128 }
  #define DIMGRAY_RGB         { 105, 105, 105 }
  #define LIGHTSLATEGRAY_RGB  { 119, 136, 153 }
  #define SLATEGRAY_RGB       { 112, 128, 144 }
  #define DARKSLATEGRAY_RGB   { 47, 79, 79 }
  #define BLACK_RGB           { 0, 0, 0 }

#endif

// System Palette : This is the system palette, in that, it contains a list of RGB or RGBW colors selected from the defined color values.
#define S_WHITE             0
#define S_RED               1
#define S_YELLOW            2
#define S_OLIVE             3
#define S_LIME              4
#define S_GREEN             5
#define S_AQUA              6
#define S_TEAL              7
#define S_BLUE              8
#define S_NAVY              9
#define S_FUCHSIA           10
#define S_PURPLE            11
#define S_PINK              12
#define S_CRIMSON           13
#define S_ORANGE            14
#define S_ORANGERED         15
#define S_DARKORANGE        16
#define S_GOLD              17
#define S_MISTYROSE         18
#define S_LAVENDER          19
#define S_PLUM              20
#define S_VIOLET            21
#define S_ORCHID            22
#define S_MAGENTA           23
#define S_BLUEVIOLET        24
#define S_INDIGO            25
#define S_CYAN              26
#define S_LIMEGREEN         27
#define S_LIGHTGREEN        28
#define S_FORESTGREEN       29
#define S_HOTPINK           30
#define S_PALEVIOLETRED     31

/////////////////////////
// Global Variables
/////////////////////////

// Neopixel Strip Objects
Adafruit_NeoPixel strips[STRIPS];
// Strip Defaults
int stripDefaults[][2] = { { PIXELS, PIN } };

// Palette Array Definitions

// The System Palette - Defines all the RGB color triplets or quadruplets currently available to the app.
#define SYSTEM_PALETTE_SIZE 32
int totalSystemColors = SYSTEM_PALETTE_SIZE;
int systemPalette[SYSTEM_PALETTE_SIZE][RGB_SIZE] = { WHITE_RGB, RED_RGB, YELLOW_RGB, OLIVE_RGB, LIME_RGB, GREEN_RGB, AQUA_RGB, TEAL_RGB, BLUE_RGB, NAVY_RGB, 
                                        FUCHSIA_RGB, PURPLE_RGB, PINK_RGB, CRIMSON_RGB, ORANGE_RGB, ORANGERED_RGB, DARKORANGE_RGB, GOLD_RGB, 
                                        MISTYROSE_RGB, LAVENDER_RGB, PLUM_RGB, VIOLET_RGB, ORCHID_RGB, MAGENTA_RGB, BLUEVIOLET_RGB, INDIGO_RGB,
                                        CYAN_RGB, LIMEGREEN_RGB, LIGHTGREEN_RGB, FORESTGREEN_RGB, HOTPINK_RGB, PALEVIOLETRED_RGB };

// The Active Palette - A list of indexes into the systemPalette, used for effects processing
// Size of Active Palette
#define ACTIVE_PALETTE_SIZE 2
int totalActiveColors = ACTIVE_PALETTE_SIZE;
int activePalette[ACTIVE_PALETTE_SIZE] = { S_GREEN, S_GOLD };

// Scratch Pixel & Default Pixels
int rgbPixel[RGB_SIZE] = GREEN_RGB;
// Off Pixel
int offPixel[RGB_SIZE] = BLACK_RGB;
// White Pixel
int whitePixel[RGB_SIZE] = WHITE_RGB;
// Palette RGB
int paletteRGB[RGB_SIZE] = BLACK_RGB;
// Palette Index
int paletteIndex = 0;

// Brightness Max
int maxBrightness = MAX;

long maxColorTimeMultiplier = 100000;
// MaxColor Time (in ms)
long maxColorTime = (3 * maxColorTimeMultiplier);
// Color Timer
long timeInEffect = 0;

// *****
// Effects Definitions
// *****

// Effects State
int effectsState = EFFECTS_INITIAL_STATE;
// Current Effect in processing
int currentEffect = INITIAL_EFFECT;

// *** Pixel Pattern Buffer
// This is used to define a series of patterns for all pixels on all strips

// Pixel Pattern Buffer
#define PATTERN_COUNT 3
int pixelPatternBuffer[PATTERN_COUNT][STRIPS][PIXELS] = {
  { { S_WHITE, S_RED, S_WHITE, S_RED, S_WHITE, S_RED, S_WHITE, S_RED, S_WHITE, S_RED, S_WHITE, S_RED, S_WHITE, S_RED, S_WHITE, S_RED, S_WHITE, S_RED, S_WHITE, S_RED, S_WHITE, S_RED, S_WHITE, S_RED, S_WHITE } },
  { { S_WHITE, S_WHITE, S_WHITE, S_WHITE, S_WHITE, S_WHITE, S_WHITE, S_WHITE, S_WHITE, S_WHITE, S_WHITE, S_WHITE, S_WHITE, S_WHITE, S_WHITE, S_WHITE, S_WHITE, S_WHITE, S_WHITE, S_WHITE, S_WHITE, S_WHITE, S_WHITE, S_WHITE, S_WHITE } },
  { { S_RED, S_RED, S_RED, S_RED, S_RED, S_RED, S_RED, S_RED, S_RED, S_RED, S_RED, S_RED, S_RED, S_RED, S_RED, S_RED, S_RED, S_RED, S_RED, S_RED, S_RED, S_RED, S_RED, S_RED, S_RED } }
  };
// Variable Pattern Cound
int patternCount = PATTERN_COUNT;
// Current Pixel Pattern
int currentPattern = 0;

// *** Fade Effect Variables
// Fades a given color up, holds it for a time, then fades it down and selects a new color from the active palette

// Fade Step
int fadeStep = -1;
// Fade Delay
int fadeDelay = 100;
// Fading Flag
int fading = 0;
// Fade Color Selection (0 = Random, 1 = Modulo)
int fadeColorSelection = 1;
// Current Fade Color (index into active palette)
int currentFadeColor = 0;

// Global Mix down Color
uint32_t mixedColor;
// Current Mixed Color
int currentColor = S_GREEN;
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

// DebugMode Flag
int DebugMode = TROUBLESHOOT;

// Initialized Flag
int initialized = 0;

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

  // Set Colors
  oneColor = -1;
  currentColor = S_GREEN;

  // Init Neopixel strips
  for (int index=0; index < STRIPS; ++index)
  {
    strips[index] = Adafruit_NeoPixel(stripDefaults[index][0],stripDefaults[index][1], NEO_GRB + NEO_KHZ800);
    strips[index].begin();
    strips[index].setBrightness(maxBrightness);
    SetRGBPixels(index,offPixel,0);
  }
}

// *** I2C Recieve Loop
void i2cReceive(int howManyBytes) {
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
        if (v <= totalSystemColors && v > -1) {
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
        if (v <= totalSystemColors && v > -1)
        {
          currentColor = v;

          TurnOnStrips(currentColor);
        }
      }
      break;
      case R_SETPALREG: {
        CopyPixel(systemPalette[paletteIndex],paletteRGB);
      }
      break;
      case R_PALETTEINDEX: {
        if (v <= SYSTEM_PALETTE_SIZE && v >= 0)
          paletteIndex = v;
      }
      break;
      case R_PALETTE_R: {
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
      case R_MAXPAL: {
        if (v <= SYSTEM_PALETTE_SIZE)
        {
          totalSystemColors = v;
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
        Serial.print("One Color Flag : ");
        Serial.println(oneColor,DEC);
        Serial.print("Current Pixel  : ");
        Serial.println(currentPixel,DEC);
        Serial.print("Current Strip  : ");
        Serial.println(currentStrip,DEC);
        Serial.print("Show LEDS      : ");
        Serial.println(showLeds,DEC);
        Serial.print("Time In Effect : ");
        Serial.println(timeInEffect,DEC);
        Serial.print("maxColorTimeM  : ");
        Serial.println(maxColorTimeMultiplier,DEC);
        Serial.print("Delay          : ");
        Serial.println(Delay,DEC);
        Serial.print("RGB Size       : ");
        Serial.println(RGB_SIZE,DEC);
        Serial.print("Effects State  : ");
        Serial.println(effectsState,DEC);
        Serial.print("Current Effect : ");
        Serial.println(currentEffect,DEC);
        if (currentEffect == EFFECT_FADE) {
          Serial.print("Fade Step      : ");
          Serial.println(fadeStep,DEC);
          Serial.print("Fade Delay     : ");
          Serial.println(fadeDelay,DEC);
          Serial.print("Percent Color  : ");
          Serial.println(percentColor,DEC);
          Serial.print("Color Selection: ");
          Serial.println(fadeColorSelection,DEC);
          Serial.print("Fading         : ");
          Serial.println(fading,DEC);
          Serial.print("Fade Color Idx : ");
          Serial.println(currentFadeColor,DEC);
        } else if (currentEffect == EFFECT_DEFINED_PATTERN) {
          Serial.println("\tCurrent Effect is : DEFINED_PATTERN");
        }
        Serial.print("LED Toggle     : ");
        Serial.println(ledToggle,DEC);
        Serial.print("System Colors  : ");
        Serial.println(totalSystemColors,DEC);
        Serial.print("DebugMode      : ");
        Serial.println(DebugMode,DEC);
        Serial.print("Serial Enable  : ");
        Serial.println(SERIALENABLE,DEC);
        Serial.print("maxColorTime   : ");
        Serial.println(maxColorTime,DEC);
        Serial.print("Initialized    : ");
        Serial.println(initialized,DEC);
        
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
      case R_DEBUG: {
        DebugMode = v;
      }
      break;
      case R_EFFECTSTATE: {
        v = (v == 0 || v > 0) ? v : 0;
        effectsState = v;
      }
      break;
      case R_COLORTIME: {
        maxColorTime = abs(v * maxColorTimeMultiplier);
      }
      break;
      case R_COLORTIMEMULT: {
        maxColorTimeMultiplier = abs(v * 1000);
      }
      break;
      case R_PERCENTCOLOR: {
        percentColor = abs(v);
      }
      break;
      case R_EFFECT:{
        currentEffect = abs(v);
      }
      break;
    }
  }
}

// Serial Msg Helper
void SendMsg(String msg) {
  Serial.println(msg);
}

// Debug Message Helper
void DebugMsg(String msg) {
  if (TROUBLESHOOT > 0 && SERIALENABLE > 0)
  {
    SendMsg(msg);
  }
}

// *** Pixel Helper Functions

// Turn Off Strip Pixels
void TurnOffStrip(int stripIndex) {
  SetRGBPixels(stripIndex,offPixel,0);
}

// Turn Off All Strips
void TurnOffStrips() {
  for (int index=0; index < STRIPS; ++index)
    TurnOffStrip(index);
}

// Turn On Strip Pixels With Given Color
void TurnOnStrip(int stripIndex, int color) {
  SetPixels(stripIndex,color,0);
}

// Turn On All Strips
void TurnOnStrips(int color) {
  for (int index=0; index < STRIPS; ++index)
    TurnOnStrip(index,color);
}

// Show Strip
void ShowStrip(int strip) {
  strips[strip].show();
}

// *** Set Pixel Functions ***

// Show New Settings On Strip
void Show(int stripIndex) {
  strips[stripIndex].show();
}

// Set the Color of a Specific Pixel (w/No Adjust)
void SetPixel(int stripIndex, int pixelIndex, int color) {
  strips[stripIndex].setPixelColor(pixelIndex,MixColorVector(systemPalette[color]));
}

// Set Pixels on A Strip To An RGB/W Value (w/0 Percent Adjust)
void SetRGBPixels(int strip, int rgbColor[], int wait) {
  uint32_t c = MixColorVector(rgbColor);
  
  for (uint16_t index=0; index < strips[strip].numPixels(); ++index)
  {
    strips[strip].setPixelColor(index,c);
  }

  ShowStrip(strip);
  
  if (wait > 0)
    delay(wait);
}

// Set Pixels on A Strip (w/Percent Adjust)
void SetPixels(int strip, int color, int wait) {
  uint32_t c = AdjustColor(color,percentColor);
  
  for (uint16_t index=0; index < strips[strip].numPixels(); ++index)
  {
    strips[strip].setPixelColor(index,c);
  }

  ShowStrip(strip);
  
  if (wait > 0)
    delay(wait);
}

// Set Strip By Pattern
void SetPattern(int pattern) {
  for (int strip = 0; strip < STRIPS; ++strip)
  {
    for (int pixel = 0; pixel < PIXELS; ++pixel)
    {
      SetPixel(strip,pixel,pixelPatternBuffer[pattern][strip][pixel]);
    }
    
    ShowStrip(strip); 
  }
}

// Mixdown RGB/W Values
uint32_t MixColorVector(int pixel[RGB_SIZE]) {
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
void CopyPixel(int dst[RGB_SIZE],int src[RGB_SIZE]) {
  for(int index=0; index < RGB_SIZE; ++index)
    dst[index] = src[index];
}

// *** End Set Pixel Functions

/*
 * Effects Helpers
 */

// Toggle Step
int ToggleStep(int stepValue) {
  return (stepValue < 0 ? abs(stepValue) : (stepValue * -1));
}

// Adjust Color By Percent (0 thru 100, 0 = no color, 100 = current color)
uint32_t AdjustColor(int color, int percent) {
  float p = (float(percent) / 100.0);
  int pixelColor[RGB_SIZE];
  int tmpColor[RGB_SIZE];

  CopyPixel(pixelColor,systemPalette[color]);

  for (int index=0; index < RGB_SIZE; ++index)
  {
    pixelColor[index] = int(float(systemPalette[color][index]) * p);
  }

  return (MixColorVector(pixelColor));
}

#if RGB_SIZE > 3

// Adjust Brightness of 4 Color Pixel
void AdjustPixelBrightness(int pixel[RGB_SIZE], int brightness){
  pixel[3] = brightness;
}

// Adjust Brightness Value of 4 Color Pixel in Palette
void AdjustPixelInPaletteBrightness(int color, int brightness) {
  colors[color][3] = brightness;
}

#endif

// Get Next Effect Pattern
void NewEffectPattern() {
  // Use Modulo arithmetic to cycle through patterns
  currentPattern = ((currentPattern+1) % patternCount);
}

// Get New Fade Color from Active Palette
void NewFadeColorFromActivePalette() {
  if (oneColor < 0) {
    currentFadeColor = (fadeColorSelection) ? ((currentFadeColor + 1) % totalActiveColors) : random(0,totalActiveColors - 1);

    currentColor = activePalette[currentFadeColor];
  } else
    currentColor = oneColor;
}

// *** End of Effects Helpers

/*
 * Effect Routines
 */
 
// Fade Down, then Up - Colors selected from Active Palette, not System Palette
void Fade() {
  if (timeInEffect > maxColorTime) {
    // Flip fade up/down the color
    if (percentColor <= 0) {
      fadeStep = 1;
      percentColor = 0;
      NewFadeColorFromActivePalette();
      fading = 2;
    } else if (percentColor >= 100) {
      fadeStep = -1;
      percentColor = 100;
      fading = 1;
    }
      
    percentColor += fadeStep;
  
    for (int index=0; index < STRIPS; ++index)
      SetPixels(index,currentColor,0);
  
    delay(fadeDelay);

    if (fading == 2 && percentColor >= 100) {
      fading = 0;
      timeInEffect = 0;
    }
  } else {
    delay(Delay);

    timeInEffect += Delay;
  }
}

// Defined Pattern Effect
void DefinedPatternEffect() {
    if (timeInEffect > maxColorTime)
    {
      NewEffectPattern();

      SetPattern(currentPattern);

      timeInEffect = 0;
    } else {
      delay(Delay);

      timeInEffect += Delay;
    }
}

/*
 * General Helper Functions
 */

// Toggle Onboard LED (if enabled, useful for debugging purposes)
void ToggleOnBoardLed() {
  if (DebugMode) {
    digitalWrite(LED_BUILTIN,ledToggle);
    ledToggle = !ledToggle;

    if (showLeds == 0)
      delay(DEBUGDELAY);
  }
}

/////////////////////////////
// Main Loop
/////////////////////////////
// the loop function runs over and over again forever
void loop() {
  ToggleOnBoardLed();

  if (showLeds) {
    if (!initialized) {
      initialized = 1;
      TurnOnStrips(currentColor);
    }

    // Effects Processing
    if (effectsState) {
      switch (currentEffect) {
      case EFFECT_FADE:
      {
          Fade();
      }
      break;
      case EFFECT_DEFINED_PATTERN:
      {
        DefinedPatternEffect();  
      }
      break;
      }
    }
  }
}
