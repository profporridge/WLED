#pragma once

#include "wled.h"

/* This driver reads quaternion data from the MPU6060 and adds it to the JSON
   This example is adapted from:
   https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050/examples/MPU6050_DMP6_ESPWiFi

   Tested with a d1 mini esp-12f

  GY-521  NodeMCU
  MPU6050 devkit 1.0
  board   Lolin         Description
  ======= ==========    ====================================================
  VCC     VU (5V USB)   Not available on all boards so use 3.3V if needed.
  GND     G             Ground
  SCL     D1 (GPIO05)   I2C clock
  SDA     D2 (GPIO04)   I2C data
  XDA     not connected
  XCL     not connected
  AD0     not connected
  INT     D8 (GPIO15)   Interrupt pin
  
  Using usermod:
  1. Copy the usermod into the sketch folder (same folder as wled00.ino)
  2. Register the usermod by adding #include "usermod_filename.h" in the top and registerUsermod(new MyUsermodClass()) in the bottom of usermods_list.cpp
  3. I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h file 
     for both classes must be in the include path of your project. To install the
     libraries add I2Cdevlib-MPU6050@fbde122cc5 to lib_deps in the platformio.ini file.
  4. You also need to change lib_compat_mode from strict to soft in platformio.ini (This ignores that I2Cdevlib-MPU6050 doesn't list platform compatibility)
  5. Wire up the MPU6050 as detailed above.
*/

//#include "I2Cdev.h"

//#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
//#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include <Wire.h>
//#endif
#include "m5stickc.h"
//#include "m5stick-spectrum.h"
#define M5STICKC_LCD
#define UPDATE_INTERVAL_MILLIS = 100
//#define m5stick_microphone =0
/*M5StickSpectrum m5spec = M5StickSpectrum();
void getFFTBins_Wrapper(double (vOut)[SAMPLES], double *f, double *v)
{m5spec.getFFTBins(vOut,f,v);}
#include "audio_reactive.h"
*/
class M5StickCDriver : public Usermod
{

  float pitch, roll, yaw, gyroX, gyroY, gyroZ, accX, accY, accZ, temp;
  float ypr[3];  // [yaw, pitch, roll]   yaw/pitch/roll container
  float gyro[3]; // [gyroX, gyroY, gyroZ] gyro container
  float accel[3];
  uint32_t nextUpdateTime;
  TFT_eSprite tftSprite = TFT_eSprite(&M5.Lcd);

public:
  //Functions called by WLED

  /*
     * setup() is called once at boot. WiFi is not yet connected at this point.
     */
  void setup()
  {
    M5.begin(true, false, false);
//delay(5000);
// join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
//    Wire.begin();
//    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif
    Serial.println(F("Initializing I2C devices..."));
    //mpu.initialize();
    ;
    //   int x = M5.IMU.Init();
    //  Serial.print("MPU init result was ");
    //   Serial.println(x);

#ifdef m5stick_microphone

    //  m5spec.setup();
    // digitalMic = true;
#endif

    tftSprite.createSprite(160, 50);
    tftSprite.setRotation(1);
    // M5.Axp.EnableCoulombcounter();
#ifndef USE_M5_LCD
    M5.Lcd.setRotation(1);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(TFT_MAGENTA);
    M5.Lcd.setTextSize(1);
    M5.Axp.SetLDO2(true);
#endif
    avgVCurr = M5.Axp.GetVBusCurrent();
    avgVBatt = M5.Axp.GetBatVoltage();
    //i2sInit();
    //xTaskCreatePinnedToCore(mic_record_task, "mic_record_task", 2048, NULL, 1, &FFT_Task, 0);

    //sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
    //Serial.println("sampleperiod: " + sampling_period_us);
    // delay(2000);
    /*for (uint8_t i = 0; i < tft_height; i++)
    {
      //colormap[i] = M5.Lcd.color565(tft_height-i*.5,i*1.1,0); //RGB
      colormap[i] = M5.Lcd.color565(tft_height - i * 4.4, i * 2.5, 0); //RGB:rev macsbug
    }
    for (byte band = 0; band <= 7; band++)
    {
      M5.Lcd.setCursor(bands_width * band + 2, 10);
      M5.Lcd.print(this->audiospectrum[band].freqname);
    }
    */
    // }
  }
  bool needRedraw = true;
  /*
     * connected() is called every time the WiFi is (re)connected
     * Use it to initialize network interfaces
     */
  void connected()
  {
    needRedraw = true;
  }
  float avgTemp = 0.0, avgVBatt = 0.0, avgVCurr = 0.0, avgBCurr = 0.0; // needRedraw marks if redraw is required to prevent often redrawing.


