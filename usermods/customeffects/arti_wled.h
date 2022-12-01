/*
   @title   Arduino Real Time Interpreter (ARTI)
   @file    arti_wled_plugin.h
   @date    20220818
   @author  Ewoud Wijma
   @repo    https://github.com/ewoudwijma/ARTI
 */

#pragma once

// For testing porposes, definitions should not only run on Arduino but also on Windows etc. 
// Because compiling on arduino takes seriously more time than on Windows.
// The plugin.h files replace native arduino calls by windows simulated calls (e.g. setPixelColor will become printf)

#define ARTI_ARDUINO 1 
#define ARTI_EMBEDDED 2
#ifdef ESP32 //ESP32 is set in wled context: small trick to set WLED context
  #define ARTI_PLATFORM ARTI_ARDUINO // else on Windows/Linux/Mac...
#endif

#if ARTI_PLATFORM == ARTI_ARDUINO
  #include "arti.h"
  extern float sampleAvg;
  extern float sampleAgc;
  extern byte soundAgc;
#else
  #include "../arti.h"
  #include <string.h>
  #include <stdlib.h>
  #include <stdio.h>
#endif

//make sure the numbers here correspond to the order in which these functions are defined in wled000.json!!
enum Externals
{
  F_ledCount,
  F_width,
  F_height,
  F_setPixelColor,
  F_leds,
  F_hsv,
  F_rgbw,

  F_setRange,
  F_fill,
  F_colorBlend,
  F_colorWheel,
  F_colorFromPalette,
  F_beatSin,
  F_fadeToBlackBy,
  F_iNoise,
  F_fadeOut,

  F_counter,
  F_segcolor,
  F_speedSlider,
  F_intensitySlider,
  F_custom1Slider,
  F_custom2Slider,
  F_custom3Slider,
  F_sampleAvg,

  F_shift,
  F_circle2D,
  F_drawLine,
  F_drawArc,

  F_constrain,
  F_map,
  F_seed,
  F_random,
  F_sin,
  F_cos,
  F_abs,
  F_min,
  F_max,
  F_floor,

  F_hour,
  F_minute,
  F_second,
  F_millis,

  F_time,
  F_triangle,
  F_wave,
  F_square,
  F_clamp,

  F_printf
};

#if ARTI_PLATFORM != ARTI_ARDUINO
  #define PI 3.141592654
#endif

