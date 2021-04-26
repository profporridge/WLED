#include "wled.h"
/*
 * Register your v2 usermods here!
 *   (for v1 usermods using just usermod.cpp, you can ignore this file)
 */

/*
 * Add/uncomment your usermod filename here (and once more below)
 * || || ||
 * \/ \/ \/
 */
//#include "usermod_v2_example.h"
#ifdef USERMOD_DALLASTEMPERATURE
#include "../usermods/Temperature/usermod_temperature.h"
#endif
#ifdef USERMOD_TTGO_T_DISPLAY
#include "../usermods/ssd1306_i2c_oled_u8g2/wled06_usermod.h"
#endif
//#include "usermod_v2_empty.h"
#ifdef USERMOD_BUZZER
#include "../usermods/buzzer/usermod_v2_buzzer.h"
#endif
#ifdef USERMOD_SENSORSTOMQTT
#include "usermod_v2_SensorsToMqtt.h"
#endif
#ifdef USERMOD_TTGO_EINK
#include "../usermods/TTGO-eink/ttgo-eink.h"
#endif
#ifdef USERMOD_M5STICKC
#include "usermod_m5stickc.h"
#endif
#ifdef USERMOD_MENU
#include "../usermods/menu/menu_main.cpp"
//#include "../usermods/menu/menu_menu.h"
#endif
#ifdef USERMOD_ROTARY
#include "usermode_rotary_set.h"
#endif
void registerUsermods()
{
/*
   * Add your usermod class name here
   * || || ||
   * \/ \/ \/
   */
#ifdef USERMOD_TTGO_T_DISPLAY
  usermods.add(new U8x8Mod());
#endif
#ifdef USERMOD_M5STICKC
  usermods.add(new M5StickCDriver());
#endif
#ifdef USERMOD_DALLASTEMPERATURE
  usermods.add(new UsermodTemperature());
#endif
  //usermods.add(new UsermodRenameMe());
#ifdef USERMOD_BUZZER
  usermods.add(new BuzzerUsermod());
#endif
#ifdef USERMOD_SENSORSTOMQTT
  usermods.add(new UserMod_SensorsToMQTT());
#endif
#ifdef USERMOD_ROTARY
  usermods.add(new RotaryEncoderSet());
#endif
#ifdef USERMOD_MENU
  usermods.add(new menu_main());
#endif
#ifdef USERMOD_TTGO_EINK
  usermods.add(new ttgo_eink());
  #endif
};