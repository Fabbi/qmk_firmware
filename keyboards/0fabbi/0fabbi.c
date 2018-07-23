#include "0fabbi.h"
#include "debug.h"
#include <print.h>
#include <util/delay.h>

#include "i2c_master.h"
#include "ssd1306.h"
#include "mcp2301X.h"

// matrix_setup();
//
//   timer_init();
//   matrix_init();
// #ifdef PS2_MOUSE_ENABLE
//   ps2_mouse_init();
// #ifdef SERIAL_MOUSE_ENABLE
//   serial_mouse_init();
// #ifdef ADB_MOUSE_ENABLE
//   adb_mouse_init();
// #ifdef BOOTMAGIC_ENABLE
//   bootmagic();
// #else
//   magic();
// #ifdef BACKLIGHT_ENABLE
//   backlight_init();
// #ifdef RGBLIGHT_ENABLE
//   rgblight_init();
// #ifdef STENO_ENABLE
//   steno_init();
// #ifdef FAUXCLICKY_ENABLE
//   fauxclicky_init();
// #ifdef POINTING_DEVICE_ENABLE
//   pointing_device_init();
//
// matrix_scan();

uint16_t getFreeSram(void) {
  uint8_t newVariable;
  // if heap is empty, use bss as start memory address
  if ((uint16_t)__brkval == 0)
    return (((uint16_t)&newVariable) - ((uint16_t)&__bss_end));
  // else use heap end as the start of the memory address
  else
    return (((uint16_t)&newVariable) - ((uint16_t)__brkval));
};

void matrix_init_kb(void)
{
  dprint("matrix_init_kb\n");

  debug_enable = true;

  fabbi_lcd_init();
  fabbi_expander_init();
}

void matrix_scan_kb(void)
{

}

void fabbi_expander_init(void)
{
#ifndef NO_EXPANDER
  expander_init();
#endif
}

static const uint8_t PROGMEM fabbiLogo[] = {FLOGO};
void fabbi_lcd_init(void)
{
  bool success = iota_gfx_init();
  if (success) {
    LED_ON;
    dprint("FOUND DISPLAY\n");
    if (iota_gfx_on()) {
      dprint("GFX ON\n");
      iota_gfx_flush();
    } else {
      dprint("GFX NOT ON\n");
    }
  } else {
    dprint("NO DISPLAY FOUND\n");
  }
  // lcd_init();

  // fb_draw_bitmap(0, 0, 40, 32, fabbiLogo, 1);
}