float ARTI::arti_external_function(uint8_t function, float par1, float par2, float par3, float par4, float par5)
{
  // MEMORY_ARTI("fun %d(%f, %f, %f)\n", function, par1, par2, par3);
  #if ARTI_PLATFORM == ARTI_ARDUINO
    switch (function) {
      case F_setPixelColor: {
        if (par3 == floatNull)
          strip.setPixelColor(((uint16_t)par1)%SEGLEN, (uint32_t)par2);
        else
          strip.setPixelColor(strip.XY((uint16_t)par1, (uint16_t)par2), (uint32_t)par3);
        return floatNull;
      }
      case F_hsv:
      {
        CRGB color = CHSV((uint8_t)par1, (uint8_t)par2, (uint8_t)par3);
        return RGBW32(color.r, color.g, color.b, 0);
      }
      case F_rgbw:
        return RGBW32((uint8_t)par1, (uint8_t)par2, (uint8_t)par3, (uint8_t)par4);

      case F_setRange: {
        strip.setRange((uint16_t)par1, (uint16_t)par2, (uint32_t)par3);
        return floatNull;
      }
      case F_fill: {
        strip.fill((uint32_t)par1);
        return floatNull;
      }
      case F_colorBlend:
        return strip.color_blend((uint32_t)par1, (uint32_t)par2, (uint16_t)par3);
      case F_colorWheel:
        return strip.color_wheel((uint8_t)par1);
      case F_colorFromPalette: 
      {
        CRGB color;
        if (par2 == floatNull)
          color = ColorFromPalette(strip.currentPalette, (uint8_t)par1);
        else
          color = ColorFromPalette(strip.currentPalette, (uint8_t)par1, (uint8_t)par2); //brightness
        return RGBW32(color.r, color.g, color.b, 0);
      }
      case F_beatSin:
        return beatsin8((uint8_t)par1, (uint8_t)par2, (uint8_t)par3, (uint8_t)par4, (uint8_t)par5);
      case F_fadeToBlackBy:
        strip.fade2black((uint8_t)par1);
        return floatNull;
      case F_iNoise:
        return inoise16((uint32_t)par1, (uint32_t)par2);
      case F_fadeOut:
        strip.fade_out((uint8_t)par1);
        return floatNull;

      case F_segcolor:
        return SEGCOLOR((uint8_t)par1);

      case F_shift: {
        uint32_t saveFirstPixel = strip.getPixelColor(0);
        for (uint16_t i=0; i<SEGLEN-1; i++)
        {
          strip.setPixelColor(i, strip.getPixelColor((uint16_t)(i + par1)%SEGLEN));
        }
        strip.setPixelColor(SEGLEN - 1, saveFirstPixel);
        return floatNull;
      }
      case F_circle2D: {
        uint16_t circleLength = min(strip.matrixWidth, strip.matrixHeight);
        uint16_t deltaWidth=0, deltaHeight=0;

        if (circleLength < strip.matrixHeight) //portrait
          deltaHeight = (strip.matrixHeight - circleLength) / 2;
        if (circleLength < strip.matrixWidth) //portrait
          deltaWidth = (strip.matrixWidth - circleLength) / 2;

        float halfLength = (circleLength-1)/2.0;

        //calculate circle positions, round to 5 digits and then round again to cater for radians inprecision (e.g. 3.49->3.5->4)
        int x = round(round((sin(radians(par1)) * halfLength + halfLength) * 10)/10) + deltaWidth;
        int y = round(round((halfLength - cos(radians(par1)) * halfLength) * 10)/10) + deltaHeight;
        return strip.XY(x,y);
      }
      case F_drawLine:
        strip.drawLine(par1, par2, par3, par4, par5);
        return floatNull;
      case F_drawArc:
        if (par5 == floatNull)
          strip.drawArc(par1, par2, par3, par4);
        else
          strip.drawArc(par1, par2, par3, par4, par5); //fillColor
        return floatNull;
      case F_constrain:
        return constrain(par1, par2, par3);
      case F_map:
        return map(par1, par2, par3, par4, par5);
      case F_seed:
        random16_set_seed((uint16_t)par1);
        return floatNull;
      case F_random:
        return random16();

      case F_millis:
        return millis();

      default: {}
    }
  #else // not arduino
    switch (function)
    {
      case F_setPixelColor:
        PRINT_ARTI("%s(%f, %f, %f)\n", "setPixelColor", par1, par2, par3);
        return floatNull;
      case F_hsv:
        PRINT_ARTI("%s(%f, %f, %f)\n", "hsv", par1, par2, par3);
        return par1 + par2 + par3;
      case F_rgbw:
        PRINT_ARTI("%s(%f, %f, %f, %f)\n", "rgbw", par1, par2, par3, par4);
        return par1 + par2 + par3 + par4;

      case F_setRange:
        return par1 + par2 + par3;
      case F_fill:
        PRINT_ARTI("%s(%f)\n", "fill", par1);
        return floatNull;
      case F_colorBlend:
        return par1 + par2 + par3;
      case F_colorWheel:
        return par1;
      case F_colorFromPalette:
        return par1 + par2;
      case F_beatSin:
        return par1+par2+par3+par4+par5;
      case F_fadeToBlackBy:
        return par1;
      case F_iNoise:
        return par1 + par2;
      case F_fadeOut:
        return par1;

      case F_segcolor:
        return par1;

      case F_shift:
        PRINT_ARTI("%s(%f)\n", "shift", par1);
        return floatNull;
      case F_circle2D:
        PRINT_ARTI("%s(%f)\n", "circle2D", par1);
        return par1 / 2;
      case F_drawLine:
        return par1 + par2 + par3 + par4 + par5;
      case F_drawArc:
        return par1 + par2 + par3 + par4 + par5;

      case F_constrain:
        return par1 + par2 + par3;
      case F_map:
        return par1 + par2 + par3 + par4 + par5;
      case F_seed:
        PRINT_ARTI("%s(%f)\n", "seed", par1);
        return floatNull;
      case F_random:
        return rand();

      case F_millis:
        return 1000;
    }
  #endif

  //same on Arduino or Windows
  switch (function)
  {
    case F_sin:
      return sin(par1);
    case F_cos:
      return cos(par1);
    case F_abs:
      return fabs(par1);
    case F_min:
      return fmin(par1, par2);
    case F_max:
      return fmax(par1, par2);
    case F_floor:
      return floorf(par1);

    // Reference: https://github.com/atuline/PixelBlaze
    case F_time: // A sawtooth waveform between 0.0 and 1.0 that loops about every 65.536*interval seconds. e.g. use .015 for an approximately 1 second.
    {
      float myVal = millis();
      myVal = myVal / 65535 / par1;           // PixelBlaze uses 1000/65535 = .015259. 
      myVal = fmod(myVal, 1.0);               // ewowi: with 0.015 as input, you get fmod(millis/1000,1.0), which has a period of 1 second, sounds right
      return myVal;
    }
    case F_triangle: // Converts a sawtooth waveform v between 0.0 and 1.0 to a triangle waveform between 0.0 to 1.0. v "wraps" between 0.0 and 1.0.
      return 1.0 - fabs(fmod(2 * par1, 2.0) - 1.0);
    case F_wave: // Converts a sawtooth waveform v between 0.0 and 1.0 to a sinusoidal waveform between 0.0 to 1.0. Same as (1+sin(v*PI2))/2 but faster. v "wraps" between 0.0 and 1.0.
      return (1 + sin(par1 * 2 * PI)) / 2;
    case F_square: // Converts a sawtooth waveform v to a square wave using the provided duty cycle where duty is a number between 0.0 and 1.0. v "wraps" between 0.0 and 1.0.
    {
      float sinValue = arti_external_function(F_wave, par1);
      return sinValue >= par2 ? 1 : 0;
    }
    case F_clamp:
    {
      const float t = par1 < par2 ? par2 : par1;
      return t > par3 ? par3 : t;
    }

    case F_printf: {
      if (par3 == floatNull) {
        if (par2 == floatNull) {
          PRINT_ARTI("%f\n", par1);
        }
        else
          PRINT_ARTI("%f, %f\n", par1, par2);
      }
      else
        PRINT_ARTI("%f, %f, %f\n", par1, par2, par3);
      return floatNull;
    }
  }

  ERROR_ARTI("Error: arti_external_function: %u not implemented\n", function);
  errorOccurred = true;
  return function;
}

