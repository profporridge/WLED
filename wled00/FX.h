/*
  WS2812FX.h - Library for WS2812 LED effects.
  Harm Aldick - 2016
  www.aldick.org
  LICENSE
  The MIT License (MIT)
  Copyright (c) 2016  Harm Aldick
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

  Modified for WLED
*/

#ifndef WS2812FX_h
#define WS2812FX_h

#include "NpbWrapper.h"

#define FASTLED_INTERNAL //remove annoying pragma messages
#include "FastLED.h"

#define DEFAULT_BRIGHTNESS (uint8_t)127
#define DEFAULT_MODE       (uint8_t)0
#define DEFAULT_SPEED      (uint8_t)128
#define DEFAULT_COLOR      (uint32_t)0xFFAA00

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

/* Not used in all effects yet */
#define WLED_FPS         42
#define FRAMETIME        (1000/WLED_FPS)

/* each segment uses 37 bytes of SRAM memory, so if you're application fails because of
  insufficient memory, decreasing MAX_NUM_SEGMENTS may help */
#define MAX_NUM_SEGMENTS 10

#define NUM_COLORS       3 /* number of colors per segment */
#define SEGMENT          _segments[_segment_index]
#define SEGCOLOR(x)      gamma32(_segments[_segment_index].colors[x])
#define SEGENV           _segment_runtimes[_segment_index]
#define SEGLEN           SEGMENT.length()
#define SEGACT           SEGMENT.stop
#define SPEED_FORMULA_L  5 + (50*(255 - SEGMENT.speed))/SEGLEN
#define RESET_RUNTIME    memset(_segment_runtimes, 0, sizeof(_segment_runtimes))

// some common colors
#define RED        (uint32_t)0xFF0000
#define GREEN      (uint32_t)0x00FF00
#define BLUE       (uint32_t)0x0000FF
#define WHITE      (uint32_t)0xFFFFFF
#define BLACK      (uint32_t)0x000000
#define YELLOW     (uint32_t)0xFFFF00
#define CYAN       (uint32_t)0x00FFFF
#define MAGENTA    (uint32_t)0xFF00FF
#define PURPLE     (uint32_t)0x400080
#define ORANGE     (uint32_t)0xFF3000
#define PINK       (uint32_t)0xFF1493
#define ULTRAWHITE (uint32_t)0xFFFFFFFF

// options
// bit    7: segment is in transition mode
// bits 2-6: TBD
// bit    1: reverse segment
// bit    0: segment is selected
#define NO_OPTIONS   (uint8_t)0x00
#define TRANSITIONAL (uint8_t)0x80
#define REVERSE      (uint8_t)0x02
#define SELECTED     (uint8_t)0x01
#define IS_TRANSITIONAL ((SEGMENT.options & TRANSITIONAL) == TRANSITIONAL)
#define IS_REVERSE      ((SEGMENT.options & REVERSE )     == REVERSE     )
#define IS_SELECTED     ((SEGMENT.options & SELECTED)     == SELECTED    )

#define MODE_COUNT  89

