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
  MACRO11,
  MACRO12,
  MACRO13
};

#define BASE 0 // default layer
#define NUMBERS 1 // MOD (green) layer with numbers
#define CONTROL 2 // controlling (blue) layer (arrow keys, F-Keys, Consumer Keys..)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[BASE] =
LAYER_PRETTY(
      KC_GRAVE,   KC_TAB,  KC_Q,      KC_W,    KC_E,  KC_R,  KC_T,     KC_Y,     KC_U,  KC_I,    KC_O,   KC_P,    KC_LBRC, KC_RBRC,
      MACRO12,    KC_ESC,  KC_A,      KC_S,    KC_D,  KC_F,  KC_G,     KC_H,     KC_J,  KC_K,    KC_L,   KC_SCLN, KC_QUOT, KC_ENT,
      KC_APP,  KC_GRAVE,  KC_Z,      KC_X,    KC_C,  KC_V,  KC_B,     KC_N,     KC_M,  KC_COMM, KC_DOT, KC_SLSH, KC_BSLS, KC_TRNS,
      KC_LALT,   KC_LGUI, KC_LSHIFT, KC_LCTL, TT(CONTROL),             KC_TRNS, TT(NUMBERS), KC_SPACE, TT(CONTROL), KC_RGUI
      ),
[NUMBERS] =
LAYER_PRETTY(
  KC_TRNS, KC_TRNS, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,      KC_6,    KC_7, KC_8, KC_9, KC_0,  KC_MINUS, KC_EQL,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_4, KC_5, KC_6, KC_TRNS, KC_TRNS,    KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_1, KC_2, KC_3, KC_TRNS, KC_TRNS, KC_TRNS,
                    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_0, KC_DOT
  ),
[CONTROL] =
LAYER_PRETTY(
  KC_TRNS, KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,   KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MUTE, KC_VOLD, KC_VOLU,   KC_MPRV, KC_MPLY, KC_MNXT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_BSPC, KC_TRNS, KC_TRNS
  )
};

void matrix_init_user(void)
{
}

#define IS_MOD_PRESSED(mod) ((bool)(get_mods() & MOD_BIT(mod)))

uint32_t last_shift = 0;
bool caps_down = false;
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
  case KC_SPC:
    if (IS_MOD_PRESSED(KC_LSHIFT)) {
      if (IS_PRESSED(record->event)) {
        SEND_STRING(SS_DOWN(X_BSPACE));
      } else {
        SEND_STRING(SS_UP(X_BSPACE));
      }
      return false;
    }
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