float ARTI::arti_get_external_variable(uint8_t variable, float par1, float par2, float par3)
{
  // MEMORY_ARTI("get %d(%f, %f, %f)\n", variable, par1, par2, par3);
  #if ARTI_PLATFORM == ARTI_ARDUINO
    switch (variable)
    {
      case F_ledCount:
        return SEGLEN;
      case F_width:
        return SEGMENT.width;
      case F_height:
        return SEGMENT.height;
      case F_leds:
        if (par1 == floatNull) {
          ERROR_ARTI("arti_get_external_variable leds without indices not supported yet (get leds)\n");
          errorOccurred = true;
          return floatNull;
        }
        else if (par2 == floatNull)
          return strip.getPixelColor((uint16_t)par1);
        else
          return strip.getPixelColor(strip.XY((uint16_t)par1, (uint16_t)par2)); //2D value!!

      case F_counter:
        return SEGENV.call;
      case F_speedSlider:
        return SEGMENT.speed;
      case F_intensitySlider:
        return SEGMENT.intensity;
      case F_custom1Slider:
        return SEGMENT.custom1;
      case F_custom2Slider:
        return SEGMENT.custom2;
      case F_custom3Slider:
        return SEGMENT.custom3;
      case F_sampleAvg:
        return((soundAgc) ? sampleAgc : sampleAvg);
      case F_hour:
        return ((float)hour(localTime));
      case F_minute:
        return ((float)minute(localTime));
      case F_second:
        return ((float)second(localTime));
    }
  #else
    switch (variable)
    {
      case F_ledCount:
        return 3; // used in testing e.g. for i = 1 to ledCount
      case F_width:
        return 2;
      case F_height:
        return 4;
      case F_leds:
        if (par1 == floatNull) {
          ERROR_ARTI("arti_get_external_variable leds without indices not supported yet (get leds)\n");
          errorOccurred = true;
          return F_leds;
        }
        else if (par2 == floatNull)
          return par1;
        else
          return par1 * par2; //2D value!!

      case F_counter:
        return frameCounter;
      case F_speedSlider:
        return F_speedSlider;
      case F_intensitySlider:
        return F_intensitySlider;
      case F_custom1Slider:
        return F_custom1Slider;
      case F_custom2Slider:
        return F_custom2Slider;
      case F_custom3Slider:
        return F_custom3Slider;
      case F_sampleAvg:
        return F_sampleAvg;

      case F_hour:
        return F_hour;
      case F_minute:
        return F_minute;
      case F_second:
        return F_second;
    }
  #endif

  ERROR_ARTI("Error: arti_get_external_variable: %u not implemented\n", variable);
  errorOccurred = true;
  return variable;
}