#define FX_MODE_STATIC                   0
#define FX_MODE_BLINK                    1
#define FX_MODE_BREATH                   2
#define FX_MODE_COLOR_WIPE               3
#define FX_MODE_COLOR_WIPE_RANDOM        4
#define FX_MODE_RANDOM_COLOR             5
#define FX_MODE_COLOR_SWEEP              6
#define FX_MODE_DYNAMIC                  7
#define FX_MODE_RAINBOW                  8
#define FX_MODE_RAINBOW_CYCLE            9
#define FX_MODE_SCAN                    10
#define FX_MODE_DUAL_SCAN               11
#define FX_MODE_FADE                    12
#define FX_MODE_THEATER_CHASE           13
#define FX_MODE_THEATER_CHASE_RAINBOW   14
#define FX_MODE_RUNNING_LIGHTS          15
#define FX_MODE_SAW                     16
#define FX_MODE_TWINKLE                 17
#define FX_MODE_DISSOLVE                18
#define FX_MODE_DISSOLVE_RANDOM         19
#define FX_MODE_SPARKLE                 20
#define FX_MODE_FLASH_SPARKLE           21
#define FX_MODE_HYPER_SPARKLE           22
#define FX_MODE_STROBE                  23
#define FX_MODE_STROBE_RAINBOW          24
#define FX_MODE_MULTI_STROBE            25
#define FX_MODE_BLINK_RAINBOW           26
#define FX_MODE_ANDROID                 27
#define FX_MODE_CHASE_COLOR             28
#define FX_MODE_CHASE_RANDOM            29
#define FX_MODE_CHASE_RAINBOW           30
#define FX_MODE_CHASE_FLASH             31
#define FX_MODE_CHASE_FLASH_RANDOM      32
#define FX_MODE_CHASE_RAINBOW_WHITE     33
#define FX_MODE_COLORFUL                34
#define FX_MODE_TRAFFIC_LIGHT           35
#define FX_MODE_COLOR_SWEEP_RANDOM      36
#define FX_MODE_RUNNING_COLOR           37
#define FX_MODE_RUNNING_RED_BLUE        38
#define FX_MODE_RUNNING_RANDOM          39
#define FX_MODE_LARSON_SCANNER          40
#define FX_MODE_COMET                   41
#define FX_MODE_FIREWORKS               42
#define FX_MODE_RAIN                    43
#define FX_MODE_MERRY_CHRISTMAS         44
#define FX_MODE_FIRE_FLICKER            45
#define FX_MODE_GRADIENT                46
#define FX_MODE_LOADING                 47
#define FX_MODE_POLICE                  48
#define FX_MODE_POLICE_ALL              49
#define FX_MODE_TWO_DOTS                50
#define FX_MODE_TWO_AREAS               51
#define FX_MODE_CIRCUS_COMBUSTUS        52
#define FX_MODE_HALLOWEEN               53
#define FX_MODE_TRICOLOR_CHASE          54
#define FX_MODE_TRICOLOR_WIPE           55
#define FX_MODE_TRICOLOR_FADE           56
#define FX_MODE_LIGHTNING               57
#define FX_MODE_ICU                     58
#define FX_MODE_MULTI_COMET             59
#define FX_MODE_DUAL_LARSON_SCANNER     60
#define FX_MODE_RANDOM_CHASE            61
#define FX_MODE_OSCILLATE               62
#define FX_MODE_PRIDE_2015              63
#define FX_MODE_JUGGLE                  64
#define FX_MODE_PALETTE                 65
#define FX_MODE_FIRE_2012               66
#define FX_MODE_COLORWAVES              67
#define FX_MODE_BPM                     68
#define FX_MODE_FILLNOISE8              69
#define FX_MODE_NOISE16_1               70
#define FX_MODE_NOISE16_2               71
#define FX_MODE_NOISE16_3               72
#define FX_MODE_NOISE16_4               73
#define FX_MODE_COLORTWINKLE            74
#define FX_MODE_LAKE                    75
#define FX_MODE_METEOR                  76
#define FX_MODE_METEOR_SMOOTH           77
#define FX_MODE_RAILWAY                 78
#define FX_MODE_RIPPLE                  79
#define FX_MODE_TWINKLEFOX              80
#define FX_MODE_TWINKLECAT              81
#define FX_MODE_HALLOWEEN_EYES          82
#define FX_MODE_STATIC_PATTERN          83
#define FX_MODE_TRI_STATIC_PATTERN      84
#define FX_MODE_SPOTS                   85
#define FX_MODE_SPOTS_FADE              86
#define FX_MODE_GLITTER                 87
#define FX_MODE_CANDLE                  88


class WS2812FX {
  typedef uint16_t (WS2812FX::*mode_ptr)(void);
  