  // Next variables hold the previous known values to determine if redraw is
  // required.
  String knownSsid = "";
  IPAddress knownIp;
  uint8_t knownBrightness = 0;
  uint8_t knownMode = 0;
  uint8_t knownPalette = 0;

  long nextUpdate = 0;
  long lastRedraw = 0;
  bool displayTurnedOff = false;
// How often we are redrawing screen
#define USER_LOOP_REFRESH_RATE_MS 1000
float avgUpdateTimeA = 0, avgUpdateTimeB = 0;
uint32_t lastUpdateTime=0;
  /*
     * loop() is called continuously. Here you can check for events, read sensors, etc.
     */
  void loop()
  {
    // begin section from usermod:

    // if (millis()-lastTime > delayMs) {                        // I need to run this continuously because the animations are too slow
    //   if (!(audioSyncEnabled & (1 << 1))) {                   // Only run the sampling code IF we're not in Receive mode
    //     lastTime = millis();
    //     micData = adcBuffer[millis()%32];
    //     micIn = micData;
    //     getSample();                                          // Sample the microphone
    //     agcAvg();                                             // Calculated the PI adjusted value as sampleAvg
    //     myVals[millis()%32] = sampleAgc;
    //     if (samplePeak > 0 )
    //     logAudio();
    //   }
    //   #ifdef ESP32
    //   if (audioSyncEnabled & (1 << 0)) {
    //     // Only run the transmit code IF we're in Transmit mode
    //   //  Serial.println("Transmitting UDP Mic Packet");
    //     transmitAudioData();
    //   }
    //   #endif
    // }// section from usermod.
    // limit update rate, crudely
    uint32_t currentTime = millis();
    if (currentTime > nextUpdateTime)
    {
      
      nextUpdateTime = currentTime + USER_LOOP_REFRESH_RATE_MS;

      M5.IMU.getTempData(&temp);
      // M5.IMU.getAhrsData(&pitch, &roll, &yaw);
      //ypr = [&yaw, pitch, roll];
      // M5.IMU.getAccelData(&accX, &accY, &accZ);
      // M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
      avgTemp = ((avgTemp * 31) + temp) / 32;
      // avgVBatt = ((avgVBatt * 31) + M5.Axp.GetBatVoltage()) / 32;
      // avgVCurr = ((avgVCurr * 31) + M5.Axp.GetVBusCurrent()) / 32;
      tftSprite.fillSprite(BLACK);
      tftSprite.setCursor(0, 0, 1);
      tftSprite.printf("Temp: %.1fC aud:%d mic:%d \r\n", M5.Axp.GetTempInAXP192(), audioSyncEnabled, digitalMic);
      tftSprite.setCursor(0, 10);
      tftSprite.printf("Bat:   %.2fv %.2f/%.0fma\r\n", M5.Axp.GetBatVoltage(), M5.Axp.GetBatCurrent(), M5.Axp.GetBatChargeCurrent());
      tftSprite.setCursor(0, 20);
      tftSprite.printf("USB:   %.2fv %.2fma\r\n", M5.Axp.GetVBusVoltage(), M5.Axp.GetVBusCurrent());
      tftSprite.setCursor(0, 30);
      tftSprite.printf("5VIn:  %.2fv %.2fma\r\n", M5.Axp.GetVinVoltage(), M5.Axp.GetVinCurrent());
      tftSprite.setCursor(0, 40);
      tftSprite.printf("Power %.3fmw (%.1f/%.1fms)", M5.Axp.GetBatPower(), avgUpdateTimeA, avgUpdateTimeB);
      tftSprite.pushSprite(0, 0);
    
#ifndef USE_M5_LCD
#ifdef M5STICKC_LCD

#ifdef AXP_DEBUG

    // M5.Lcd.setCursor(0, 45);
    M5.Lcd.printf("Ta:%.1fC B:%.2fV %gmA\n", avgTemp, avgVBatt, avgVCurr);
    M5.Lcd.printf("aud:%d udp:%d mic:%d \n", audioSyncEnabled, udpSyncConnected, digitalMic);

    M5.Lcd.printf("APS:%.1fV VIn:%.1fmA %.1fV\n", M5.Axp.GetAPSVoltage(), M5.Axp.GetVinCurrent(), M5.Axp.GetVinVoltage());

    M5.Lcd.printf("BAT:%.1fW c:%.1fmA %.1fmA\n", M5.Axp.GetBatPower(), M5.Axp.GetBatChargeCurrent(), M5.Axp.GetBatCurrent());
    M5.Lcd.printf("BUS:%.1fV %.1fmA", M5.Axp.GetVBusVoltage(), M5.Axp.GetVBusCurrent(), M5.Axp.GetBatCurrent());

    M5.Lcd.printf("PwrSts:%d BCG:%d T%.1f\n", M5.Axp.GetInputPowerStatus(), M5.Axp.GetBatteryChargingStatus(), M5.Axp.GetTempInAXP192());
#endif // M5.Lcd.println();                          \
       // M5.Lcd.printf("IP: %s\n", WiFi.localIP()); \
       // M5.Lcd.printf("Mode: %g, brt: %g\n", strip.getMode(),strip.getBrightness());
#endif

    // Check if we time interval for redrawing passes.
   // uint16_t timeVal = millis() - lastUpdate;
    if (millis() < nextUpdate)
    {

      return;
    }
    nextUpdate = millis()+USER_LOOP_REFRESH_RATE_MS;

    // Turn off display after 3 minutes with no change.
    if (!displayTurnedOff && millis() - lastRedraw > 3 * 60 * 1000)
    {
      M5.Axp.SetLDO2(false);
      displayTurnedOff = true;
    }

    // Check if values which are shown on display changed from the last time.
    if (((apActive) ? String(apSSID) : WiFi.SSID()) != knownSsid)
    {
      needRedraw = true;
    }
    else if (knownIp != (apActive ? IPAddress(4, 3, 2, 1) : WiFi.localIP()))
    {
      needRedraw = true;
    }
    else if (knownBrightness != bri)
    {
      needRedraw = true;
    }
    else if (knownMode != strip.getMode())
    {
      needRedraw = true;
    }
    else if (knownPalette != strip.getSegment(0).palette)
    {
      needRedraw = true;
    }

    if (!needRedraw)
    {
    //       lastUpdateTime = (millis()-currentTime);
    //  updateTime = ((updateTime * 31) + lastUpdateTime) / 32;
      return;
    }
         avgUpdateTimeA = ((avgUpdateTimeA * 31) +  (millis()-currentTime)) / 32;
    M5.Lcd.fillRect(0, 50, 160, 80, 0x000000);
    M5.Lcd.setCursor(0, 50);
    needRedraw = false;

    if (displayTurnedOff)
    {
      Serial.println("Turned Display On");
      M5.Axp.SetLDO2(true);
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
    knownMode = strip.getMode();
    knownPalette = strip.getSegment(0).palette;

    // First row with Wifi name
    //M5.Lcd.setCursor(0, M5.Lcd.height() / 2);
    M5.Lcd.print(knownSsid.substring(0, M5.Lcd.width() > 1 ? M5.Lcd.width() - 2 : 0));
    // Print `~` char to indicate that SSID is longer, than owr dicplay
    if (knownSsid.length() > M5.Lcd.width())
      M5.Lcd.print("~");
    M5.Lcd.println();
    // Second row with IP or Psssword
    // M5.Lcd.setCursor(1, 1);
    // Print password in AP mode and if led is OFF.
    if (apActive && bri == 0)
      M5.Lcd.println(apPass);
    else
      M5.Lcd.println(knownIp);

    // Third row with mode name
    // M5.Lcd.setCursor(2, 2);
    uint8_t qComma = 0;
    bool insideQuotes = false;
    uint8_t printedChars = 0;
    char singleJsonSymbol;
    // Find the mode name in JSON
    for (size_t i = 0; i < strlen_P(JSON_mode_names); i++)
    {
      singleJsonSymbol = pgm_read_byte_near(JSON_mode_names + i);
      switch (singleJsonSymbol)
      {
      case '"':
        insideQuotes = !insideQuotes;
        break;
      case '[':
      case ']':
        break;
      case ',':
        qComma++;
      default:
        if (!insideQuotes || (qComma != knownMode))
          break;
        M5.Lcd.print(singleJsonSymbol);
        printedChars++;
      }
      if ((qComma > knownMode) || (printedChars > M5.Lcd.width() - 2))
      {
        M5.Lcd.println();
        break;
      }
    }
#endif
    // Fourth row with palette name
    // M5.Lcd.setCursor(2, 3);
    // qComma = 0;
    // insideQuotes = false;
    // printedChars = 0;
    // // Looking for palette name in JSON.
    // for (size_t i = 0; i < strlen_P(JSON_palette_names); i++) {
    //   singleJsonSymbol = pgm_read_byte_near(JSON_palette_names + i);
    //   switch (singleJsonSymbol) {
    //   case '"':
    //     insideQuotes = !insideQuotes;
    //     break;
    //   case '[':
    //   case ']':
    //     break;
    //   case ',':
    //     qComma++;
    //   default:
    //     if (!insideQuotes || (qComma != knownPalette))
    //       break;
    //     M5.Lcd.print(singleJsonSymbol);
    //     printedChars++;
    //   }
    //   if ((qComma > knownMode) || (printedChars > M5.Lcd.gCols() - 2))
    //     break;

    //   }
    //     #ifdef m5stick_microphone
    // if (!buff_read){
    //   m5spec.showSignal();
    //   m5spec.getFFTBins(fftBin, &FFT_MajorPeak, &FFT_Magnitude);
    //   fftResultFiller();
    //   }
    // #endif
    // M5.update();
    // M5.Axp.SetAdcState(true);
    lastUpdateTime = (millis()-currentTime);
     avgUpdateTimeB = ((avgUpdateTimeB * 31) + lastUpdateTime) / 32;
     }
  }

  void addToJsonInfo(JsonObject &root)
  {
    // int reading = 20;
    // //this code adds "u":{"Light":[20," lux"]} to the info object
    // JsonObject user = root["m5stick"];
    // if (user.isNull())
    //   user = root.createNestedObject("m5stick");

    // JsonObject imu_meas = user.createNestedObject("IMU");
    // imu_meas.getOrAddMember("temp").set(avgTemp);
    // // JsonArray quat_json = imu_meas.createNestedArray("Quat");
    // // quat_json.add(qat.w);
    // // quat_json.add(qat.x);
    // // quat_json.add(qat.y);
    // // quat_json.add(qat.z);
    // // JsonArray euler_json = imu_meas.createNestedArray("Euler");
    // // euler_json.add(euler[0]);
    // // euler_json.add(euler[1]);
    // // euler_json.add(euler[2]);
    // JsonArray accel_json = imu_meas.createNestedArray("Accel");
    // accel_json.add(accX);
    // accel_json.add(accY);
    // accel_json.add(accZ);
    // JsonArray gyro_json = imu_meas.createNestedArray("Gyro");
    // gyro_json.add(gyroX);
    // gyro_json.add(gyroY);
    // gyro_json.add(gyroZ);
    // // JsonArray world_json = imu_meas.createNestedArray("WorldAccel");
    // // world_json.add(aaWorld.x);
    // // world_json.add(aaWorld.y);
    // // world_json.add(aaWorld.z);
    // // JsonArray real_json = imu_meas.createNestedArray("RealAccel");
    // // real_json.add(aaReal.x);
    // // real_json.add(aaReal.y);
    // // real_json.add(aaReal.z);
    // // JsonArray grav_json = imu_meas.createNestedArray("Gravity");
    // // grav_json.add(gravity.x);
    // // grav_json.add(gravity.y);
    // // grav_json.add(gravity.z);
    // JsonArray orient_json = imu_meas.createNestedArray("Orientation");
    // orient_json.add(yaw);
    // orient_json.add(pitch);
    // orient_json.add(roll);
  }

  /*
     * addToJsonState() can be used to add custom entries to the /json/state part of the JSON API (state object).
     * Values in the state object may be modified by connected clients
     */
  void addToJsonState(JsonObject &root)
  {
    //root["user0"] = userVar0;
  }

  /*
     * readFromJsonState() can be used to receive data clients send to the /json/state part of the JSON API (state object).
     * Values in the state object may be modified by connected clients
     */
  void readFromJsonState(JsonObject &root)
  {
    //if (root["bri"] == 255) Serial.println(F("Don't burn down your garage!"));
  }

  /*
     * getId() allows you to optionally give your V2 usermod an unique ID (please define it in const.h!).
     */
  uint16_t getId()
  {
    return USERMOD_ID_IMU;
  }
};