void ARTI::arti_set_external_variable(float value, uint8_t variable, float par1, float par2, float par3)
{
  #if ARTI_PLATFORM == ARTI_ARDUINO
    // MEMORY_ARTI("%s %s %u %u (%u)\n", spaces+50-depth, variable_name, par1, par2, esp_get_free_heap_size());
    switch (variable)
    {
      case F_leds:
        if (par1 == floatNull) 
        {
          ERROR_ARTI("arti_set_external_variable leds without indices not supported yet (set leds to %f)\n", value);
          errorOccurred = true;
        }
        else if (par2 == floatNull)
          strip.setPixelColor((uint16_t)par1%SEGLEN, value);
        else
          strip.setPixelColor(strip.XY((uint16_t)par1%SEGMENT.width, (uint16_t)par2%SEGMENT.height), value); //2D value!!

        return;
    }
  #else
    switch (variable)
    {
      case F_leds:
        if (par1 == floatNull) 
        {
          ERROR_ARTI("arti_set_external_variable leds without indices not supported yet (set leds to %f)\n", value);
          errorOccurred = true;
        }
        else if (par2 == floatNull)
          RUNLOG_ARTI("arti_set_external_variable: leds(%f) := %f\n", par1, value);
        else
          RUNLOG_ARTI("arti_set_external_variable: leds(%f, %f) := %f\n", par1, par2, value);

        return;
    }
  #endif

  ERROR_ARTI("Error: arti_set_external_variable: %u not implemented\n", variable);
  errorOccurred = true;
}

bool ARTI::loop() 
{
  if (stages < 5) {close(); return true;}

  if (parseTreeJsonDoc == nullptr || parseTreeJsonDoc->isNull()) 
  {
    ERROR_ARTI("Loop: No parsetree created\n");
    errorOccurred = true;
    return false;
  }
  else 
  {
    uint8_t depth = 8;

    bool foundRenderFunction = false;
    
    const char * function_name = "renderFrame";
    Symbol* function_symbol = global_scope->lookup(function_name);

    if (function_symbol != nullptr) { //calling undefined function: pre-defined functions e.g. print

      foundRenderFunction = true;

      ActivationRecord* ar = new ActivationRecord(function_name, "Function", function_symbol->scope_level + 1);

      RUNLOG_ARTI("%s %s %s (%u)\n", spaces+50-depth, "Call", function_name, this->callStack->recordsCounter);

      this->callStack->push(ar);

      if (!interpret(function_symbol->block, nullptr, global_scope, depth + 1))
        return false;

      this->callStack->pop();

      delete ar; ar = nullptr;

    } //function_symbol != nullptr

    function_name = "renderLed";
    function_symbol = global_scope->lookup(function_name);

    if (function_symbol != nullptr) { //calling undefined function: pre-defined functions e.g. print

      foundRenderFunction = true;

      ActivationRecord* ar = new ActivationRecord(function_name, "function", function_symbol->scope_level + 1);

      for (int i = 0; i< arti_get_external_variable(F_ledCount); i++)
      {
        if (function_symbol->function_scope->nrOfFormals == 2) {// 2D
          ar->set(function_symbol->function_scope->symbols[0]->scope_index, i%strip.matrixWidth); // set x
          ar->set(function_symbol->function_scope->symbols[1]->scope_index, i/strip.matrixWidth); // set y
        }
        else
          ar->set(function_symbol->function_scope->symbols[0]->scope_index, i); // set x

        this->callStack->push(ar);

        if (!interpret(function_symbol->block, nullptr, global_scope, depth + 1))
          return false;

        this->callStack->pop();
      }

      delete ar; ar = nullptr;

    }

    if (!foundRenderFunction) 
    {
      ERROR_ARTI("%s renderFrame or renderLed not found\n", spaces+50-depth);
      errorOccurred = true;
      return false;
    }
  }
  frameCounter++;

  if (frameCounter == 1)
    startMillis = millis();

  if (millis() - startMillis > 3000) //startMillis != 0 && logToFile && 
  {
    // ERROR_ARTI("time %u\n", millis() - startMillis);
    closeLog();
    // startMillis = 0;
  }

  return true;
} // loop