  // segment parameters
  public:
    typedef struct Segment { // 24 bytes
      uint16_t start;
      uint16_t stop; //segment invalid if stop == 0
      uint8_t speed;
      uint8_t intensity;
      uint8_t palette;
      uint8_t mode;
      uint8_t options; //bit pattern: msb first: transitional tbd tbd tbd tbd paused reverse selected
      uint8_t group, spacing;
      uint8_t opacity;
      uint32_t colors[NUM_COLORS];
      void setOption(uint8_t n, bool val)
      {
        if (val) {
          options |= 0x01 << n;
        } else
        {
          options &= ~(0x01 << n);
        }
      }
      bool getOption(uint8_t n)
      {
        return ((options >> n) & 0x01);
      }
      bool isSelected()
      {
        return getOption(0);
      }
      bool isActive()
      {
        return stop > start;
      }
      uint16_t length()
      {
        return stop - start;
      }
    } segment;

  // segment runtime parameters
    typedef struct Segment_runtime { // 16 bytes
      unsigned long next_time;
      uint32_t step;
      uint32_t call;
      uint16_t aux0;
      uint16_t aux1;
      void reset(){next_time = 0; step = 0; call = 0; aux0 = 0; aux1 = 0;};
    } segment_runtime;

    WS2812FX() {
      //assign each member of the _mode[] array to its respective function reference 
      _mode[FX_MODE_STATIC]                  = &WS2812FX::mode_static;
      _mode[FX_MODE_BLINK]                   = &WS2812FX::mode_blink;
      _mode[FX_MODE_COLOR_WIPE]              = &WS2812FX::mode_color_wipe;
      _mode[FX_MODE_COLOR_WIPE_RANDOM]       = &WS2812FX::mode_color_wipe_random;
      _mode[FX_MODE_RANDOM_COLOR]            = &WS2812FX::mode_random_color;
      _mode[FX_MODE_COLOR_SWEEP]             = &WS2812FX::mode_color_sweep;
      _mode[FX_MODE_DYNAMIC]                 = &WS2812FX::mode_dynamic;
      _mode[FX_MODE_RAINBOW]                 = &WS2812FX::mode_rainbow;
      _mode[FX_MODE_RAINBOW_CYCLE]           = &WS2812FX::mode_rainbow_cycle;
      _mode[FX_MODE_SCAN]                    = &WS2812FX::mode_scan;
      _mode[FX_MODE_DUAL_SCAN]               = &WS2812FX::mode_dual_scan;
      _mode[FX_MODE_FADE]                    = &WS2812FX::mode_fade;
      _mode[FX_MODE_THEATER_CHASE]           = &WS2812FX::mode_theater_chase;
      _mode[FX_MODE_THEATER_CHASE_RAINBOW]   = &WS2812FX::mode_theater_chase_rainbow;
      _mode[FX_MODE_SAW]                     = &WS2812FX::mode_saw;
      _mode[FX_MODE_TWINKLE]                 = &WS2812FX::mode_twinkle;
      _mode[FX_MODE_DISSOLVE]                = &WS2812FX::mode_dissolve;
      _mode[FX_MODE_DISSOLVE_RANDOM]         = &WS2812FX::mode_dissolve_random;
      _mode[FX_MODE_SPARKLE]                 = &WS2812FX::mode_sparkle;
      _mode[FX_MODE_FLASH_SPARKLE]           = &WS2812FX::mode_flash_sparkle;
      _mode[FX_MODE_HYPER_SPARKLE]           = &WS2812FX::mode_hyper_sparkle;
      _mode[FX_MODE_STROBE]                  = &WS2812FX::mode_strobe;
      _mode[FX_MODE_STROBE_RAINBOW]          = &WS2812FX::mode_strobe_rainbow;
      _mode[FX_MODE_MULTI_STROBE]            = &WS2812FX::mode_multi_strobe;
      _mode[FX_MODE_BLINK_RAINBOW]           = &WS2812FX::mode_blink_rainbow;
      _mode[FX_MODE_ANDROID]                 = &WS2812FX::mode_android;
      _mode[FX_MODE_CHASE_COLOR]             = &WS2812FX::mode_chase_color;
      _mode[FX_MODE_CHASE_RANDOM]            = &WS2812FX::mode_chase_random;
      _mode[FX_MODE_CHASE_RAINBOW]           = &WS2812FX::mode_chase_rainbow;
      _mode[FX_MODE_CHASE_FLASH]             = &WS2812FX::mode_chase_flash;
      _mode[FX_MODE_CHASE_FLASH_RANDOM]      = &WS2812FX::mode_chase_flash_random;
      _mode[FX_MODE_CHASE_RAINBOW_WHITE]     = &WS2812FX::mode_chase_rainbow_white;
      _mode[FX_MODE_COLORFUL]                = &WS2812FX::mode_colorful;
      _mode[FX_MODE_TRAFFIC_LIGHT]           = &WS2812FX::mode_traffic_light;
      _mode[FX_MODE_COLOR_SWEEP_RANDOM]      = &WS2812FX::mode_color_sweep_random;
      _mode[FX_MODE_RUNNING_COLOR]           = &WS2812FX::mode_running_color;
      _mode[FX_MODE_RUNNING_RED_BLUE]        = &WS2812FX::mode_running_red_blue;
      _mode[FX_MODE_RUNNING_RANDOM]          = &WS2812FX::mode_running_random;
      _mode[FX_MODE_LARSON_SCANNER]          = &WS2812FX::mode_larson_scanner;
      _mode[FX_MODE_COMET]                   = &WS2812FX::mode_comet;
      _mode[FX_MODE_FIREWORKS]               = &WS2812FX::mode_fireworks;
      _mode[FX_MODE_RAIN]                    = &WS2812FX::mode_rain;
      _mode[FX_MODE_MERRY_CHRISTMAS]         = &WS2812FX::mode_merry_christmas;
      _mode[FX_MODE_FIRE_FLICKER]            = &WS2812FX::mode_fire_flicker;
      _mode[FX_MODE_GRADIENT]                = &WS2812FX::mode_gradient;
      _mode[FX_MODE_LOADING]                 = &WS2812FX::mode_loading;
      _mode[FX_MODE_POLICE]                  = &WS2812FX::mode_police;
      _mode[FX_MODE_POLICE_ALL]              = &WS2812FX::mode_police_all;
      _mode[FX_MODE_TWO_DOTS]                = &WS2812FX::mode_two_dots;
      _mode[FX_MODE_TWO_AREAS]               = &WS2812FX::mode_two_areas;
      _mode[FX_MODE_CIRCUS_COMBUSTUS]        = &WS2812FX::mode_circus_combustus;
      _mode[FX_MODE_HALLOWEEN]               = &WS2812FX::mode_halloween;
      _mode[FX_MODE_TRICOLOR_CHASE]          = &WS2812FX::mode_tricolor_chase;
      _mode[FX_MODE_TRICOLOR_WIPE]           = &WS2812FX::mode_tricolor_wipe;
      _mode[FX_MODE_TRICOLOR_FADE]           = &WS2812FX::mode_tricolor_fade;
      _mode[FX_MODE_BREATH]                  = &WS2812FX::mode_breath;
      _mode[FX_MODE_RUNNING_LIGHTS]          = &WS2812FX::mode_running_lights;
      _mode[FX_MODE_LIGHTNING]               = &WS2812FX::mode_lightning;
      _mode[FX_MODE_ICU]                     = &WS2812FX::mode_icu;
      _mode[FX_MODE_MULTI_COMET]             = &WS2812FX::mode_multi_comet;
      _mode[FX_MODE_DUAL_LARSON_SCANNER]     = &WS2812FX::mode_dual_larson_scanner;
      _mode[FX_MODE_RANDOM_CHASE]            = &WS2812FX::mode_random_chase;
      _mode[FX_MODE_OSCILLATE]               = &WS2812FX::mode_oscillate;
      _mode[FX_MODE_FIRE_2012]               = &WS2812FX::mode_fire_2012;
      _mode[FX_MODE_PRIDE_2015]              = &WS2812FX::mode_pride_2015;
      _mode[FX_MODE_BPM]                     = &WS2812FX::mode_bpm;
      _mode[FX_MODE_JUGGLE]                  = &WS2812FX::mode_juggle;
      _mode[FX_MODE_PALETTE]                 = &WS2812FX::mode_palette;
      _mode[FX_MODE_COLORWAVES]              = &WS2812FX::mode_colorwaves;
      _mode[FX_MODE_FILLNOISE8]              = &WS2812FX::mode_fillnoise8;
      _mode[FX_MODE_NOISE16_1]               = &WS2812FX::mode_noise16_1;
      _mode[FX_MODE_NOISE16_2]               = &WS2812FX::mode_noise16_2;
      _mode[FX_MODE_NOISE16_3]               = &WS2812FX::mode_noise16_3;
      _mode[FX_MODE_NOISE16_4]               = &WS2812FX::mode_noise16_4;
      _mode[FX_MODE_COLORTWINKLE]            = &WS2812FX::mode_colortwinkle;
      _mode[FX_MODE_LAKE]                    = &WS2812FX::mode_lake;
      _mode[FX_MODE_METEOR]                  = &WS2812FX::mode_meteor;
      _mode[FX_MODE_METEOR_SMOOTH]           = &WS2812FX::mode_meteor_smooth;
      _mode[FX_MODE_RAILWAY]                 = &WS2812FX::mode_railway;
      _mode[FX_MODE_RIPPLE]                  = &WS2812FX::mode_ripple;
      _mode[FX_MODE_TWINKLEFOX]              = &WS2812FX::mode_twinklefox;
      _mode[FX_MODE_TWINKLECAT]              = &WS2812FX::mode_twinklecat;
      _mode[FX_MODE_HALLOWEEN_EYES]          = &WS2812FX::mode_halloween_eyes;
      _mode[FX_MODE_STATIC_PATTERN]          = &WS2812FX::mode_static_pattern;
      _mode[FX_MODE_TRI_STATIC_PATTERN]      = &WS2812FX::mode_tri_static_pattern;
      _mode[FX_MODE_SPOTS]                   = &WS2812FX::mode_spots;
      _mode[FX_MODE_SPOTS_FADE]              = &WS2812FX::mode_spots_fade;
      _mode[FX_MODE_GLITTER]                 = &WS2812FX::mode_glitter;
      _mode[FX_MODE_CANDLE]                  = &WS2812FX::mode_candle;

      _brightness = DEFAULT_BRIGHTNESS;
      currentPalette = CRGBPalette16(CRGB::Black);
      targetPalette = CloudColors_p;
      ablMilliampsMax = 850;
      currentMilliamps = 0;
      timebase = 0;
      _locked = nullptr;
      _modeUsesLock = false;
      bus = new NeoPixelWrapper();
      resetSegments();
    }

