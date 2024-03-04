// Credits to @mrVanboy, @gwaland and my dearest friend @westward
// Also for @spiff72 for usermod TTGO-T-Display
// 210217
#pragma once

#include "wled.h"
#include <TFT_eSPI.h>
#include <SPI.h>

#ifndef USER_SETUP_LOADED
    #ifndef ST7789_DRIVER
        #error Please define ST7789_DRIVER
    #endif
    #ifndef TFT_WIDTH
        #error Please define TFT_WIDTH
    #endif
    #ifndef TFT_HEIGHT
        #error Please define TFT_HEIGHT
    #endif
    // #ifndef TFT_MOSI
    //     #error Please define TFT_MOSI
    // #endif
    // #ifndef TFT_SCLK
    //     #error Please define TFT_SCLK
    // #endif
    // #ifndef TFT_DC
    //     #error Please define TFT_DC
    // #endif
    // #ifndef TFT_RST
    //     #error Please define TFT_RST
    // #endif
    // #ifndef LOAD_GLCD
    //     #error Please define LOAD_GLCD
    // #endif
#endif
#ifndef TFT_BL
    #define TFT_BL -1
#endif

#define USERMOD_ID_ST7789_DISPLAY 97

TFT_eSPI tft = TFT_eSPI(TFT_WIDTH, TFT_HEIGHT); // Invoke custom library

// Extra char (+1) for null
#define LINE_BUFFER_SIZE          40

// How often we are redrawing screen
#define USER_LOOP_REFRESH_RATE_MS 100

extern int getSignalQuality(int rssi);


//class name. Use something descriptive and leave the ": public Usermod" part :)
class St7789DisplayUsermod : public Usermod {
  private:
    //Private class members. You can declare variables and functions only accessible to your usermod here
    unsigned long lastTime = 0;
    bool enabled = true;

    bool displayTurnedOff = false;
    long lastRedraw = 0;
    // needRedraw marks if redraw is required to prevent often redrawing.
    bool needRedraw = true;
    // Next variables hold the previous known values to determine if redraw is required.
    String knownSsid = "";
    IPAddress knownIp;
    uint8_t knownBrightness = 0;
    uint8_t knownMode = 0;
    uint8_t knownPalette = 0;
    uint8_t knownEffectSpeed = 0;
    uint8_t knownEffectIntensity = 0;
    uint8_t knownMinute = 99;
    uint8_t knownHour = 99;
    uint8_t knownSecond = 0;

    const uint8_t tftcharwidth = 24;  // Number of chars that fit on screen with text size set to 2
    long lastUpdate = 0;

    void center(String &line, uint8_t width) {
      int len = line.length();
      if (len<width) for (byte i=(width-len)/2; i>0; i--) line = ' ' + line;
      for (byte i=line.length(); i<width; i++) line += ' ';
    }

    /**
     * Display the current date and time in large characters
     * on the middle rows. Based 24 or 12 hour depending on
     * the useAMPM configuration.
     */
    void showTime() {
        if (!ntpEnabled) return;
        char lineBuffer[LINE_BUFFER_SIZE];

        // updateLocalTime();
        // byte minuteCurrent = minute(localTime);
        // byte hourCurrent   = hour(localTime);
        // byte secondCurrent = second(localTime);
        // knownMinute = minuteCurrent;
        // knownHour = hourCurrent;
      // knownSecond = millis()/1000;

        // byte currentMonth = month(localTime);
        // sprintf_P(lineBuffer, PSTR("%s %2d "), monthShortStr(currentMonth), day(localTime));
       tft.setTextColor(TFT_BLACK);
        // tft.setCursor(84, 0);
        // tft.setTextSize(2);
        // tft.print(lineBuffer);

        // byte showHour = hourCurrent;
        // boolean isAM = false;
        // if (useAMPM) {
        //     if (showHour == 0) {
        //         showHour = 12;
        //         isAM = true;
        //     } else if (showHour > 12) {
        //         showHour -= 12;
        //         isAM = false;
        //     } else {
        //         isAM = true;
        //     }
        // }
        getTimeString(lineBuffer);
        tft.drawString(lineBuffer, 0, 0, 4);
        // sprintf_P(lineBuffer, PSTR("%2d:%02d:%02d"), (useAMPM ? showHour : hourCurrent), minuteCurrent, secondCurrent);
        // tft.setTextColor(TFT_WHITE);
        // tft.setTextSize(4);
        // tft.setCursor(60, 24);
        //  tft.drawCentreString(lineBuffer, 64, 24, 4);
        // tft.print(lineBuffer);

        // tft.setTextSize(2);
        // tft.setCursor(186, 24);
        // //sprintf_P(lineBuffer, PSTR("%02d"), secondCurrent);
        // if (useAMPM) tft.print(isAM ? "AM" : "PM");
        // //else         tft.print(lineBuffer);
    }