//declare global variables
ARTI * arti;

//effect function
uint16_t WS2812FX::mode_customEffect(void) { 
  //tbd: move statics to SEGMENT.data
  static bool succesful;
  static bool notEnoughHeap;

  static char previousEffect[charLength];
  if (SEGENV.call == 0) {
    strcpy(previousEffect, ""); //force init
    fill(BLACK); //in case not all leds used e.g. when using expand 1d Circles. Tbd: fill black should never be used to allow for blends/transitions
  }

  char currentEffect[charLength];
  strcpy(currentEffect, (SEGMENT.name != nullptr)?SEGMENT.name:"default"); //note: switching preset with segment name to preset without does not clear the SEGMENT.name variable, but not gonna solve here ;-)

  if (strcmp(previousEffect, currentEffect) != 0) 
  {
    strcpy(previousEffect, currentEffect);

    // if (artiWrapper != nullptr && artiWrapper->arti != nullptr) {
    if (arti != nullptr) 
    {
      arti->close();
      delete arti; arti = nullptr;
    }

    // if (!SEGENV.allocateData(sizeof(ArtiWrapper))) return mode_static();  // We use this method for allocating memory for static variables.
    // artiWrapper = reinterpret_cast<ArtiWrapper*>(SEGENV.data);
    arti = new ARTI();

    char programFileName[fileNameLength];
    strcpy(programFileName, "/");
    strcat(programFileName, currentEffect);
    strcat(programFileName, ".wled");

    succesful = arti->setup("/wledv032.json", programFileName);

    if (!succesful)
      ERROR_ARTI("Setup not succesful\n");
  }
  else 
  {
    if (succesful) // && SEGENV.call < 250 for each frame
    {
      if (esp_get_free_heap_size() <= 20000) 
      {
        ERROR_ARTI("Not enough free heap (%u <= 30000)\n", esp_get_free_heap_size());
        notEnoughHeap = true;
        succesful = false;
      }
      else
      {
        // static int previousMillis;
        // static int previousCall;
        // if (millis() - previousMillis > 5000) { //tried SEGENV.aux0 but that looks to be overwritten!!! (dangling pointer???)
        //   previousMillis = millis();
        //   MEMORY_ARTI("Heap renderFrame %u %u fps\n", esp_get_free_heap_size(), (SEGENV.call - previousCall)/5);
        //   previousCall = SEGENV.call;
        // }
        
        succesful = arti->loop();
      }
    }
    else 
    {
      arti->closeLog();
      if (notEnoughHeap && esp_get_free_heap_size() > 20000) {
        ERROR_ARTI("Again enough free heap, restart effect (%u > 30000)\n", esp_get_free_heap_size());
        succesful = true;
        notEnoughHeap = false;
        strcpy(previousEffect, ""); // force new create
      }
      else {
        //mode_static
        strip.fill(SEGCOLOR(0));
        return 350;
      }
    }
  }

  return FRAMETIME;
}