    void
      init(bool supportWhite, uint16_t countPixels, bool skipFirs, uint8_t disableNLeds),
      service(void),
      blur(uint8_t),
      fade_out(uint8_t r),
      setMode(uint8_t segid, uint8_t m),
      setColor(uint8_t slot, uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0),
      setColor(uint8_t slot, uint32_t c),
      setBrightness(uint8_t b),
      driverModeCronixie(bool b),
      setCronixieDigits(byte* d),
      setCronixieBacklight(bool b),
      setIndividual(uint16_t i, uint32_t col),
      setRange(uint16_t i, uint16_t i2, uint32_t col),
      lock(uint16_t i),
      lockRange(uint16_t i, uint16_t i2),
      unlock(uint16_t i),
      unlockRange(uint16_t i, uint16_t i2),
      unlockAll(void),
      setTransitionMode(bool t),
      trigger(void),
      setSegment(uint8_t n, uint16_t start, uint16_t stop),
      resetSegments(),
      setPixelColor(uint16_t n, uint32_t c),
      setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0),
      show(void);

    bool
      reverseMode = false,
      gammaCorrectBri = false,
      gammaCorrectCol = true,
      applyToAllSelected = true,
      segmentsAreIdentical(Segment* a, Segment* b),
      setEffectConfig(uint8_t m, uint8_t s, uint8_t i, uint8_t p);

    uint8_t
      mainSegment = 0,
      paletteFade = 0,
      paletteBlend = 0,
      colorOrder = 0,
      milliampsPerLed = 55,
      _disableNLeds = 0,
      getBrightness(void),
      getMode(void),
      getSpeed(void),
      getModeCount(void),
      getPaletteCount(void),
      getMaxSegments(void),
      //getFirstSelectedSegment(void),
      getMainSegmentId(void),
      gamma8(uint8_t),
      get_random_wheel_index(uint8_t);

    uint16_t
      ablMilliampsMax,
      currentMilliamps,
      triwave16(uint16_t),
      getUsableCount();

    uint32_t
      timebase,
      color_wheel(uint8_t),
      color_from_palette(uint16_t, bool, bool, uint8_t, uint8_t pbri = 255),
      color_blend(uint32_t,uint32_t,uint8_t),
      gamma32(uint32_t),
      getLastShow(void),
      getPixelColor(uint16_t),
      getColor(void);

    WS2812FX::Segment&
      getSegment(uint8_t n);

    WS2812FX::Segment_runtime
      getSegmentRuntime(void);

    WS2812FX::Segment*
      getSegments(void);

    // builtin modes
    uint16_t
      mode_static(void),
      mode_blink(void),
      mode_blink_rainbow(void),
      mode_strobe(void),
      mode_strobe_rainbow(void),
      mode_color_wipe(void),
      mode_color_sweep(void),
      mode_color_wipe_random(void),
      mode_color_sweep_random(void),
      mode_random_color(void),
      mode_dynamic(void),
      mode_breath(void),
      mode_fade(void),
      mode_scan(void),
      mode_dual_scan(void),
      mode_theater_chase(void),
      mode_theater_chase_rainbow(void),
      mode_rainbow(void),
      mode_rainbow_cycle(void),
      mode_running_lights(void),
      mode_saw(void),
      mode_twinkle(void),
      mode_dissolve(void),
      mode_dissolve_random(void),
      mode_sparkle(void),
      mode_flash_sparkle(void),
      mode_hyper_sparkle(void),
      mode_multi_strobe(void),
      mode_android(void),
      mode_chase_color(void),
      mode_chase_random(void),
      mode_chase_rainbow(void),
      mode_chase_flash(void),
      mode_chase_flash_random(void),
      mode_chase_rainbow_white(void),
      mode_colorful(void),
      mode_traffic_light(void),
      mode_running_color(void),
      mode_running_red_blue(void),
      mode_running_random(void),
      mode_larson_scanner(void),
      mode_comet(void),
      mode_fireworks(void),
      mode_rain(void),
      mode_merry_christmas(void),
      mode_halloween(void),
      mode_fire_flicker(void),
      mode_gradient(void),
      mode_loading(void),
      mode_police(void),
      mode_police_all(void),
      mode_two_dots(void),
      mode_two_areas(void),
      mode_circus_combustus(void),
      mode_bicolor_chase(void),
      mode_tricolor_chase(void),
      mode_tricolor_wipe(void),
      mode_tricolor_fade(void),
      mode_lightning(void),
      mode_icu(void),
      mode_multi_comet(void),
      mode_dual_larson_scanner(void),
      mode_random_chase(void),
      mode_oscillate(void),
      mode_fire_2012(void),
      mode_pride_2015(void),
      mode_bpm(void),
      mode_juggle(void),
      mode_palette(void),
      mode_colorwaves(void),
      mode_fillnoise8(void),
      mode_noise16_1(void),
      mode_noise16_2(void),
      mode_noise16_3(void),
      mode_noise16_4(void),
      mode_colortwinkle(void),
      mode_lake(void),
      mode_meteor(void),
      mode_meteor_smooth(void),
      mode_railway(void),
      mode_ripple(void),
      mode_twinklefox(void),
      mode_twinklecat(void),
      mode_halloween_eyes(void),
      mode_static_pattern(void),
	    mode_tri_static_pattern(void),
      mode_spots(void),
      mode_spots_fade(void),
      mode_glitter(void),
      mode_candle(void);
      

  private:
    NeoPixelWrapper *bus;

    uint32_t crgb_to_col(CRGB fastled);
    CRGB col_to_crgb(uint32_t);
    CRGBPalette16 currentPalette;
    CRGBPalette16 targetPalette;

    uint32_t now;
    uint16_t _length, _lengthRaw, _usableCount;
    uint16_t _rand16seed;
    uint8_t _brightness;

    void handle_palette(void);
    void fill(uint32_t);
    bool modeUsesLock(uint8_t);

    bool
      _modeUsesLock,
      _rgbwMode,
      _cronixieMode,
      _cronixieBacklightEnabled,
      _skipFirstMode,
      _triggered;

    byte* _locked;
    byte _cronixieDigits[6];

    mode_ptr _mode[MODE_COUNT]; // SRAM footprint: 4 bytes per element

    // mode helper functions
    uint16_t
      blink(uint32_t, uint32_t, bool strobe, bool),
      color_wipe(bool, bool),
      scan(bool),
      theater_chase(uint32_t, uint32_t, bool),
      running_base(bool),
      dissolve(uint32_t),
      chase(uint32_t, uint32_t, uint32_t, bool),
      gradient_base(bool),
      police_base(uint32_t, uint32_t),
      running(uint32_t, uint32_t),
      tricolor_chase(uint32_t, uint32_t),
      twinklefox_base(bool),
      spots_base(uint16_t);

    CRGB twinklefox_one_twinkle(uint32_t ms, uint8_t salt, bool cat);
    
    uint32_t _lastPaletteChange = 0;
    uint32_t _lastShow = 0;
    
    uint8_t _segment_index = 0;
    uint8_t _segment_index_palette_last = 99;
    segment _segments[MAX_NUM_SEGMENTS] = { // SRAM footprint: 24 bytes per element
      // start, stop, speed, intensity, palette, mode, options, 3 unused bytes (group, spacing, opacity), color[]
      { 0, 7, DEFAULT_SPEED, 128, 0, DEFAULT_MODE, NO_OPTIONS, 1, 0, 255, {DEFAULT_COLOR}}
    };
    segment_runtime _segment_runtimes[MAX_NUM_SEGMENTS]; // SRAM footprint: 16 bytes per element
};