  public:
    //Functions called by WLED

    /*
     * setup() is called once at boot. WiFi is not yet connected at this point.
     * You can use it to initialize variables, sensors or similar.
     */
    void setup()
    {
        #ifdef TTGO_TDISPLAY_S3
        #define PIN_LCD_BL                   38

#define PIN_LCD_D0                   39
#define PIN_LCD_D1                   40
#define PIN_LCD_D2                   41
#define PIN_LCD_D3                   42
#define PIN_LCD_D4                   45
#define PIN_LCD_D5                   46
#define PIN_LCD_D6                   47
#define PIN_LCD_D7                   48

#define PIN_POWER_ON                 15

#define PIN_LCD_RES                  5
#define PIN_LCD_CS                   6
#define PIN_LCD_DC                   7
#define PIN_LCD_WR                   8
#define PIN_LCD_RD                   9

#define PIN_BUTTON_1                 0
#define PIN_BUTTON_2                 14
#define PIN_BAT_VOLT                 4

#define PIN_IIC_SCL                  17
#define PIN_IIC_SDA                  18

#define PIN_TOUCH_INT                16
#define PIN_TOUCH_RES                21
// ST7789 240 x 240 display with no chip select line
#define USER_SETUP_ID 24

#define ST7789_DRIVER // Configure all registers

// #define TFT_RGB_ORDER TFT_BGR // Colour order Blue-Green-Red

#define TFT_WIDTH 170  // ST7789 172 x 320
#define TFT_HEIGHT 320 // ST7789 240 x 320

#define CGRAM_OFFSET
// #define TFT_RGB_ORDER TFT_RGB  // Colour order Red-Green-Blue
#define TFT_RGB_ORDER TFT_BGR  // Colour order Blue-Green-Red

//#define TFT_INVERSION_ON
//#define TFT_INVERSION_OFF

#define TFT_PARALLEL_8_BIT
#endif
// The ESP32 and TFT the pins used for testing are:
#define TFT_CS 6  // Chip select control pin (library pulls permanently low
#define TFT_DC 7  // Data Command control pin - must use a pin in the range 0-31
#define TFT_RST 5 // Reset pin, toggles on startup

#define TFT_WR 8 // Write strobe control pin - must use a pin in the range 0-31
#define TFT_RD 9 // Read strobe control pin

#define TFT_DATA_PIN_OFFSET_EN /* The ESP32S3 controller is controlled by two registers. \
                                Select data pin numbers higher than 32 to enable this option, \
                                this macro definition added by Micky -20220701 */
#ifdef TTG_TDISPLAY_S3
#define TFT_D0 39 // Must use pins in the range 0-31 for the data bus
#define TFT_D1 40 // so a single register write sets/clears all bits.
#define TFT_D2 41 // Pins can be randomly assigned, this does not affect
#define TFT_D3 42 // TFT screen update performance.
#define TFT_D4 45
#define TFT_D5 46
#define TFT_D6 47
#define TFT_D7 48

#define TFT_BL 38 // LED back-light (required for M5Stack)

#define LOAD_GLCD  // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2 // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
//#define LOAD_FONT4 // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
//#define LOAD_FONT6 // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
//#define LOAD_FONT7 // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
//#define LOAD_FONT8 // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
//#define LOAD_FONT8N // Font 8. Alternative to Font 8 above, slightly narrower, so 3 digits fit a 160 pixel TFT
#define LOAD_GFXFF // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT
pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);

