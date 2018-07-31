#include QMK_KEYBOARD_H
#include "debug.h"
#include "action_layer.h"
#include "version.h"
#include "ssd1306.h"

// only `*_user` methods in here!
/////////////////

// uint16_t num = 0;
enum custom_keycodes {
  PLACEHOLDER = SAFE_RANGE, // THIS MUST STAY!
  MACRO11
  ,MACRO12
  ,MACRO13
  // ,SHFTNUM
};

#define BASE 0 // default layer
#define NUMBERS 1 // MOD (green) layer with numbers
#define CONTROL 2 // controlling (blue) layer (arrow keys, F-Keys, Consumer Keys..)
#define HARDWARE 3 // hardware layer

#define ______ KC_TRNS
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[BASE] =
LAYER(
  // left side
  KC_GRAVE, KC_TAB,   KC_Q, KC_W, KC_E,    KC_R,    KC_T,
  MACRO12,  KC_ESC,   KC_A, KC_S, KC_D,    KC_F,    KC_G,
  KC_APP,   KC_GRAVE, KC_Z, KC_X, KC_C,    KC_V,    KC_B,
  ,         ,         ,     ,     KC_LALT, KC_LGUI, KC_LSHIFT, KC_LCTL, TT(CONTROL),


  // right side
  ,        ,            KC_Y,     KC_U,                  KC_I,      KC_O,   KC_P,    KC_LBRC, KC_RBRC,
  ,        ,            KC_H,     KC_J,                  KC_K,      KC_L,   KC_SCLN, KC_QUOT, KC_ENT,
  ,        ,            KC_N,     KC_M,                  KC_COMM,   KC_DOT, KC_SLSH, KC_BSLS, KC_RALT,
  KC_BSPC, TT(NUMBERS), KC_SPACE, LM(NUMBERS, MOD_LSFT), KC_RGUI
  ),

[NUMBERS] =
LAYER(
  // left side
  ______, ______, KC_1,   KC_2,   KC_3,   KC_4,   KC_5,
  ______, ______, ______, ______, ______, ______, ______,
  ______, ______, ______, ______, ______, ______, ______,
  ,       ,       ,       ,       ______, ______, ______, ______,   TT(HARDWARE) ,


  // right side
  ,       ,       KC_6,   KC_7, KC_8, KC_9, KC_0,     KC_MINUS, KC_EQL,
  ,       ,       ______, KC_4, KC_5, KC_6, KC_MINUS, KC_EQL,   ______,
  ,       ,       ______, KC_1, KC_2, KC_3, ______,   ______,   ______,
  ______, ______, ______, KC_0, KC_DOT
  ),

[CONTROL] =
LAYER(
  // left side
  ______, ______, KC_F1,  KC_F2,  KC_F3,   KC_F4,   KC_F5,
  ______, ______, ______, ______, ______,  ______,  ______,
  ______, ______, ______, ______, KC_MUTE, KC_VOLD, KC_VOLU,
  ,       ,       ,       ,       ______,  ______,  KC_BSPC, ______, ______,


  // right side
  ,       ,             KC_F6,   KC_F7,   KC_F8,   KC_F9,    KC_F10,  KC_F11, KC_F12,
  ,       ,             KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, ______,  ______, ______,
  ,       ,             KC_MPRV, KC_MPLY, KC_MNXT, ______,   ______,  ______, ______,
  ______, TT(HARDWARE), KC_BSPC, ______,  ______
  ),

[HARDWARE] =
LAYER(
  // left side
  ______, ______, ______,  ______, ______, ______, RESET,
  ______, ______, ______,  ______, ______, ______, ______,
  ______, ______, ______,  ______, ______, ______, ______,
  ,       ,       ,        ,       ______, ______, ______, ______, ______,


  // right side
  ,       ,       ______,  ______, ______, ______, ______, ______, ______,
  ,       ,       ______,  ______, ______, ______, ______, ______, ______,
  ,       ,       ______,  ______, ______, ______, ______, ______, ______,
  ______, ______, KC_BSPC, ______, ______
  )
};
#undef _______

void matrix_init_user(void)
{
}

#define IS_MOD_PRESSED(mod) ((bool)(get_mods() & MOD_BIT(mod)))

uint32_t last_shift = 0;
bool caps_down = false;
// bool spc_shft = false;
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // dprintf("rep: %d\n", record->tap.count);
  // if (!record->event.pressed) {
  // num++;
  // }
  // LED_TOGGLE;
  // matrix_print();
  if (keycode == KC_LSHIFT) {
    if (!record->event.pressed) {
      uint32_t timer_now = timer_read32();
      if (TIMER_DIFF_32(timer_now, last_shift)<300) {
        register_code(KC_CAPS);
        wait_ms(100);
        unregister_code(KC_CAPS);
        caps_down = !caps_down;
        return true;
      }
      last_shift = timer_now;
    }
  } else {
    last_shift = 0;
  }

  switch (keycode) {
    // SPC + LSHIFT = BSPC
  // case KC_SPC:
  //   if (IS_MOD_PRESSED(KC_LSHIFT)) {
  //     if (IS_PRESSED(record->event)) {
  //       spc_shft = true;
  //       SEND_STRING(SS_DOWN(X_BSPACE));
  //       return false;
  //     }
  //   }
  //   if (spc_shft) {
  //   spc_shft = false;
  //     SEND_STRING(SS_UP(X_BSPACE));
  //     return false;
  //   }
  //   break;

  case KC_ESC:
    if (caps_down) {
      SEND_STRING(SS_TAP(X_CAPSLOCK));
      caps_down = false;
    }
    break;
  case MACRO11:
    // dprintf("FREE RAM: %d\n", getFreeSram());
    break;
  case MACRO12:
    break;
  case MACRO13:
    break;
  };
  return true;
}

void matrix_scan_user(void)
{
  iota_gfx_task();
}

// uint16_t old_num = 0;
void iota_gfx_task_user(void)
{
  // if (num != old_num) {
  // matrix_draw_dsg(&display, num, 1);
  // old_num = num;
  // }
}
#undef IS_MOD_PRESSED