//10 names per line
const char JSON_mode_names[] PROGMEM = R"=====([
"Solid","Blink","Breathe","Wipe","Wipe Random","Random Colors","Sweep","Dynamic","Colorloop","Rainbow",
"Scan","Dual Scan","Fade","Theater","Theater Rainbow","Running","Saw","Twinkle","Dissolve","Dissolve Rnd",
"Sparkle","Sparkle Dark","Sparkle+","Strobe","Strobe Rainbow","Strobe Mega","Blink Rainbow","Android","Chase","Chase Random",
"Chase Rainbow","Chase Flash","Chase Flash Rnd","Rainbow Runner","Colorful","Traffic Light","Sweep Random","Running 2","Red & Blue","Stream",
"Scanner","Lighthouse","Fireworks","Rain","Merry Christmas","Fire Flicker","Gradient","Loading","Police","Police All",
"Two Dots","Two Areas","Circus","Halloween","Tri Chase","Tri Wipe","Tri Fade","Lightning","ICU","Multi Comet",
"Scanner Dual ","Stream 2","Oscillate","Pride 2015","Juggle","Palette","Fire 2012","Colorwaves","Bpm","Fill Noise",
"Noise 1","Noise 2","Noise 3","Noise 4","Colortwinkles","Lake","Meteor","Meteor Smooth","Railway","Ripple",
"Twinklefox","Twinklecat","Halloween Eyes","Solid Pattern","Solid Pattern Tri","Spots","Spots Fade","Glitter","Candle"
])=====";


const char JSON_palette_names[] PROGMEM = R"=====([
"Default","Random Cycle","Primary Color","Based on Primary","Set Colors","Based on Set","Party","Cloud","Lava","Ocean",
"Forest","Rainbow","Rainbow Bands","Sunset","Rivendell","Breeze","Red & Blue","Yellowout","Analogous","Splash",
"Pastel","Sunset 2","Beech","Vintage","Departure","Landscape","Beach","Sherbet","Hult","Hult 64",
"Drywet","Jul","Grintage","Rewhi","Tertiary","Fire","Icefire","Cyane","Light Pink","Autumn",
"Magenta","Magred","Yelmag","Yelblu","Orange & Teal","Tiamat","April Night","Orangery","C9","Sakura",
"Aurora"
])=====";

#endif