  Serial.begin(115200);
  Serial.println("Hello T-Display-S3");

//   tft.begin();
//   tft.setRotation(3);
//   tft.setSwapBytes(true);
  //tft.pushImage(0, 0, 320, 170, (uint16_t *)img_logo);
  //delay(2000);

  ledcSetup(0, 2000, 8);
  ledcAttachPin(PIN_LCD_BL, 0);
  ledcWrite(0, 255);

   PinManagerPinType pins[] = { { TFT_WR, true }, { TFT_RD, false}, { TFT_CS, true}, { TFT_DC, true}, { TFT_RST, true }, { TFT_BL, true }, { TFT_D0, true }, { TFT_D1, false}, { TFT_D2, true }, { TFT_D3, true}, { TFT_D4, true}, { TFT_D5, true }, { TFT_D6, true },  { TFT_D7, true }   };
        if (!pinManager.allocateMultiplePins(pins, 14, PinOwner::UM_FourLineDisplay)) { enabled = false; return; }

   #else     
        PinManagerPinType pins[] = { { TFT_MOSI, true }, { TFT_MISO, false}, { TFT_SCLK, true }, { TFT_CS, true}, { TFT_DC, true}, { TFT_RST, true }, { TFT_BL, true } };
        if (!pinManager.allocateMultiplePins(pins, 7, PinOwner::UM_FourLineDisplay)) { enabled = false; return; }
   #endif
        tft.init();
        tft.setRotation(3);  //Rotation here is set up for the text to be readable with the port on the left. Use 1 to flip.
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_RED);
        tft.setCursor(60, 100);
        tft.setTextDatum(MC_DATUM);
        tft.setTextSize(2);
        tft.print("Loading...");
  #ifndef TTGO_TDISPLAY_S3
        if (TFT_BL >= 0) 
        {
            pinMode(TFT_BL, OUTPUT); // Set backlight pin to output mode
            digitalWrite(TFT_BL, HIGH); // Turn backlight on.
        }
        #endif
     
    }

    /*
     * connected() is called every time the WiFi is (re)connected
     * Use it to initialize network interfaces
     */
    void connected() {
      //Serial.println("Connected to WiFi!");
    }

    /*
     * loop() is called continuously. Here you can check for events, read sensors, etc.
     *
     * Tips:
     * 1. You can use "if (WLED_CONNECTED)" to check for a successful network connection.
     *    Additionally, "if (WLED_MQTT_CONNECTED)" is available to check for a connection to an MQTT broker.
     *
     * 2. Try to avoid using the delay() function. NEVER use delays longer than 10 milliseconds.
     *    Instead, use a timer check as shown here.
     */
    void loop() {
        char buff[LINE_BUFFER_SIZE];

        // Check if we time interval for redrawing passes.
        if (millis() - lastUpdate < USER_LOOP_REFRESH_RATE_MS)
        {
            return;
        }
        lastUpdate = millis();
       showTime();
        // Turn off display after 5 minutes with no change.
        if (!displayTurnedOff && millis() - lastRedraw > 5*60*1000)
        {
            
#ifdef TTGO_TDISPLAY_S3
 ledcWrite(0, 15);
 #else
 if (TFT_BL >= 0) 
                 digitalWrite(TFT_BL, LOW); // Turn backlight off. 
                 #endif
            displayTurnedOff = true;
        } 

        // Check if values which are shown on display changed from the last time.
        if ((((apActive) ? String(apSSID) : WiFi.SSID()) != knownSsid) ||
            (knownIp != (apActive ? IPAddress(4, 3, 2, 1) : Network.localIP())) ||
            (knownBrightness != bri) ||
            (knownEffectSpeed != strip.getMainSegment().speed) ||
            (knownEffectIntensity != strip.getMainSegment().intensity) ||
            (knownMode != strip.getMainSegment().mode) ||
            (knownPalette != strip.getMainSegment().palette))
        {
            needRedraw = true;
        }

        if (!needRedraw)
        {
            return;
        }
        needRedraw = false;
    
        if (displayTurnedOff)
        {
 #ifdef TTGO_TDISPLAY_S3
 ledcWrite(0, 255);
 #else           
 digitalWrite(TFT_BL, HIGH); // Turn backlight on.
 #endif
            displayTurnedOff = false;
        }
        lastRedraw = millis();

        // Update last known values.
        #if defined(ESP8266)
            knownSsid = apActive ? WiFi.softAPSSID() : WiFi.SSID();
        #else
            knownSsid = WiFi.SSID();
        #endif
        knownIp = apActive ? IPAddress(4, 3, 2, 1) : WiFi.localIP();
        knownBrightness = bri;
        knownMode = strip.getMainSegment().mode;
        knownPalette = strip.getMainSegment().palette;
        knownEffectSpeed = strip.getMainSegment().speed;
        knownEffectIntensity = strip.getMainSegment().intensity;

        tft.fillScreen(TFT_BLACK);

   

        tft.setTextSize(2);

        // Wifi name
        tft.setTextColor(TFT_GREEN);
        tft.setCursor(0, 60);
        String line = knownSsid.substring(0, tftcharwidth-1);
        // Print `~` char to indicate that SSID is longer, than our display
        if (knownSsid.length() > tftcharwidth) line = line.substring(0, tftcharwidth-1) + '~';
        center(line, tftcharwidth);
        tft.print(line.c_str());

        // Print AP IP and password in AP mode or knownIP if AP not active.
        if (apActive)
        {
            tft.setCursor(0, 84);
            tft.print("AP IP: ");
            tft.print(knownIp);
            tft.setCursor(0,108);
            tft.print("AP Pass:");
            tft.print(apPass);
        }
        else
        {
            tft.setCursor(0, 84);
            line = knownIp.toString();
            center(line, tftcharwidth);
            tft.print(line.c_str());
            // percent brightness
            tft.setCursor(0, 120);
            tft.setTextColor(TFT_WHITE);
            tft.print("Bri: ");
            tft.print((((int)bri*100)/255));
            tft.print("%");
            // signal quality
            tft.setCursor(124,120);
            tft.print("Sig: ");
            if (getSignalQuality(WiFi.RSSI()) < 10) {
                tft.setTextColor(TFT_RED);
            } else if (getSignalQuality(WiFi.RSSI()) < 25) {
                tft.setTextColor(TFT_ORANGE);
            } else {
                tft.setTextColor(TFT_GREEN);
            }
            tft.print(getSignalQuality(WiFi.RSSI()));
            tft.setTextColor(TFT_WHITE);
            tft.print("%");
        }

        // mode name
        tft.setTextColor(TFT_CYAN);
        tft.setCursor(0, 144);
        char lineBuffer[tftcharwidth+1];
        extractModeName(knownMode, JSON_mode_names, lineBuffer, tftcharwidth);
        tft.print(lineBuffer);

        // palette name
        tft.setTextColor(TFT_YELLOW);
        tft.setCursor(0, 168);
        extractModeName(knownPalette, JSON_palette_names, lineBuffer, tftcharwidth);
        tft.print(lineBuffer);

        tft.setCursor(0, 192);
        tft.setTextColor(TFT_SILVER);
        sprintf_P(buff, PSTR("FX  Spd:%3d Int:%3d"), effectSpeed, effectIntensity);
        tft.print(buff);

        // Fifth row with estimated mA usage
        tft.setTextColor(TFT_SILVER);
        tft.setCursor(0, 216);
        // Print estimated milliamp usage (must specify the LED type in LED prefs for this to be a reasonable estimate).
        tft.print("Current: ");
        tft.setTextColor(TFT_ORANGE);
        tft.print(strip.currentMilliamps);
        tft.print("mA");
    }

    /*
     * addToJsonInfo() can be used to add custom entries to the /json/info part of the JSON API.
     * Creating an "u" object allows you to add custom key/value pairs to the Info section of the WLED web UI.
     * Below it is shown how this could be used for e.g. a light sensor
     */
    void addToJsonInfo(JsonObject& root)
    {
      JsonObject user = root["u"];
      if (user.isNull()) user = root.createNestedObject("u");

      JsonArray lightArr = user.createNestedArray("ST7789"); //name
      lightArr.add(enabled?F("installed"):F("disabled")); //unit
    }


    /*
     * addToJsonState() can be used to add custom entries to the /json/state part of the JSON API (state object).
     * Values in the state object may be modified by connected clients
     */
    void addToJsonState(JsonObject& root)
    {
      //root["user0"] = userVar0;
    }


    /*
     * readFromJsonState() can be used to receive data clients send to the /json/state part of the JSON API (state object).
     * Values in the state object may be modified by connected clients
     */
    void readFromJsonState(JsonObject& root)
    {
      //userVar0 = root["user0"] | userVar0; //if "user0" key exists in JSON, update, else keep old value
      //if (root["bri"] == 255) Serial.println(F("Don't burn down your garage!"));
    }


    /*
     * addToConfig() can be used to add custom persistent settings to the cfg.json file in the "um" (usermod) object.
     * It will be called by WLED when settings are actually saved (for example, LED settings are saved)
     * If you want to force saving the current state, use serializeConfig() in your loop().
     *
     * CAUTION: serializeConfig() will initiate a filesystem write operation.
     * It might cause the LEDs to stutter and will cause flash wear if called too often.
     * Use it sparingly and always in the loop, never in network callbacks!
     *
     * addToConfig() will also not yet add your setting to one of the settings pages automatically.
     * To make that work you still have to add the setting to the HTML, xml.cpp and set.cpp manually.
     *
     * I highly recommend checking out the basics of ArduinoJson serialization and deserialization in order to use custom settings!
     */
    void addToConfig(JsonObject& root)
    {
      JsonObject top = root.createNestedObject("ST7789");
      JsonArray pins = top.createNestedArray("pin");
#ifndef TTGO_TDISPLAY_S3      
      pins.add(TFT_MOSI);
      pins.add(TFT_MISO);
      pins.add(TFT_SCLK);
#endif
      pins.add(TFT_CS);
      pins.add(TFT_DC);
      pins.add(TFT_RST);
      pins.add(TFT_BL);
#ifdef TTGO_TDISPLAY_S3

pins.add(TFT_D0);// 39 // Must use pins in the range 0-31 for the data bus
pins.add(TFT_D1);// 40 // so a single register write sets/clears all bits.
pins.add(TFT_D2);// 41 // Pins can be randomly assigned, this does not affect
pins.add(TFT_D3);// 42 // TFT screen update performance.
pins.add(TFT_D4);// 45
pins.add(TFT_D5);// 46
pins.add(TFT_D6);// 47
pins.add(TFT_D7);// 48


#endif
      //top["great"] = userVar0; //save this var persistently whenever settings are saved
    }


    /*
     * readFromConfig() can be used to read back the custom settings you added with addToConfig().
     * This is called by WLED when settings are loaded (currently this only happens once immediately after boot)
     *
     * readFromConfig() is called BEFORE setup(). This means you can use your persistent values in setup() (e.g. pin assignments, buffer sizes),
     * but also that if you want to write persistent values to a dynamic buffer, you'd need to allocate it here instead of in setup.
     * If you don't know what that is, don't fret. It most likely doesn't affect your use case :)
     */
    bool readFromConfig(JsonObject& root)
    {
      //JsonObject top = root["top"];
      //userVar0 = top["great"] | 42; //The value right of the pipe "|" is the default value in case your setting was not present in cfg.json (e.g. first boot)
      return true;
    }


    /*
     * getId() allows you to optionally give your V2 usermod an unique ID (please define it in const.h!).
     * This could be used in the future for the system to determine whether your usermod is installed.
     */
    uint16_t getId()
    {
      return USERMOD_ID_ST7789_DISPLAY;
    }

   //More methods can be added in the future, this example will then be extended.
   //Your usermod will remain compatible as it does not need to implement all methods from the Usermod base class!
};